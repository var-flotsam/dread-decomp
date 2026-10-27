// NVN textures, texture and sampler pools, samplers (include/nvn_shim/texture.h has the evidence for
// every number this file interprets).

#include "nvn_shim/texture.h"

#include <algorithm>
#include <cstdio>
#include <map>
#include <set>

#include <vulkan/vulkan.h>

#include "nvn_shim/device.h"
#include "nvn_shim/memory.h"
#include "nvn_shim/procs.h"
#include "objects.h"

namespace dread::nvn {

namespace {

const FormatInfo kFormats[] = {
    {0x01, VK_FORMAT_R8_UNORM, 1, 1, false, false, "MEDS XTX_Tegra_Format R8_UNORM"},
    {0x0d, VK_FORMAT_R8G8_UNORM, 2, 1, false, false, "MEDS XTX_Tegra_Format R8G8_UNORM"},
    {0x25, VK_FORMAT_R8G8B8A8_UNORM, 4, 1, false, false, "MEDS XTX_Tegra_Format R8G8B8A8_UNORM"},
    {0x42, VK_FORMAT_BC1_RGBA_UNORM_BLOCK, 8, 4, false, false, "MEDS XTX_Tegra_Format BC1_UNORM"},
    {0x44, VK_FORMAT_BC3_UNORM_BLOCK, 16, 4, false, false, "MEDS XTX_Tegra_Format BC3_UNORM"},
    {0x4b, VK_FORMAT_BC5_UNORM_BLOCK, 16, 4, false, false, "MEDS XTX_Tegra_Format BC5_UNORM"},
    {0x50, VK_FORMAT_BC6H_UFLOAT_BLOCK, 16, 4, false, false, "MEDS XTX_Tegra_Format BC6H_UF16"},
    {0x6d, VK_FORMAT_B8G8R8A8_UNORM, 4, 1, false, false, "MEDS XTX_Tegra_Format B8G8R8A8_UNORM"},
    {0x33, VK_FORMAT_D32_SFLOAT_S8_UINT, 8, 1, true, true, "depth by use (fn_71001604e0's R,R,R,A swizzle)"},
    {0x35, VK_FORMAT_D32_SFLOAT_S8_UINT, 8, 1, true, true, "depth by use (the depth target of fn_71001587f0)"},
};

constexpr std::int32_t kTarget2D = 1;
constexpr std::int32_t kTargetCube = 8;
constexpr std::uint64_t kStorageAlignment = 0x200;

struct TextureBuilder {
    const void* device = nullptr;
    std::int32_t flags = 0, target = kTarget2D, format = 0;
    std::uint32_t width = 1, height = 1, depth = 1, levels = 1;
    std::int32_t swizzle[4] = {2, 3, 4, 5};
    const void* pool = nullptr;
    std::int64_t offset = 0;
    std::int64_t stride = 0;
    const void* packaged_data = nullptr;
    const void* packaged_layout = nullptr;
};

struct Texture {
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    const FormatInfo* format = nullptr;
    std::uint32_t width = 0, height = 0, layers = 1, levels = 1;
    bool cube = false;
    const void* packaged_data = nullptr;  // not uploaded yet (header comment)
};

struct SamplerBuilder {
    const void* device = nullptr;
    std::int32_t min_filter = 0, mag_filter = 0;
    std::int32_t wrap[3] = {};
    float lod_min = 0.0f, lod_max = 1000.0f, lod_bias = 0.0f, max_anisotropy = 1.0f;
    float border[4] = {};
    std::int32_t reduction = 0, compare_mode = 0, compare_func = 0;
};

struct Sampler {
    SamplerBuilder settings;
};

struct PoolSampler {
    VkSampler sampler = VK_NULL_HANDLE;
};

detail::Registry<TextureBuilder> g_texture_builders;
detail::Registry<Texture> g_textures;
detail::Registry<SamplerBuilder> g_sampler_builders;
detail::Registry<Sampler> g_samplers;
std::map<std::uint32_t, const void*> g_texture_ids;  // texture pool id -> texture storage
std::map<std::uint32_t, PoolSampler> g_sampler_ids;   // sampler pool id -> Vulkan sampler
std::uint32_t g_texture_pool_size = 0, g_sampler_pool_size = 0;
std::set<std::int32_t> g_reported_formats, g_reported_targets;

// Placeholders for the binding layer: a 1x1 image, a 64 KB zero buffer, a sampler.
struct Placeholders {
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory image_memory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory buffer_memory = VK_NULL_HANDLE;
    VkSampler sampler = VK_NULL_HANDLE;
} g_placeholders;

std::uint64_t StorageSize(const TextureBuilder& b) {
    const FormatInfo* f = FindFormat(b.format);
    if (b.stride > 0)  // a linear texture (fn_710015fff0 sets a stride)
        return static_cast<std::uint64_t>(b.stride) * b.height;
    const std::uint64_t texel = f ? f->block_bytes : 16;
    const std::uint32_t block = f ? f->block_size : 1;
    const std::uint64_t layers = b.target == kTargetCube ? 6 : b.depth;
    std::uint64_t size = 0;
    for (std::uint32_t l = 0; l < std::max(b.levels, 1u); ++l) {
        const std::uint64_t w = std::max(1u, b.width >> l), h = std::max(1u, b.height >> l);
        size += (w + block - 1) / block * ((h + block - 1) / block) * texel * layers;
    }
    return (size + kStorageAlignment - 1) / kStorageAlignment * kStorageAlignment;
}

VkComponentSwizzle Swizzle(std::int32_t v) {
    switch (v) {
    case 0:
        return VK_COMPONENT_SWIZZLE_ZERO;
    case 1:
        return VK_COMPONENT_SWIZZLE_ONE;
    case 2:
        return VK_COMPONENT_SWIZZLE_R;
    case 3:
        return VK_COMPONENT_SWIZZLE_G;
    case 4:
        return VK_COMPONENT_SWIZZLE_B;
    case 5:
        return VK_COMPONENT_SWIZZLE_A;
    default:
        return VK_COMPONENT_SWIZZLE_IDENTITY;
    }
}

bool CreateImage(const Backend& be, VkFormat format, std::uint32_t w, std::uint32_t h, std::uint32_t layers,
                 std::uint32_t levels, bool cube, VkImageUsageFlags usage, VkImageAspectFlags aspect,
                 const VkComponentMapping& swizzle, VkImage* image, VkDeviceMemory* memory, VkImageView* view) {
    VkImageCreateInfo ici{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    ici.flags = cube ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
    ici.imageType = VK_IMAGE_TYPE_2D;
    ici.format = format;
    ici.extent = {w, h, 1};
    ici.mipLevels = levels;
    ici.arrayLayers = layers;
    ici.samples = VK_SAMPLE_COUNT_1_BIT;
    ici.tiling = VK_IMAGE_TILING_OPTIMAL;
    ici.usage = usage;
    if (vkCreateImage(be.device, &ici, nullptr, image) != VK_SUCCESS)
        return false;
    VkMemoryRequirements req;
    vkGetImageMemoryRequirements(be.device, *image, &req);
    VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mai.allocationSize = req.size;
    mai.memoryTypeIndex = be.MemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (mai.memoryTypeIndex == UINT32_MAX)
        mai.memoryTypeIndex = be.MemoryType(req.memoryTypeBits, 0);
    if (vkAllocateMemory(be.device, &mai, nullptr, memory) != VK_SUCCESS) {
        vkDestroyImage(be.device, *image, nullptr);
        *image = VK_NULL_HANDLE;
        return false;
    }
    vkBindImageMemory(be.device, *image, *memory, 0);
    VkImageViewCreateInfo vci{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    vci.image = *image;
    vci.viewType = cube ? VK_IMAGE_VIEW_TYPE_CUBE : (layers > 1 ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D);
    vci.format = format;
    vci.components = swizzle;
    vci.subresourceRange = {aspect, 0, levels, 0, layers};
    if (vkCreateImageView(be.device, &vci, nullptr, view) != VK_SUCCESS) {
        vkFreeMemory(be.device, *memory, nullptr);
        vkDestroyImage(be.device, *image, nullptr);
        *image = VK_NULL_HANDLE;
        *memory = VK_NULL_HANDLE;
        return false;
    }
    return true;
}

void DestroyTexture(Texture& t) {
    const Backend* be = CurrentBackend();
    if (be) {
        if (t.view)
            vkDestroyImageView(be->device, t.view, nullptr);
        if (t.image)
            vkDestroyImage(be->device, t.image, nullptr);
        if (t.memory)
            vkFreeMemory(be->device, t.memory, nullptr);
    }
    t.view = VK_NULL_HANDLE;
    t.image = VK_NULL_HANDLE;
    t.memory = VK_NULL_HANDLE;
}

VkSampler MakeSampler(const SamplerBuilder& s) {
    const Backend* be = CurrentBackend();
    if (!be)
        return VK_NULL_HANDLE;
    VkSamplerCreateInfo sci{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
    sci.magFilter = VK_FILTER_LINEAR;  // the game's filter numbers are not decoded (texture.h)
    sci.minFilter = VK_FILTER_LINEAR;
    sci.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sci.addressModeU = sci.addressModeV = sci.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sci.mipLodBias = s.lod_bias;
    sci.minLod = s.lod_min;
    sci.maxLod = std::max(s.lod_min, s.lod_max);
    const float max_aniso = be->properties.limits.maxSamplerAnisotropy;
    sci.anisotropyEnable = VK_FALSE;  // needs the samplerAnisotropy feature; kept for later
    sci.maxAnisotropy = std::min(std::max(s.max_anisotropy, 1.0f), max_aniso > 0 ? max_aniso : 1.0f);
    sci.compareEnable = s.compare_mode != 0 ? VK_TRUE : VK_FALSE;
    sci.compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;  // the game's compare numbers are not decoded
    sci.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    VkSampler out = VK_NULL_HANDLE;
    if (vkCreateSampler(be->device, &sci, nullptr, &out) != VK_SUCCESS)
        return VK_NULL_HANDLE;
    return out;
}

// --- texture builder and texture ---

void TextureBuilderSetDevice(void* b, const void* device) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).device = device;
}
void TextureBuilderSetDefaults(void* b) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_texture_builders.ResetKeepingDevice(b);
}
void TextureBuilderSetFlags(void* b, std::int32_t flags) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).flags = flags;
}
void TextureBuilderSetTarget(void* b, std::int32_t target) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).target = target;
}
void TextureBuilderSetFormat(void* b, std::int32_t format) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).format = format;
}
void TextureBuilderSetSize2D(void* b, std::int32_t w, std::int32_t h) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    TextureBuilder& t = g_texture_builders.Get(b);
    t.width = static_cast<std::uint32_t>(std::max(w, 1));
    t.height = static_cast<std::uint32_t>(std::max(h, 1));
    t.depth = 1;
}
void TextureBuilderSetSize3D(void* b, std::int32_t w, std::int32_t h, std::int32_t d) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    TextureBuilder& t = g_texture_builders.Get(b);
    t.width = static_cast<std::uint32_t>(std::max(w, 1));
    t.height = static_cast<std::uint32_t>(std::max(h, 1));
    t.depth = static_cast<std::uint32_t>(std::max(d, 1));
}
void TextureBuilderSetLevels(void* b, std::int32_t levels) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).levels = static_cast<std::uint32_t>(std::max(levels, 1));
}
void TextureBuilderSetSwizzle(void* b, std::int32_t r, std::int32_t g, std::int32_t bl, std::int32_t a) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    TextureBuilder& t = g_texture_builders.Get(b);
    t.swizzle[0] = r;
    t.swizzle[1] = g;
    t.swizzle[2] = bl;
    t.swizzle[3] = a;
}
void TextureBuilderSetStorage(void* b, const void* pool, std::int64_t offset) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    TextureBuilder& t = g_texture_builders.Get(b);
    t.pool = pool;
    t.offset = offset;
}
void TextureBuilderSetStride(void* b, std::int64_t stride) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).stride = stride;
}
void TextureBuilderSetPackagedTextureData(void* b, const void* data) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).packaged_data = data;
}
void TextureBuilderSetPackagedTextureLayout(void* b, const void* layout) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).packaged_layout = layout;
}
std::uint64_t TextureBuilderGetStorageSize(const void* b) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    TextureBuilder* t = g_texture_builders.Find(b);
    return t ? StorageSize(*t) : 0;
}
std::uint64_t TextureBuilderGetStorageAlignment(const void*) {
    return kStorageAlignment;
}

