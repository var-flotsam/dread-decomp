// NVN memory pools and buffers over Vulkan (include/nvn_shim/memory.h). Signatures are those of the
// game's calls: fn_710015385c and fn_7100153350 for the pool builder and MemoryPoolInitialize, the
// shader loader fn_710015c9b0 for the buffer builder, BufferInitialize, BufferGetAddress and
// BufferMap, fn_71001587f0 for BufferBuilderSetDevice.

#include "nvn_shim/memory.h"

#include <cstdio>
#include <cstring>
#include <map>

#include <vulkan/vulkan.h>

#include "nvn_shim/device.h"
#include "nvn_shim/procs.h"
#include "objects.h"

namespace dread::nvn {

namespace {

constexpr BufferAddress kAddressBase = 0x100000000ull;  // 4 GB: no game pointer looks like a GPU address
constexpr std::uint64_t kAddressAlignment = 0x10000;

struct MemoryPoolBuilder {
    const void* device = nullptr;
    std::int32_t flags = 0;
    void* storage = nullptr;
    std::uint64_t size = 0;
};

struct MemoryPool {
    void* storage = nullptr;
    std::uint64_t size = 0;
    std::int32_t flags = 0;
    BufferAddress address = 0;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    void* mapped = nullptr;  // the Vulkan memory, host-visible and mapped
    bool imported = false;   // the Vulkan memory is the game's storage itself (ImportPoolStorage)
    bool vulkan_failed = false;  // creating the Vulkan buffer failed; not retried
};

struct BufferBuilder {
    const void* device = nullptr;
    const void* pool = nullptr;
    std::uint64_t offset = 0;
    std::uint64_t size = 0;
};

struct Buffer {
    const void* pool = nullptr;
    std::uint64_t offset = 0;
    std::uint64_t size = 0;
};

detail::Registry<MemoryPoolBuilder> g_pool_builders;
detail::Registry<MemoryPool> g_pools;
detail::Registry<BufferBuilder> g_buffer_builders;
detail::Registry<Buffer> g_buffers;
std::map<BufferAddress, const void*> g_by_address;  // pool GPU address -> pool storage key
BufferAddress g_next_address = kAddressBase;

bool CreatePoolBuffer(MemoryPool& pool);

// The pool's Vulkan buffer, made on first use; false if it cannot be (reported once).
bool EnsurePoolBuffer(MemoryPool& pool) {
    if (pool.buffer)
        return true;
    if (pool.vulkan_failed)
        return false;
    if (!CreatePoolBuffer(pool)) {
        pool.vulkan_failed = true;
        std::fprintf(stderr, "nvn shim: no Vulkan memory for a %llu-byte memory pool\n",
                     static_cast<unsigned long long>(pool.size));
        return false;
    }
    return true;
}

// The pool holding [address, address + size), under the state lock.
MemoryPool* PoolAt(BufferAddress address, std::uint64_t size) {
    auto it = g_by_address.upper_bound(address);
    if (it == g_by_address.begin())
        return nullptr;
    --it;
    MemoryPool* pool = g_pools.Find(it->second);
    if (!pool || address < pool->address || address - pool->address + size > pool->size)
        return nullptr;
    return pool;
}

constexpr VkBufferUsageFlags kPoolBufferUsage =
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
    VK_BUFFER_USAGE_TRANSFER_DST_BIT;

// The pool's Vulkan buffer over the game's own storage, when the device can import host memory
// (VK_EXT_external_memory_host) and the storage is aligned as it needs (the game's pools are 4 KB
// aligned, fn_710015385c): the GPU then reads what the game writes, as on the console, and nothing
// is copied. False (and nothing made) when it cannot.
bool ImportPoolStorage(const Backend& b, MemoryPool& pool) {
    const VkDeviceSize align = b.host_import_alignment;
    if (!b.host_import || !align || reinterpret_cast<std::uintptr_t>(pool.storage) % align || pool.size % align)
        return false;
    auto get_properties = reinterpret_cast<PFN_vkGetMemoryHostPointerPropertiesEXT>(
        vkGetDeviceProcAddr(b.device, "vkGetMemoryHostPointerPropertiesEXT"));
    VkMemoryHostPointerPropertiesEXT props{VK_STRUCTURE_TYPE_MEMORY_HOST_POINTER_PROPERTIES_EXT};
    if (!get_properties ||
        get_properties(b.device, VK_EXTERNAL_MEMORY_HANDLE_TYPE_HOST_ALLOCATION_BIT_EXT, pool.storage, &props) != VK_SUCCESS)
        return false;
    VkExternalMemoryBufferCreateInfo external{VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO};
    external.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_HOST_ALLOCATION_BIT_EXT;
    VkBufferCreateInfo bci{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    bci.pNext = &external;
    bci.size = pool.size;
    bci.usage = kPoolBufferUsage;
    if (vkCreateBuffer(b.device, &bci, nullptr, &pool.buffer) != VK_SUCCESS) {
        pool.buffer = VK_NULL_HANDLE;
        return false;
    }
    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(b.device, pool.buffer, &req);
    VkImportMemoryHostPointerInfoEXT import{VK_STRUCTURE_TYPE_IMPORT_MEMORY_HOST_POINTER_INFO_EXT};
    import.handleType = VK_EXTERNAL_MEMORY_HANDLE_TYPE_HOST_ALLOCATION_BIT_EXT;
    import.pHostPointer = pool.storage;
    VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mai.pNext = &import;
    mai.allocationSize = pool.size;
    // Coherent only: the game never flushes what it writes (nvnMemoryPoolFlushMappedRange is not
    // among the functions it calls).
    mai.memoryTypeIndex = b.MemoryType(req.memoryTypeBits & props.memoryTypeBits,
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (req.size > pool.size || mai.memoryTypeIndex == UINT32_MAX ||
        vkAllocateMemory(b.device, &mai, nullptr, &pool.memory) != VK_SUCCESS ||
        vkBindBufferMemory(b.device, pool.buffer, pool.memory, 0) != VK_SUCCESS) {
        if (pool.memory)
            vkFreeMemory(b.device, pool.memory, nullptr);
        vkDestroyBuffer(b.device, pool.buffer, nullptr);
        pool.buffer = VK_NULL_HANDLE;
        pool.memory = VK_NULL_HANDLE;
        return false;
    }
    pool.mapped = pool.storage;
    pool.imported = true;
    return true;
}

bool CreatePoolBuffer(MemoryPool& pool) {
    const Backend* b = CurrentBackend();
    if (!b)
        return false;
    if (ImportPoolStorage(*b, pool))
        return true;
    // Otherwise a host-visible copy, refreshed from the game's storage by SyncPoolRange.
    VkBufferCreateInfo bci{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    bci.size = pool.size;
    bci.usage = kPoolBufferUsage;
    if (vkCreateBuffer(b->device, &bci, nullptr, &pool.buffer) != VK_SUCCESS)
        return false;
    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(b->device, pool.buffer, &req);
    VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mai.allocationSize = req.size;
    mai.memoryTypeIndex =
        b->MemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (mai.memoryTypeIndex == UINT32_MAX || vkAllocateMemory(b->device, &mai, nullptr, &pool.memory) != VK_SUCCESS ||
        vkBindBufferMemory(b->device, pool.buffer, pool.memory, 0) != VK_SUCCESS ||
        vkMapMemory(b->device, pool.memory, 0, VK_WHOLE_SIZE, 0, &pool.mapped) != VK_SUCCESS) {
        if (pool.memory)
            vkFreeMemory(b->device, pool.memory, nullptr);
        vkDestroyBuffer(b->device, pool.buffer, nullptr);
        pool.buffer = VK_NULL_HANDLE;
        pool.memory = VK_NULL_HANDLE;
        return false;
    }
    return true;
}

void DestroyPoolBuffer(MemoryPool& pool) {
    const Backend* b = CurrentBackend();
    if (b && pool.buffer)
        WaitFor(LastSubmitted(), UINT64_MAX);  // the GPU may still read it (the game finalises pools rarely)
    if (b) {
        if (pool.memory)
            vkFreeMemory(b->device, pool.memory, nullptr);  // unmaps (an imported pool's storage stays the game's)
        if (pool.buffer)
            vkDestroyBuffer(b->device, pool.buffer, nullptr);
    }
    pool.buffer = VK_NULL_HANDLE;
    pool.memory = VK_NULL_HANDLE;
    pool.mapped = nullptr;
    pool.imported = false;
    pool.vulkan_failed = false;
}

// --- memory pool builder and pool ---

void MemoryPoolBuilderSetDefaults(void* builder) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_pool_builders.ResetKeepingDevice(builder);
}
const void* MemoryPoolBuilderGetDevice(const void* builder) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    MemoryPoolBuilder* b = g_pool_builders.Find(builder);
    return b ? b->device : nullptr;
}
void MemoryPoolBuilderSetDevice(void* builder, const void* device) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_pool_builders.Get(builder).device = device;
}
void MemoryPoolBuilderSetFlags(void* builder, std::int32_t flags) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_pool_builders.Get(builder).flags = flags;
}
void MemoryPoolBuilderSetStorage(void* builder, void* storage, std::uint64_t size) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    MemoryPoolBuilder& b = g_pool_builders.Get(builder);
    b.storage = storage;
    b.size = size;
}

