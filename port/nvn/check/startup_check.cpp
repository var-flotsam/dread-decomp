// nvn_startup_check
//
// Replays the game's graphics start-up and frame loop against the shim, in the game's order and
// with its argument shapes, as far as they are known from our own sources and the call sites
// (port/nvn/README.md): the graphics start-up fn_71001587f0 (src/game_s7/fn_71001587f0.cpp), the
// memory pool set-up fn_710015385c, the buffer creation of the shader loader fn_710015c9b0, the
// frame's begin fn_7100159920, end fn_7100159ac0 and present fn_7100159bf0, the texture and sampler
// pools fn_710015f100, the sampler fn_710015e9fc, the render targets fn_7100160f60 and the cube
// target fn_710015f880. The game's objects live in storage this program owns, as the game's
// do in its own structs; every function is fetched by name through nvnBootstrapLoader and
// nvnDeviceGetProcAddress, as fn_71001587f0 and fn_7100154770 do.
//
// It checks what the implemented parts must do (the device, queue, command buffers, pools and
// buffers, syncs, state objects, window) and prints, in the order reached, the NVN functions the
// shim still traps: the next work list. Exit status 0 when every check passes; traps are not
// failures. Needs a Vulkan device (the port's Vulkan image: lavapipe); $DREAD_NVN_VALIDATION=1 adds
// the validation layers, whose errors fail the check.

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "nvn_shim/command.h"
#include "nvn_shim/device.h"
#include "nvn_shim/memory.h"
#include "nvn_shim/procs.h"
#include "nvn_shim/queue.h"
#include "nvn_shim/texture.h"
#include "nvn_shim/window.h"

using namespace dread::nvn;

namespace {

int g_failures = 0;
void Check(bool ok, const char* what) {
    if (!ok) {
        ++g_failures;
        std::fprintf(stderr, "FAIL %s\n", what);
    }
}

using GetProcFn = void* (*)(const void* device, const char* name);
GetProcFn g_get_proc;

template <class F>
F Fn(const char* name) {
    return reinterpret_cast<F>(g_get_proc(nullptr, name));
}

// Storage for one game object: generous, 16-aligned, zeroed; the shim never writes into it.
struct alignas(16) Storage {
    unsigned char bytes[0x400] = {};
};

void* AlignedAlloc(std::size_t size, std::size_t align) {
    void* p = nullptr;
    if (posix_memalign(&p, align, size) != 0)
        return nullptr;
    std::memset(p, 0, size);
    return p;
}

// Storage for a large pool, reserved but never touched (the kernel commits pages on first write),
// as the engine's own pools are before it fills them.
void* LargeStorage(std::size_t size) {
    void* p = nullptr;
    return posix_memalign(&p, 0x1000, size) == 0 ? p : nullptr;
}

}  // namespace