std::int32_t TextureInitialize(void* texture, const void* builder) {
    const Backend* be = CurrentBackend();
    std::lock_guard<std::mutex> lock(detail::StateLock());
    TextureBuilder* b = g_texture_builders.Find(builder);
    if (!b || !be)
        return 0;
    const FormatInfo* f = FindFormat(b->format);
    if (!f) {
        if (g_reported_formats.insert(b->format).second)
            std::fprintf(stderr, "nvn shim: texture format %#x is not known (texture.h); nvnTextureInitialize fails\n",
                         static_cast<unsigned>(b->format));
        return 0;
    }
    if (b->target != kTarget2D && b->target != kTargetCube) {
        if (g_reported_targets.insert(b->target).second)
            std::fprintf(stderr, "nvn shim: texture target %d is not known (texture.h); nvnTextureInitialize fails\n",
                         b->target);
        return 0;
    }
    if (Texture* old = g_textures.Find(texture))
        DestroyTexture(*old);
    Texture& t = g_textures.Reset(texture);
    t.format = f;
    t.width = b->width;
    t.height = b->height;
    t.cube = b->target == kTargetCube;
    t.layers = t.cube ? 6 : b->depth;
    t.levels = b->levels;
    t.packaged_data = b->packaged_data;
    const bool compressed = f->block_size > 1;
    VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    if (f->depth)
        usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    else if (!compressed)
        usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    const VkImageAspectFlags aspect = f->depth ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
    const VkComponentMapping swizzle = f->depth ? VkComponentMapping{}
                                                : VkComponentMapping{Swizzle(b->swizzle[0]), Swizzle(b->swizzle[1]),
                                                                     Swizzle(b->swizzle[2]), Swizzle(b->swizzle[3])};
    if (!CreateImage(*be, f->vk, t.width, t.height, t.layers, t.levels, t.cube, usage, aspect, swizzle, &t.image,
                     &t.memory, &t.view)) {
        std::fprintf(stderr, "nvn shim: no Vulkan image for a %ux%u texture of format %#x\n", t.width, t.height,
                     static_cast<unsigned>(b->format));
        g_textures.Erase(texture);
        return 0;
    }
    return 1;
}