std::int32_t MemoryPoolInitialize(void* pool_storage, const void* builder) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    MemoryPoolBuilder* b = g_pool_builders.Find(builder);
    if (!b || !b->storage || !b->size) {
        std::fprintf(stderr, "nvn shim: nvnMemoryPoolInitialize without storage\n");
        return 0;
    }
    if (MemoryPool* old = g_pools.Find(pool_storage)) {  // initialised twice without Finalize
        g_by_address.erase(old->address);
        DestroyPoolBuffer(*old);
    }
    MemoryPool& pool = g_pools.Reset(pool_storage);
    pool.storage = b->storage;
    pool.size = b->size;
    pool.flags = b->flags;
    // The Vulkan buffer is made when the GPU first needs the pool (EnsurePoolBuffer): the engine's
    // own pools (fn_7100153350: 300 MB, 1008 MB, 208 MB, 12 MB, 1 MB) would otherwise double 1.5 GB
    // of memory whether or not anything the GPU reads ever lands in them.
    pool.address = g_next_address;
    g_next_address += (pool.size + kAddressAlignment - 1) / kAddressAlignment * kAddressAlignment;
    g_by_address[pool.address] = pool_storage;
    return 1;
}

void MemoryPoolFinalize(void* pool_storage) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (MemoryPool* pool = g_pools.Find(pool_storage)) {
        g_by_address.erase(pool->address);
        DestroyPoolBuffer(*pool);
        g_pools.Erase(pool_storage);
    }
}