int main() {
    // fn_71001587f0: the bootstrap loader, then the device's entry points.
    auto get_proc = reinterpret_cast<GetProcFn>(nvnBootstrapLoader("nvnDeviceGetProcAddress"));
    Check(get_proc != nullptr, "nvnBootstrapLoader(nvnDeviceGetProcAddress)");
    if (!get_proc)
        return 1;
    g_get_proc = get_proc;
    Check(reinterpret_cast<void*>(get_proc(nullptr, "nvnDeviceGetProcAddress")) == reinterpret_cast<void*>(get_proc),
          "nvnDeviceGetProcAddress returns itself");

    Storage device, device_builder, queue, queue_builder, sync0, window_sync, window_builder_storage, window;
    Storage blend, channel_mask, color, depth_stencil, multisample, polygon, buffer_builder, texture_builder;
    Storage cmd_builders[2];
    Storage pools[2];  // the engine's pool record holds the NVN pool at +8; ours is the object itself

    auto DeviceBuilderSetDefaults = Fn<void (*)(void*)>("nvnDeviceBuilderSetDefaults");
    auto DeviceBuilderSetFlags = Fn<void (*)(void*, std::int32_t)>("nvnDeviceBuilderSetFlags");
    auto DeviceInitialize = Fn<std::int32_t (*)(void*, void*)>("nvnDeviceInitialize");
    auto DeviceGetInteger = Fn<void (*)(const void*, std::int32_t, std::int32_t*)>("nvnDeviceGetInteger");
    auto DeviceSetDepthMode = Fn<void (*)(void*, std::int32_t)>("nvnDeviceSetDepthMode");
    auto DeviceSetWindowOriginMode = Fn<void (*)(void*, std::int32_t)>("nvnDeviceSetWindowOriginMode");
    auto DeviceRegisterFastClearColor = Fn<void (*)(void*, const float*, std::int32_t)>("nvnDeviceRegisterFastClearColor");
    auto DeviceRegisterFastClearDepth = Fn<void (*)(void*, float)>("nvnDeviceRegisterFastClearDepth");

    std::int32_t major = -1, minor = -1;
    DeviceGetInteger(nullptr, 0, &major);
    DeviceGetInteger(nullptr, 1, &minor);
    DeviceBuilderSetDefaults(&device_builder);
    DeviceBuilderSetFlags(&device_builder, 0);
    Check(DeviceInitialize(&device, &device_builder) == 1, "nvnDeviceInitialize");
    Check(CurrentBackend() != nullptr, "a Vulkan backend after nvnDeviceInitialize");
    DeviceSetDepthMode(&device, 0);
    DeviceSetWindowOriginMode(&device, 1);
    const float clear[4] = {};
    DeviceRegisterFastClearColor(&device, clear, 0xa);
    DeviceRegisterFastClearColor(&device, clear, 0x3f);
    DeviceRegisterFastClearColor(&device, clear, 0x25);
    DeviceRegisterFastClearDepth(&device, 1.0f);

    // The queue, its memory from the game's allocator, 4 KB aligned.
    auto QueueBuilderSetDevice = Fn<void (*)(void*, const void*)>("nvnQueueBuilderSetDevice");
    auto QueueBuilderSetDefaults = Fn<void (*)(void*)>("nvnQueueBuilderSetDefaults");
    auto QueueBuilderSetComputeMemorySize = Fn<void (*)(void*, std::int64_t)>("nvnQueueBuilderSetComputeMemorySize");
    auto QueueBuilderSetCommandMemorySize = Fn<void (*)(void*, std::int64_t)>("nvnQueueBuilderSetCommandMemorySize");
    auto QueueBuilderSetCommandFlushThreshold = Fn<void (*)(void*, std::int64_t)>("nvnQueueBuilderSetCommandFlushThreshold");
    auto QueueBuilderGetQueueMemorySize = Fn<std::uint64_t (*)(const void*)>("nvnQueueBuilderGetQueueMemorySize");
    auto QueueBuilderSetQueueMemory = Fn<void (*)(void*, void*, std::uint64_t)>("nvnQueueBuilderSetQueueMemory");
    auto QueueInitialize = Fn<std::int32_t (*)(void*, const void*)>("nvnQueueInitialize");
    QueueBuilderSetDevice(&queue_builder, &device);
    QueueBuilderSetDefaults(&queue_builder);
    Check(Fn<const void* (*)(const void*)>("nvnQueueBuilderGetDevice")(&queue_builder) == &device,
          "the queue builder keeps its device across SetDefaults (fn_71001587f0 sets it first)");
    QueueBuilderSetComputeMemorySize(&queue_builder, 0);
    std::int32_t command_size = 0;
    DeviceGetInteger(&device, 0x51, &command_size);
    Check(command_size > 0, "nvnDeviceGetInteger(0x51) > 0");
    QueueBuilderSetCommandMemorySize(&queue_builder, command_size);
    QueueBuilderSetCommandFlushThreshold(&queue_builder, command_size);
    const std::uint64_t queue_memory_size = QueueBuilderGetQueueMemorySize(&queue_builder);
    void* queue_memory = AlignedAlloc(queue_memory_size, 0x1000);
    QueueBuilderSetQueueMemory(&queue_builder, queue_memory, queue_memory_size);
    Check(QueueInitialize(&queue, &queue_builder) == 1, "nvnQueueInitialize");

    // fn_710015385c for each command buffer: a 15 MB command memory pool (flags 0x22), and 2 MB of
    // control memory, both aligned as nvnDeviceGetInteger(0x13 / 0x14) says.
    auto MemoryPoolBuilderSetDefaults = Fn<void (*)(void*)>("nvnMemoryPoolBuilderSetDefaults");
    auto MemoryPoolBuilderSetDevice = Fn<void (*)(void*, const void*)>("nvnMemoryPoolBuilderSetDevice");
    auto MemoryPoolBuilderSetFlags = Fn<void (*)(void*, std::int32_t)>("nvnMemoryPoolBuilderSetFlags");
    auto MemoryPoolBuilderSetStorage = Fn<void (*)(void*, void*, std::uint64_t)>("nvnMemoryPoolBuilderSetStorage");
    auto MemoryPoolInitialize = Fn<std::int32_t (*)(void*, const void*)>("nvnMemoryPoolInitialize");
    auto CommandBufferInitialize = Fn<std::int32_t (*)(void*, const void*)>("nvnCommandBufferInitialize");
    auto CommandBufferAddCommandMemory = Fn<void (*)(void*, const void*, std::int64_t, std::uint64_t)>("nvnCommandBufferAddCommandMemory");
    auto CommandBufferAddControlMemory = Fn<void (*)(void*, void*, std::uint64_t)>("nvnCommandBufferAddControlMemory");
    auto CommandBufferSetMemoryCallback = Fn<void (*)(void*, void*)>("nvnCommandBufferSetMemoryCallback");
    // fn_7100153350: the engine's own pools, 300 MB, 1008 MB, 208 MB, 12 MB and 1 MB, flags 0x22,
    // 0x21 and 0x24. Their Vulkan buffers are made only when the GPU first needs them.
    const struct {
        std::uint64_t size;
        std::int32_t flags;
    } kEnginePools[] = {{300ull << 20, 0x22}, {1008ull << 20, 0x21}, {208ull << 20, 0x24}, {12ull << 20, 0x22},
                        {1ull << 20, 0x22}};
    Storage engine_pools[5];
    void* engine_storage[5] = {};
    for (int i = 0; i < 5; ++i) {
        Storage builder;
        engine_storage[i] = LargeStorage(kEnginePools[i].size);
        Check(engine_storage[i] != nullptr, "storage for an engine pool");
        MemoryPoolBuilderSetDefaults(&builder);
        MemoryPoolBuilderSetDevice(&builder, &device);
        MemoryPoolBuilderSetFlags(&builder, kEnginePools[i].flags);
        MemoryPoolBuilderSetStorage(&builder, engine_storage[i], kEnginePools[i].size);
        Check(MemoryPoolInitialize(&engine_pools[i], &builder) == 1, "nvnMemoryPoolInitialize (an engine pool)");
    }
    Check(PoolsWithVulkanBuffer() == 0, "no Vulkan buffer before the GPU needs a pool");

    // fn_710015f100's integers and the pools it sizes with them.
    std::int32_t texture_size = 0, sampler_size = 0, reserved_textures = 0, reserved_samplers = 0, max_textures = 0,
                 max_samplers = 0;
    DeviceGetInteger(&device, 0xf, &texture_size);
    DeviceGetInteger(&device, 0x10, &sampler_size);
    DeviceGetInteger(&device, 0x11, &reserved_textures);
    DeviceGetInteger(&device, 0x24, &max_textures);
    DeviceGetInteger(&device, 0x12, &reserved_samplers);
    DeviceGetInteger(&device, 0x25, &max_samplers);
    const std::int32_t textures_n = std::min(reserved_textures + 0x800, max_textures);
    const std::int32_t samplers_n = std::min(reserved_samplers + 0x2000, max_samplers);
    Check(texture_size > 0 && sampler_size > 0 && reserved_textures > 0 && reserved_samplers > 0,
          "descriptor sizes and reserved counts");
    Check(textures_n == reserved_textures + 0x800 && samplers_n == reserved_samplers + 0x2000,
          "the pools hold every texture and sampler the game asks for");

    std::int32_t command_alignment = 0, control_alignment = 0;
    DeviceGetInteger(&device, 0x13, &command_alignment);
    DeviceGetInteger(&device, 0x14, &control_alignment);
    Check(command_alignment > 0 && control_alignment > 0, "command and control alignments");
    const std::uint64_t kCommandPoolSize = 0xf00000, kControlSize = 0x200000;
    void* control_memory[2] = {};
    for (int i = 0; i < 2; ++i) {
        Storage builder;
        void* storage = AlignedAlloc(kCommandPoolSize, 0x1000);
        MemoryPoolBuilderSetDefaults(&builder);
        MemoryPoolBuilderSetDevice(&builder, &device);
        MemoryPoolBuilderSetFlags(&builder, 0x22);
        MemoryPoolBuilderSetStorage(&builder, storage, kCommandPoolSize);
        Check(MemoryPoolInitialize(&pools[i], &builder) == 1, "nvnMemoryPoolInitialize (command memory)");
        Check(CommandBufferInitialize(&cmd_builders[i], &device) == 1, "nvnCommandBufferInitialize");
        control_memory[i] = AlignedAlloc(kControlSize, static_cast<std::size_t>(std::max(control_alignment, 8)));
        CommandBufferAddCommandMemory(&cmd_builders[i], &pools[i], 0, kCommandPoolSize);
        CommandBufferAddControlMemory(&cmd_builders[i], control_memory[i], kControlSize);
        CommandBufferSetMemoryCallback(&cmd_builders[i], reinterpret_cast<void*>(&AlignedAlloc));
    }
    Check(MemoryPoolCount() == 7, "seven memory pools (five of the engine's, two of command memory)");

    // Syncs and the fixed-function state objects, set as fn_71001587f0 sets them.
    auto SyncInitialize = Fn<std::int32_t (*)(void*, const void*)>("nvnSyncInitialize");
    Check(SyncInitialize(&sync0, &device) == 1 && SyncInitialize(&window_sync, &device) == 1, "nvnSyncInitialize");
    Fn<void (*)(void*)>("nvnBlendStateSetDefaults")(&blend);
    Fn<void (*)(void*)>("nvnChannelMaskStateSetDefaults")(&channel_mask);
    Fn<void (*)(void*)>("nvnColorStateSetDefaults")(&color);
    Fn<void (*)(void*)>("nvnDepthStencilStateSetDefaults")(&depth_stencil);
    Fn<void (*)(void*)>("nvnMultisampleStateSetDefaults")(&multisample);
    Fn<void (*)(void*)>("nvnPolygonStateSetDefaults")(&polygon);
    Fn<void (*)(void*, std::int32_t)>("nvnPolygonStateSetFrontFace")(&polygon, 1);
    Fn<void (*)(void*, std::uint8_t)>("nvnDepthStencilStateSetDepthTestEnable")(&depth_stencil, 1);
    Fn<void (*)(void*, std::uint8_t)>("nvnDepthStencilStateSetDepthWriteEnable")(&depth_stencil, 1);
    Fn<void (*)(void*, std::uint8_t)>("nvnDepthStencilStateSetStencilTestEnable")(&depth_stencil, 0);
    Fn<void (*)(void*, std::int32_t)>("nvnDepthStencilStateSetDepthFunc")(&depth_stencil, 3);
    Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnDepthStencilStateSetStencilFunc")(&depth_stencil, 1, 7);
    Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>("nvnDepthStencilStateSetStencilOp")(
        &depth_stencil, 1, 1, 1, 1);
    Fn<void (*)(void*, std::int32_t)>("nvnPolygonStateSetCullFace")(&polygon, 0);
    Fn<void (*)(void*, std::int32_t)>("nvnPolygonStateSetPolygonMode")(&polygon, 2);
    Fn<void (*)(void*, std::int32_t)>("nvnMultisampleStateSetMultisampleEnable")(&multisample, 0);
    Fn<void (*)(void*, const void*)>("nvnBufferBuilderSetDevice")(&buffer_builder, &device);
    Fn<void (*)(void*)>("nvnBufferBuilderSetDefaults")(&buffer_builder);
    Fn<void (*)(void*, const void*)>("nvnTextureBuilderSetDevice")(&texture_builder, &device);
    Fn<void (*)(void*)>("nvnTextureBuilderSetDefaults")(&texture_builder);

    // The shader loader's buffers (fn_710015c9b0): a uniform buffer in a pool, its address and map.
    {
        Storage builder, pool, buffer;
        const std::uint64_t size = 0x10000;
        void* storage = AlignedAlloc(size, 0x1000);
        MemoryPoolBuilderSetDefaults(&builder);
        MemoryPoolBuilderSetDevice(&builder, &device);
        MemoryPoolBuilderSetFlags(&builder, 0x22);
        MemoryPoolBuilderSetStorage(&builder, storage, size);
        Check(MemoryPoolInitialize(&pool, &builder) == 1, "nvnMemoryPoolInitialize (buffer memory)");
        Fn<void (*)(void*)>("nvnBufferBuilderSetDefaults")(&buffer_builder);
        Check(Fn<const void* (*)(const void*)>("nvnBufferBuilderGetDevice")(&buffer_builder) == &device,
              "the buffer builder keeps its device across SetDefaults (fn_710015c9b0 sets defaults per buffer)");
        Fn<void (*)(void*, const void*, std::int64_t, std::uint64_t)>("nvnBufferBuilderSetStorage")(&buffer_builder, &pool,
                                                                                                    0x100, 0x200);
        Check(Fn<std::int32_t (*)(void*, const void*)>("nvnBufferInitialize")(&buffer, &buffer_builder) == 1,
              "nvnBufferInitialize");
        const BufferAddress address = Fn<BufferAddress (*)(const void*)>("nvnBufferGetAddress")(&buffer);
        void* mapped = Fn<void* (*)(const void*)>("nvnBufferMap")(&buffer);
        Check(address >= 0x100000000ull, "a buffer address in the GPU address space");
        Check(mapped == static_cast<char*>(storage) + 0x100, "nvnBufferMap is the game's storage + offset");
        Check(GpuAddressToStorage(address) == mapped, "the address maps back to the storage");
        std::memset(mapped, 0x5a, 0x200);
        VkBuffer vk = VK_NULL_HANDLE;
        VkDeviceSize off = 0;
        Check(ResolveGpuAddress(address, 0x200, &vk, &off) && vk != VK_NULL_HANDLE && off == 0x100,
              "the binding layer's address resolution");
        Check(!ResolveGpuAddress(address, 0x10000, &vk, &off), "a range past the pool's end does not resolve");
        Check(SyncPoolRange(address, 0x200), "copying the range into the Vulkan buffer");
        Check(PoolsWithVulkanBuffer() == 1, "a Vulkan buffer only for the pool the GPU used");
        Check(Fn<std::uint64_t (*)(const void*)>("nvnBufferGetSize")(&buffer) == 0x200, "nvnBufferGetSize");
        Fn<void (*)(void*)>("nvnBufferFinalize")(&buffer);
        Fn<void (*)(void*)>("nvnMemoryPoolFinalize")(&pool);
        std::free(storage);
    }

    // fn_710015f100: the texture and sampler pools in an engine pool, sized as the game sizes them.
    Storage texture_pool, sampler_pool, sampler_builder;
    Check(Fn<std::int32_t (*)(void*, const void*, std::int64_t, std::int32_t)>("nvnSamplerPoolInitialize")(
              &sampler_pool, &engine_pools[4], 0, samplers_n) == 1,
          "nvnSamplerPoolInitialize");
    Check(Fn<std::int32_t (*)(void*, const void*, std::int64_t, std::int32_t)>("nvnTexturePoolInitialize")(
              &texture_pool, &engine_pools[4], static_cast<std::int64_t>(sampler_size) * samplers_n, textures_n) == 1,
          "nvnTexturePoolInitialize");
    Fn<void (*)(void*, const void*)>("nvnSamplerBuilderSetDevice")(&sampler_builder, &device);

    // fn_710015e9fc: a sampler made on the stack, registered, finalised at once.
    const std::int32_t sampler_id = reserved_samplers;
    {
        Storage sampler;
        const float border[4] = {0, 0, 0, 1};
        Fn<void (*)(void*)>("nvnSamplerBuilderSetDefaults")(&sampler_builder);
        Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnSamplerBuilderSetMinMagFilter")(&sampler_builder, 3, 1);
        Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t)>("nvnSamplerBuilderSetWrapMode")(&sampler_builder, 0, 0, 0);
        Fn<void (*)(void*, float, float)>("nvnSamplerBuilderSetLodClamp")(&sampler_builder, 0.0f, 13.0f);
        Fn<void (*)(void*, float)>("nvnSamplerBuilderSetLodBias")(&sampler_builder, 0.0f);
        Fn<void (*)(void*, const float*)>("nvnSamplerBuilderSetBorderColor")(&sampler_builder, border);
        Fn<void (*)(void*, float)>("nvnSamplerBuilderSetMaxAnisotropy")(&sampler_builder, 4.0f);
        Fn<void (*)(void*, std::int32_t)>("nvnSamplerBuilderSetReductionFilter")(&sampler_builder, 0);
        Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnSamplerBuilderSetCompare")(&sampler_builder, 1, 3);
        Check(Fn<std::int32_t (*)(void*, const void*)>("nvnSamplerInitialize")(&sampler, &sampler_builder) == 1,
              "nvnSamplerInitialize");
        Fn<void (*)(void*, std::int32_t, const void*)>("nvnSamplerPoolRegisterSampler")(&sampler_pool, sampler_id, &sampler);
        Fn<void (*)(void*)>("nvnSamplerFinalize")(&sampler);
    }
    Check(RegisteredSampler(static_cast<std::uint32_t>(sampler_id)), "the pool keeps the sampler after it is finalised");

    // fn_7100160f60: the depth target (0x35, flags 0xc) and two colour targets (0x25, flags 0xd),
    // storage suballocated from an engine pool, registered from the first id after the reserved ones.
    auto TB_SetDefaults = Fn<void (*)(void*)>("nvnTextureBuilderSetDefaults");
    auto TB_SetFlags = Fn<void (*)(void*, std::int32_t)>("nvnTextureBuilderSetFlags");
    auto TB_SetTarget = Fn<void (*)(void*, std::int32_t)>("nvnTextureBuilderSetTarget");
    auto TB_SetFormat = Fn<void (*)(void*, std::int32_t)>("nvnTextureBuilderSetFormat");
    auto TB_SetSize2D = Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnTextureBuilderSetSize2D");
    auto TB_SetSize3D = Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t)>("nvnTextureBuilderSetSize3D");
    auto TB_SetLevels = Fn<void (*)(void*, std::int32_t)>("nvnTextureBuilderSetLevels");
    auto TB_SetSwizzle = Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>("nvnTextureBuilderSetSwizzle");
    auto TB_GetAlignment = Fn<std::uint64_t (*)(const void*)>("nvnTextureBuilderGetStorageAlignment");
    auto TB_GetSize = Fn<std::uint64_t (*)(const void*)>("nvnTextureBuilderGetStorageSize");
    auto TB_SetStorage = Fn<void (*)(void*, const void*, std::int64_t)>("nvnTextureBuilderSetStorage");
    auto TextureInitialize = Fn<std::int32_t (*)(void*, const void*)>("nvnTextureInitialize");
    auto RegisterTexture = Fn<void (*)(void*, std::int32_t, const void*, const void*)>("nvnTexturePoolRegisterTexture");
    std::int64_t texture_offset = 0;
    std::int32_t next_texture_id = reserved_textures;
    Storage depth_target, render_targets[2], cube_target, unknown_target;
    auto make_target = [&](void* texture, std::int32_t flags, std::int32_t target, std::int32_t format, std::int32_t w,
                           std::int32_t h, std::int32_t depth3d, const std::int32_t* swizzle) -> std::int32_t {
        TB_SetDefaults(&texture_builder);
        TB_SetFlags(&texture_builder, flags);
        TB_SetTarget(&texture_builder, target);
        TB_SetFormat(&texture_builder, format);
        if (depth3d)
            TB_SetSize3D(&texture_builder, w, h, depth3d);
        else
            TB_SetSize2D(&texture_builder, w, h);
        TB_SetLevels(&texture_builder, 1);
        if (swizzle)
            TB_SetSwizzle(&texture_builder, swizzle[0], swizzle[1], swizzle[2], swizzle[3]);
        const std::uint64_t align = TB_GetAlignment(&texture_builder);
        const std::uint64_t size = TB_GetSize(&texture_builder);
        Check(align > 0 && size >= static_cast<std::uint64_t>(w) * static_cast<std::uint64_t>(h) * 4u, "texture storage size and alignment");
        texture_offset = static_cast<std::int64_t>((static_cast<std::uint64_t>(texture_offset) + align - 1) / align * align);
        TB_SetStorage(&texture_builder, &engine_pools[0], texture_offset);
        texture_offset += static_cast<std::int64_t>(size);
        if (!TextureInitialize(texture, &texture_builder))
            return -1;
        RegisterTexture(&texture_pool, next_texture_id, texture, nullptr);
        return next_texture_id++;
    };
    const std::int32_t depth_swizzle[4] = {2, 2, 2, 5}, identity[4] = {2, 3, 4, 5};
    const std::int32_t depth_id = make_target(&depth_target, 0xc, 1, 0x35, 1280, 720, 0, depth_swizzle);
    Check(depth_id >= 0, "the depth target (format 0x35)");
    std::int32_t color_ids[2];
    for (int i = 0; i < 2; ++i) {
        color_ids[i] = make_target(&render_targets[i], 0xd, 1, 0x25, 1280, 720, 0, identity);
        Check(color_ids[i] >= 0, "a colour target (format 0x25)");
    }
    const TextureInfo depth_info = GetTextureInfo(&depth_target);
    Check(depth_info.exists && depth_info.format == VK_FORMAT_D32_SFLOAT_S8_UINT && depth_info.width == 1280,
          "the depth target is a 1280x720 depth-stencil image");
    const TextureInfo color_info = GetTextureInfo(&render_targets[0]);
    Check(color_info.exists && color_info.format == VK_FORMAT_R8G8B8A8_UNORM && color_info.layers == 1,
          "the colour target is a 1280x720 RGBA8 image");
    // fn_710015f880: a cube target (target 8, SetSize3D(w, w, 1)).
    const std::int32_t cube_id = make_target(&cube_target, 0xd, 8, 0x25, 128, 128, 1, identity);
    const TextureInfo cube_info = GetTextureInfo(&cube_target);
    Check(cube_id >= 0 && cube_info.cube && cube_info.layers == 6, "a cube target has six layers");
    // A format the shim does not know fails (and is reported), rather than being guessed.
    Check(make_target(&unknown_target, 0xd, 1, 0x3f, 64, 64, 0, identity) < 0, "an unknown format fails");
    Check(RegisteredTexture(static_cast<std::uint32_t>(color_ids[1])) == &render_targets[1], "the pool knows the texture");

    // The binding layer's resolver over the shim's objects: a handle from nvnDeviceGetTextureHandle.
    {
        auto GetTextureHandle = Fn<TextureHandle (*)(const void*, std::int32_t, std::int32_t)>("nvnDeviceGetTextureHandle");
        const TextureHandle handle = GetTextureHandle(&device, color_ids[0], sampler_id);
        VkImageView view = VK_NULL_HANDLE;
        VkImageLayout layout{};
        VkSampler vk_sampler = VK_NULL_HANDLE;
        Check(ShimResolver().ResolveTexture(TextureId(handle), &view, &layout) && view == color_info.view,
              "a texture handle resolves to the texture's view");
        Check(ShimResolver().ResolveSampler(SamplerId(handle), &vk_sampler) && vk_sampler != VK_NULL_HANDLE,
              "and its sampler");
        Check(ShimResolver().PlaceholderImage().imageView != VK_NULL_HANDLE &&
                  ShimResolver().PlaceholderBuffer().buffer != VK_NULL_HANDLE,
              "the placeholders exist");
    }

    // The window: two textures (the game's render targets), interval 1, the first acquire.
    auto WindowBuilderSetDefaults = Fn<void (*)(void*)>("nvnWindowBuilderSetDefaults");
    auto WindowBuilderSetDevice = Fn<void (*)(void*, const void*)>("nvnWindowBuilderSetDevice");
    auto WindowBuilderSetNativeWindow = Fn<void (*)(void*, void*)>("nvnWindowBuilderSetNativeWindow");
    auto WindowBuilderSetTextures = Fn<void (*)(void*, std::int32_t, const void* const*)>("nvnWindowBuilderSetTextures");
    auto WindowInitialize = Fn<std::int32_t (*)(void*, const void*)>("nvnWindowInitialize");
    auto WindowSetPresentInterval = Fn<void (*)(void*, std::int32_t)>("nvnWindowSetPresentInterval");
    auto WindowAcquireTexture = Fn<std::int32_t (*)(void*, void*, std::int32_t*)>("nvnWindowAcquireTexture");
    auto QueueFenceSync = Fn<void (*)(void*, void*, std::int32_t, std::int32_t)>("nvnQueueFenceSync");
    auto QueueFlush = Fn<void (*)(void*)>("nvnQueueFlush");
    auto SyncWait = Fn<std::int32_t (*)(const void*, std::uint64_t)>("nvnSyncWait");
    const void* textures[2] = {&render_targets[0], &render_targets[1]};
    int native_window = 0;
    WindowBuilderSetDefaults(&window_builder_storage);
    WindowBuilderSetDevice(&window_builder_storage, &device);
    WindowBuilderSetNativeWindow(&window_builder_storage, &native_window);
    QueueFenceSync(&queue, &sync0, 0, 0);
    QueueFlush(&queue);
    SyncWait(&sync0, ~0ull);
    WindowBuilderSetTextures(&window_builder_storage, 2, textures);
    Check(WindowInitialize(&window, &window_builder_storage) == 1, "nvnWindowInitialize");
    WindowSetPresentInterval(&window, 1);
    std::int32_t index = -1;
    Check(WindowAcquireTexture(&window, &window_sync, &index) == 0 && index == 0, "the first nvnWindowAcquireTexture");
    auto CommandBufferSetViewport = Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>("nvnCommandBufferSetViewport");
    auto CommandBufferSetScissor = Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>("nvnCommandBufferSetScissor");
    CommandBufferSetViewport(&cmd_builders[0], 0, 0, 1280, 720);
    CommandBufferSetScissor(&cmd_builders[0], 0, 0, 1280, 720);

    // Three frames: fn_7100159920 (begin), fn_7100159ac0 (end, submit), fn_7100159bf0 (present).
    auto BeginRecording = Fn<void (*)(void*)>("nvnCommandBufferBeginRecording");
    auto EndRecording = Fn<std::uint64_t (*)(void*)>("nvnCommandBufferEndRecording");
    auto SetSampleMask = Fn<void (*)(void*, std::int32_t)>("nvnCommandBufferSetSampleMask");
    auto SubmitCommands = Fn<void (*)(void*, std::int32_t, const std::uint64_t*)>("nvnQueueSubmitCommands");
    auto PresentTexture = Fn<void (*)(void*, const void*, std::int32_t)>("nvnQueuePresentTexture");
    for (int frame = 0; frame < 3; ++frame) {
        void* cmd = &cmd_builders[frame & 1];
        CommandBufferAddCommandMemory(cmd, &pools[frame & 1], 0, kCommandPoolSize);
        CommandBufferAddControlMemory(cmd, control_memory[frame & 1], kControlSize);
        BeginRecording(cmd);
        SetSampleMask(cmd, -1);
        CommandBufferSetViewport(cmd, 0, 0, 1280, 720);
        CommandBufferSetScissor(cmd, 0, 0, 1280, 720);
        const std::uint64_t handle = EndRecording(cmd);
        Check(handle != 0, "nvnCommandBufferEndRecording gives a handle");
        SyncWait(&sync0, ~0ull);
        SyncWait(&window_sync, ~0ull);
        SubmitCommands(&queue, 1, &handle);
        QueueFenceSync(&queue, &sync0, 0, 0);
        QueueFlush(&queue);
        PresentTexture(&queue, &window, index);
        const std::int32_t previous = index;
        Check(WindowAcquireTexture(&window, &window_sync, &index) == 0 && index == (previous + 1) % 2,
              "nvnWindowAcquireTexture alternates the two textures");
    }
    const QueueStats qs = GetQueueStats(&queue);
    Check(qs.submits == 3 && qs.submitted_handles == 3 && qs.presents == 3 && qs.fences == 4,
          "the queue saw 3 submits, 3 presents and 4 fences");
    const CommandBufferStats cs = GetCommandBufferStats(&cmd_builders[0]);
    Check(cs.recordings == 2 && !cs.recording && cs.has_command_memory && cs.has_control_memory &&
              cs.has_memory_callback && cs.viewport[2] == 1280,
          "command buffer 0 recorded twice with its memory and viewport");
    const WindowStats ws = GetWindowStats(&window);
    Check(ws.textures == 2 && ws.acquires == 4 && ws.native_window == &native_window, "the window's textures and acquires");

    Check(TrapsCalled().empty(), "the start-up replay reaches no unimplemented NVN function");
    // Every name the game fetches resolves (trapped or not); a duplicate registration is a bug.
    Check(DuplicateRegistrations().empty(), "no NVN name registered twice");

    std::printf("nvn shim: %zu NVN functions implemented; the start-up replay reached %zu unimplemented ones%s\n",
                Registered().size(), TrapsCalled().size(), TrapsCalled().empty() ? "" : ", in order:");
    for (const std::string& name : TrapsCalled())
        std::printf("  %s\n", name.c_str());
    // The engine finalises the device with pools alive (the shutdown fn_7100132730); the shim must
    // release their Vulkan objects first (validation reports leaks at vkDestroyDevice). One pool
    // gets a Vulkan buffer first so that there is something to release.
    {
        auto MemoryPoolGetBufferAddress = Fn<BufferAddress (*)(const void*)>("nvnMemoryPoolGetBufferAddress");
        VkBuffer vk = VK_NULL_HANDLE;
        VkDeviceSize off = 0;
        Check(ResolveGpuAddress(MemoryPoolGetBufferAddress(&engine_pools[4]), 0x100, &vk, &off),
              "a 1 MB engine pool resolves");
    }
    Fn<void (*)(void*)>("nvnDeviceFinalize")(&device);
    Check(CurrentBackend() == nullptr, "nvnDeviceFinalize destroys the backend");
    Check(PoolsWithVulkanBuffer() == 0, "nvnDeviceFinalize released the pools' Vulkan buffers");
    auto MemoryPoolFinalize = Fn<void (*)(void*)>("nvnMemoryPoolFinalize");
    for (Storage& pool : pools)
        MemoryPoolFinalize(&pool);
    for (int i = 0; i < 5; ++i) {
        MemoryPoolFinalize(&engine_pools[i]);
        std::free(engine_storage[i]);
    }
    Check(MemoryPoolCount() == 0, "every memory pool finalised");
    const int errors = ValidationErrors();  // after the device is gone: leaks are reported there
    std::printf("%d failures, %d Vulkan validation errors\n", g_failures, errors);
    return g_failures || errors ? 1 : 0;
}