void TextureFinalize(void* texture) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    if (Texture* t = g_textures.Find(texture)) {
        DestroyTexture(*t);
        g_textures.Erase(texture);
    }
}

void TextureSetDebugLabel(void*, const char*) {}

// --- pools ---

// The pools' memory is the game's (a memory pool, an offset, the descriptor count it sized with
// nvnDeviceGetInteger 0xf / 0x10); the shim keeps the descriptors itself.
std::int32_t TexturePoolInitialize(void*, const void*, std::int64_t, std::int32_t count) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_texture_pool_size = static_cast<std::uint32_t>(std::max(count, 0));
    g_texture_ids.clear();
    return 1;
}

void TexturePoolRegisterTexture(void*, std::int32_t id, const void* texture, const void*) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    if (id < 0 || static_cast<std::uint32_t>(id) >= g_texture_pool_size) {
        std::fprintf(stderr, "nvn shim: texture id %d outside the texture pool (%u)\n", id, g_texture_pool_size);
        return;
    }
    g_texture_ids[static_cast<std::uint32_t>(id)] = texture;
}

std::int32_t SamplerPoolInitialize(void*, const void*, std::int64_t, std::int32_t count) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_sampler_pool_size = static_cast<std::uint32_t>(std::max(count, 0));
    return 1;
}