void* MemoryPoolMap(const void* pool_storage) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    MemoryPool* pool = g_pools.Find(pool_storage);
    return pool ? pool->storage : nullptr;
}

BufferAddress MemoryPoolGetBufferAddress(const void* pool_storage) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    MemoryPool* pool = g_pools.Find(pool_storage);
    return pool ? pool->address : 0;
}

std::uint64_t MemoryPoolGetSize(const void* pool_storage) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    MemoryPool* pool = g_pools.Find(pool_storage);
    return pool ? pool->size : 0;
}

std::int32_t MemoryPoolGetFlags(const void* pool_storage) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    MemoryPool* pool = g_pools.Find(pool_storage);
    return pool ? pool->flags : 0;
}

// The game's storage is the CPU's view: the GPU reads it directly (an imported pool) or a copy the
// submit refreshes (SyncPoolRange), so flushing and invalidating mapped ranges have nothing to do.
void MemoryPoolFlushMappedRange(const void*, std::int64_t, std::uint64_t) {}
void MemoryPoolInvalidateMappedRange(const void*, std::int64_t, std::uint64_t) {}

void MemoryPoolSetDebugLabel(void*, const char*) {}

// --- buffer builder and buffer ---

void BufferBuilderSetDefaults(void* builder) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_buffer_builders.ResetKeepingDevice(builder);
}
const void* BufferBuilderGetDevice(const void* builder) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    BufferBuilder* b = g_buffer_builders.Find(builder);
    return b ? b->device : nullptr;
}
void BufferBuilderSetDevice(void* builder, const void* device) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_buffer_builders.Get(builder).device = device;
}
void BufferBuilderSetStorage(void* builder, const void* pool, std::int64_t offset, std::uint64_t size) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    BufferBuilder& b = g_buffer_builders.Get(builder);
    b.pool = pool;
    b.offset = static_cast<std::uint64_t>(offset);
    b.size = size;
}

std::int32_t BufferInitialize(void* buffer_storage, const void* builder) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    BufferBuilder* b = g_buffer_builders.Find(builder);
    MemoryPool* pool = b ? g_pools.Find(b->pool) : nullptr;
    if (!pool || b->offset + b->size > pool->size) {
        std::fprintf(stderr, "nvn shim: nvnBufferInitialize outside a memory pool\n");
        return 0;
    }
    Buffer& buffer = g_buffers.Reset(buffer_storage);
    buffer.pool = b->pool;
    buffer.offset = b->offset;
    buffer.size = b->size;
    return 1;
}

void BufferFinalize(void* buffer_storage) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_buffers.Erase(buffer_storage);
}

BufferAddress BufferGetAddress(const void* buffer_storage) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    Buffer* buffer = g_buffers.Find(buffer_storage);
    MemoryPool* pool = buffer ? g_pools.Find(buffer->pool) : nullptr;
    return pool ? pool->address + buffer->offset : 0;
}

void* BufferMap(const void* buffer_storage) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    Buffer* buffer = g_buffers.Find(buffer_storage);
    MemoryPool* pool = buffer ? g_pools.Find(buffer->pool) : nullptr;
    return pool ? static_cast<char*>(pool->storage) + buffer->offset : nullptr;
}