void SamplerPoolSetDebugLabel(void*, const char*) {}
void TexturePoolSetDebugLabel(void*, const char*) {}

// fn_710015e9fc initialises a sampler on its stack, registers it and finalises it at once: the pool
// entry keeps its own Vulkan sampler, made from the sampler's settings here.
void SamplerPoolRegisterSampler(void*, std::int32_t id, const void* sampler) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    Sampler* s = g_samplers.Find(sampler);
    if (!s || id < 0 || static_cast<std::uint32_t>(id) >= g_sampler_pool_size) {
        std::fprintf(stderr, "nvn shim: nvnSamplerPoolRegisterSampler(%d) without a sampler or outside the pool\n", id);
        return;
    }
    PoolSampler& slot = g_sampler_ids[static_cast<std::uint32_t>(id)];
    if (slot.sampler) {
        if (const Backend* be = CurrentBackend())
            vkDestroySampler(be->device, slot.sampler, nullptr);
    }
    slot.sampler = MakeSampler(s->settings);
}

// --- samplers ---

void SamplerBuilderSetDevice(void* b, const void* device) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_sampler_builders.Get(b).device = device;
}
void SamplerBuilderSetDefaults(void* b) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_sampler_builders.ResetKeepingDevice(b);
}
void SamplerBuilderSetMinMagFilter(void* b, std::int32_t min, std::int32_t mag) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    SamplerBuilder& s = g_sampler_builders.Get(b);
    s.min_filter = min;
    s.mag_filter = mag;
}
void SamplerBuilderSetWrapMode(void* b, std::int32_t s_, std::int32_t t, std::int32_t r) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    SamplerBuilder& s = g_sampler_builders.Get(b);
    s.wrap[0] = s_;
    s.wrap[1] = t;
    s.wrap[2] = r;
}
void SamplerBuilderSetLodClamp(void* b, float min, float max) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    SamplerBuilder& s = g_sampler_builders.Get(b);
    s.lod_min = min;
    s.lod_max = max;
}
void SamplerBuilderSetLodBias(void* b, float bias) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_sampler_builders.Get(b).lod_bias = bias;
}
void SamplerBuilderSetBorderColor(void* b, const float* color) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    SamplerBuilder& s = g_sampler_builders.Get(b);
    for (int i = 0; i < 4 && color; ++i)
        s.border[i] = color[i];
}
void SamplerBuilderSetMaxAnisotropy(void* b, float aniso) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_sampler_builders.Get(b).max_anisotropy = aniso;
}
void SamplerBuilderSetReductionFilter(void* b, std::int32_t reduction) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_sampler_builders.Get(b).reduction = reduction;
}
void SamplerBuilderSetCompare(void* b, std::int32_t mode, std::int32_t func) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    SamplerBuilder& s = g_sampler_builders.Get(b);
    s.compare_mode = mode;
    s.compare_func = func;
}

std::int32_t SamplerInitialize(void* sampler, const void* builder) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    SamplerBuilder* b = g_sampler_builders.Find(builder);
    if (!b)
        return 0;
    g_samplers.Reset(sampler).settings = *b;
    return 1;
}

void SamplerFinalize(void* sampler) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_samplers.Erase(sampler);
}

TextureHandle DeviceGetTextureHandle(const void*, std::int32_t texture_id, std::int32_t sampler_id) {
    return MakeTextureHandle(static_cast<std::uint32_t>(texture_id), static_cast<std::uint32_t>(sampler_id));
}

// --- the binding layer's resolver ---

class Resolver final : public ResourceResolver {
public:
    bool ResolveBuffer(BufferAddress address, std::uint64_t size, VkBuffer* buffer, VkDeviceSize* offset) override {
        return ResolveGpuAddress(address, size, buffer, offset);
    }
    bool ResolveTexture(std::uint32_t id, VkImageView* view, VkImageLayout* layout) override {
        std::lock_guard<std::mutex> lock(detail::StateLock());
        auto it = g_texture_ids.find(id);
        Texture* t = it == g_texture_ids.end() ? nullptr : g_textures.Find(it->second);
        if (!t || !t->view)
            return false;
        *view = t->view;
        *layout = t->format->depth ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        return true;
    }
    bool ResolveSampler(std::uint32_t id, VkSampler* sampler) override {
        std::lock_guard<std::mutex> lock(detail::StateLock());
        auto it = g_sampler_ids.find(id);
        if (it == g_sampler_ids.end() || !it->second.sampler)
            return false;
        *sampler = it->second.sampler;
        return true;
    }
    VkDescriptorBufferInfo PlaceholderBuffer() override {
        EnsurePlaceholders();
        return {g_placeholders.buffer, 0, VK_WHOLE_SIZE};
    }
    VkDescriptorImageInfo PlaceholderImage() override {
        EnsurePlaceholders();
        return {g_placeholders.sampler, g_placeholders.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
    }

private:
    static void EnsurePlaceholders() {
        std::lock_guard<std::mutex> lock(detail::StateLock());
        const Backend* be = CurrentBackend();
        if (!be || g_placeholders.view)
            return;
        // 64 KB: at least the largest uniform block of the game's shaders (the manifest's sizes; the
        // binding layer's PlaceholderBuffer contract).
        VkBufferCreateInfo bci{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bci.size = 0x10000;
        bci.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        if (vkCreateBuffer(be->device, &bci, nullptr, &g_placeholders.buffer) == VK_SUCCESS) {
            VkMemoryRequirements req;
            vkGetBufferMemoryRequirements(be->device, g_placeholders.buffer, &req);
            VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
            mai.allocationSize = req.size;
            mai.memoryTypeIndex = be->MemoryType(req.memoryTypeBits, 0);
            if (vkAllocateMemory(be->device, &mai, nullptr, &g_placeholders.buffer_memory) == VK_SUCCESS)
                vkBindBufferMemory(be->device, g_placeholders.buffer, g_placeholders.buffer_memory, 0);
        }
        CreateImage(*be, VK_FORMAT_R8G8B8A8_UNORM, 1, 1, 1, 1, false, VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT,
                    VkComponentMapping{}, &g_placeholders.image, &g_placeholders.image_memory, &g_placeholders.view);
        g_placeholders.sampler = MakeSampler(SamplerBuilder{});
    }
};

Resolver g_resolver;

}  // namespace

const FormatInfo* FindFormat(std::int32_t nvn) {
    for (const FormatInfo& f : kFormats)
        if (f.nvn == nvn)
            return &f;
    return nullptr;
}

ResourceResolver& ShimResolver() {
    return g_resolver;
}

TextureInfo GetTextureInfo(const void* texture) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    TextureInfo out;
    if (Texture* t = g_textures.Find(texture)) {
        out.exists = true;
        out.image = t->image;
        out.view = t->view;
        out.format = t->format ? t->format->vk : VK_FORMAT_UNDEFINED;
        out.width = t->width;
        out.height = t->height;
        out.layers = t->layers;
        out.levels = t->levels;
        out.cube = t->cube;
    }
    return out;
}