std::uint64_t BufferGetSize(const void* buffer_storage) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    Buffer* buffer = g_buffers.Find(buffer_storage);
    return buffer ? buffer->size : 0;
}

void BufferFlushMappedRange(const void*, std::int64_t, std::uint64_t) {}
void BufferInvalidateMappedRange(const void*, std::int64_t, std::uint64_t) {}
void BufferSetDebugLabel(void*, const char*) {}

}  // namespace

bool ResolveGpuAddress(BufferAddress address, std::uint64_t size, VkBuffer* buffer, VkDeviceSize* offset) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    MemoryPool* pool = PoolAt(address, size);
    if (!pool || !EnsurePoolBuffer(*pool))
        return false;
    *buffer = pool->buffer;
    *offset = address - pool->address;
    return true;
}

bool SyncPoolRange(BufferAddress address, std::uint64_t size) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    MemoryPool* pool = PoolAt(address, size);
    if (!pool || !EnsurePoolBuffer(*pool) || !pool->mapped)
        return false;
    if (pool->imported)
        return true;  // the GPU reads the game's storage itself
    const std::uint64_t off = address - pool->address;
    std::memcpy(static_cast<char*>(pool->mapped) + off, static_cast<const char*>(pool->storage) + off, size);
    return true;
}

void* GpuAddressToStorage(BufferAddress address) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    MemoryPool* pool = PoolAt(address, 1);
    return pool ? static_cast<char*>(pool->storage) + (address - pool->address) : nullptr;
}

void detail::ReleaseMemoryVulkanObjects() {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_pools.ForEach([](const void*, MemoryPool& pool) { DestroyPoolBuffer(pool); });
}

std::size_t PoolsWithVulkanBuffer() {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    std::size_t n = 0;
    g_pools.ForEach([&](const void*, MemoryPool& pool) { n += pool.buffer != VK_NULL_HANDLE; });
    return n;
}

std::size_t PoolsImported() {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    std::size_t n = 0;
    g_pools.ForEach([&](const void*, MemoryPool& pool) { n += pool.imported; });
    return n;
}

std::size_t MemoryPoolCount() {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    return g_pools.size();
}

std::uint64_t GpuAddressSpaceUsed() {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    return g_next_address - kAddressBase;
}

void detail::RegisterMemoryProcs() {
    Register("nvnMemoryPoolBuilderSetDefaults", &MemoryPoolBuilderSetDefaults);
    Register("nvnMemoryPoolBuilderGetDevice", &MemoryPoolBuilderGetDevice);
    Register("nvnMemoryPoolBuilderSetDevice", &MemoryPoolBuilderSetDevice);
    Register("nvnMemoryPoolBuilderSetFlags", &MemoryPoolBuilderSetFlags);
    Register("nvnMemoryPoolBuilderSetStorage", &MemoryPoolBuilderSetStorage);
    Register("nvnMemoryPoolInitialize", &MemoryPoolInitialize);
    Register("nvnMemoryPoolFinalize", &MemoryPoolFinalize);
    Register("nvnMemoryPoolMap", &MemoryPoolMap);
    Register("nvnMemoryPoolGetBufferAddress", &MemoryPoolGetBufferAddress);
    Register("nvnMemoryPoolGetSize", &MemoryPoolGetSize);
    Register("nvnMemoryPoolGetFlags", &MemoryPoolGetFlags);
    Register("nvnMemoryPoolFlushMappedRange", &MemoryPoolFlushMappedRange);
    Register("nvnMemoryPoolInvalidateMappedRange", &MemoryPoolInvalidateMappedRange);
    Register("nvnMemoryPoolSetDebugLabel", &MemoryPoolSetDebugLabel);
    Register("nvnBufferBuilderSetDefaults", &BufferBuilderSetDefaults);
    Register("nvnBufferBuilderGetDevice", &BufferBuilderGetDevice);
    Register("nvnBufferBuilderSetDevice", &BufferBuilderSetDevice);
    Register("nvnBufferBuilderSetStorage", &BufferBuilderSetStorage);
    Register("nvnBufferInitialize", &BufferInitialize);
    Register("nvnBufferFinalize", &BufferFinalize);
    Register("nvnBufferGetAddress", &BufferGetAddress);
    Register("nvnBufferMap", &BufferMap);
    Register("nvnBufferGetSize", &BufferGetSize);
    Register("nvnBufferFlushMappedRange", &BufferFlushMappedRange);
    Register("nvnBufferInvalidateMappedRange", &BufferInvalidateMappedRange);
    Register("nvnBufferSetDebugLabel", &BufferSetDebugLabel);
}

}  // namespace dread::nvn