const void* RegisteredTexture(std::uint32_t id) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    auto it = g_texture_ids.find(id);
    return it == g_texture_ids.end() ? nullptr : it->second;
}

bool RegisteredSampler(std::uint32_t id) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    auto it = g_sampler_ids.find(id);
    return it != g_sampler_ids.end() && it->second.sampler != VK_NULL_HANDLE;
}

void detail::ReleaseTextureVulkanObjects() {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    const Backend* be = CurrentBackend();
    g_textures.ForEach([](const void*, Texture& t) { DestroyTexture(t); });
    for (auto& kv : g_sampler_ids) {
        if (be && kv.second.sampler)
            vkDestroySampler(be->device, kv.second.sampler, nullptr);
        kv.second.sampler = VK_NULL_HANDLE;
    }
    if (be) {
        if (g_placeholders.view)
            vkDestroyImageView(be->device, g_placeholders.view, nullptr);
        if (g_placeholders.image)
            vkDestroyImage(be->device, g_placeholders.image, nullptr);
        if (g_placeholders.image_memory)
            vkFreeMemory(be->device, g_placeholders.image_memory, nullptr);
        if (g_placeholders.buffer)
            vkDestroyBuffer(be->device, g_placeholders.buffer, nullptr);
        if (g_placeholders.buffer_memory)
            vkFreeMemory(be->device, g_placeholders.buffer_memory, nullptr);
        if (g_placeholders.sampler)
            vkDestroySampler(be->device, g_placeholders.sampler, nullptr);
    }
    g_placeholders = Placeholders{};
}

void detail::RegisterTextureProcs() {
    Register("nvnTextureBuilderSetDevice", &TextureBuilderSetDevice);
    Register("nvnTextureBuilderSetDefaults", &TextureBuilderSetDefaults);
    Register("nvnTextureBuilderSetFlags", &TextureBuilderSetFlags);
    Register("nvnTextureBuilderSetTarget", &TextureBuilderSetTarget);
    Register("nvnTextureBuilderSetFormat", &TextureBuilderSetFormat);
    Register("nvnTextureBuilderSetSize2D", &TextureBuilderSetSize2D);
    Register("nvnTextureBuilderSetSize3D", &TextureBuilderSetSize3D);
    Register("nvnTextureBuilderSetLevels", &TextureBuilderSetLevels);
    Register("nvnTextureBuilderSetSwizzle", &TextureBuilderSetSwizzle);
    Register("nvnTextureBuilderSetStorage", &TextureBuilderSetStorage);
    Register("nvnTextureBuilderSetStride", &TextureBuilderSetStride);
    Register("nvnTextureBuilderSetPackagedTextureData", &TextureBuilderSetPackagedTextureData);
    Register("nvnTextureBuilderSetPackagedTextureLayout", &TextureBuilderSetPackagedTextureLayout);
    Register("nvnTextureBuilderGetStorageSize", &TextureBuilderGetStorageSize);
    Register("nvnTextureBuilderGetStorageAlignment", &TextureBuilderGetStorageAlignment);
    Register("nvnTextureInitialize", &TextureInitialize);
    Register("nvnTextureFinalize", &TextureFinalize);
    Register("nvnTextureSetDebugLabel", &TextureSetDebugLabel);
    Register("nvnTexturePoolInitialize", &TexturePoolInitialize);
    Register("nvnTexturePoolRegisterTexture", &TexturePoolRegisterTexture);
    Register("nvnTexturePoolSetDebugLabel", &TexturePoolSetDebugLabel);
    Register("nvnSamplerPoolInitialize", &SamplerPoolInitialize);
    Register("nvnSamplerPoolRegisterSampler", &SamplerPoolRegisterSampler);
    Register("nvnSamplerPoolSetDebugLabel", &SamplerPoolSetDebugLabel);
    Register("nvnSamplerBuilderSetDevice", &SamplerBuilderSetDevice);
    Register("nvnSamplerBuilderSetDefaults", &SamplerBuilderSetDefaults);
    Register("nvnSamplerBuilderSetMinMagFilter", &SamplerBuilderSetMinMagFilter);
    Register("nvnSamplerBuilderSetWrapMode", &SamplerBuilderSetWrapMode);
    Register("nvnSamplerBuilderSetLodClamp", &SamplerBuilderSetLodClamp);
    Register("nvnSamplerBuilderSetLodBias", &SamplerBuilderSetLodBias);
    Register("nvnSamplerBuilderSetBorderColor", &SamplerBuilderSetBorderColor);
    Register("nvnSamplerBuilderSetMaxAnisotropy", &SamplerBuilderSetMaxAnisotropy);
    Register("nvnSamplerBuilderSetReductionFilter", &SamplerBuilderSetReductionFilter);
    Register("nvnSamplerBuilderSetCompare", &SamplerBuilderSetCompare);
    Register("nvnSamplerInitialize", &SamplerInitialize);
    Register("nvnSamplerFinalize", &SamplerFinalize);
    Register("nvnDeviceGetTextureHandle", &DeviceGetTextureHandle);
}

}  // namespace dread::nvn
