// NVN textures, texture and sampler pools, samplers (include/nvn_shim/texture.h has the evidence for
// every number this file interprets).

#include "nvn_shim/texture.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <map>
#include <set>

#include <vulkan/vulkan.h>

#include "nvn_shim/device.h"
#include "nvn_shim/memory.h"
#include "nvn_shim/procs.h"
#include "objects.h"
#include "texture_internal.h"

namespace dread::nvn {

namespace {

// The engine's converter fn_7100161410 turns a texture header's (type, sRGB flag, channel layout)
// into these numbers; the layouts are its reflected enum EComponents (fn_710016468c registers the
// names). "MEDS" also: mercury-engine-data-structures' XTX_Tegra_Format names the number a .bctex
// header carries, which equals the converter's output for every linear file.
const FormatInfo kFormats[] = {
    {0x01, VK_FORMAT_R8_UNORM, 1, 1, false, false, "converter: L8, A8, L4, A4; MEDS R8_UNORM"},
    {0x0a, VK_FORMAT_R32_SFLOAT, 4, 1, false, false, "converter: type 2, E_COMPONENTS_R32F"},
    {0x0d, VK_FORMAT_R8G8_UNORM, 2, 1, false, false, "converter: L8_A8, U8_V8, L4_A4; MEDS R8G8_UNORM"},
    {0x12, VK_FORMAT_R16G16_UNORM, 4, 1, false, true, "converter: E_COMPONENTS_U16_V16 (signedness not known)"},
    {0x25, VK_FORMAT_R8G8B8A8_UNORM, 4, 1, false, false, "converter: R8_G8_B8_A8; MEDS R8G8B8A8_UNORM"},
    {0x37, VK_FORMAT_B8G8R8A8_SRGB, 4, 1, false, false, "converter: 0x6d's layout with the sRGB flag"},
    {0x38, VK_FORMAT_R8G8B8A8_SRGB, 4, 1, false, false, "converter: 0x25's layout with the sRGB flag"},
    {0x3b, VK_FORMAT_R5G5B5A1_UNORM_PACK16, 2, 1, false, true, "converter: E_COMPONENTS_R5_G5_B5_A1 (bit order not known)"},
    {0x3c, VK_FORMAT_R5G6B5_UNORM_PACK16, 2, 1, false, true, "converter: E_COMPONENTS_R5_G6_B5 (bit order not known)"},
    {0x3f, VK_FORMAT_R16G16B16A16_SFLOAT, 8, 1, false, true,
     "a colour render target by use (fn_71001587f0 registers a fast-clear colour for it); layout not known"},
    {0x42, VK_FORMAT_BC1_RGBA_UNORM_BLOCK, 8, 4, false, false, "converter: block, R8_G8_B8; MEDS BC1_UNORM"},
    {0x44, VK_FORMAT_BC3_UNORM_BLOCK, 16, 4, false, false, "converter: block, R8_G8_B8_A8; MEDS BC3_UNORM"},
    {0x46, VK_FORMAT_BC1_RGBA_SRGB_BLOCK, 8, 4, false, false, "converter: 0x42's layout with the sRGB flag"},
    {0x48, VK_FORMAT_BC3_SRGB_BLOCK, 16, 4, false, false, "converter: 0x44's layout with the sRGB flag"},
    {0x4b, VK_FORMAT_BC5_UNORM_BLOCK, 16, 4, false, false, "converter: block default; MEDS BC5_UNORM"},
    {0x50, VK_FORMAT_BC6H_UFLOAT_BLOCK, 16, 4, false, false, "converter: block, layout 0xf; MEDS BC6H_UF16"},
    {0x6d, VK_FORMAT_B8G8R8A8_UNORM, 4, 1, false, false, "converter: R8_G8_B8, R8_G8_B8_X8; MEDS B8G8R8A8_UNORM"},
    {0x33, VK_FORMAT_D32_SFLOAT_S8_UINT, 8, 1, true, true, "depth by use (fn_71001604e0's R,R,R,A swizzle)"},
    {0x35, VK_FORMAT_D32_SFLOAT_S8_UINT, 8, 1, true, true, "depth by use (the depth target of fn_71001587f0)"},
};

constexpr std::int32_t kTarget2D = 1;
constexpr std::int32_t kTargetCube = 8;
constexpr std::uint64_t kStorageAlignment = 0x200;
constexpr VkDeviceSize kPlaceholderBufferSize = 0x4000;

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
    VkImageView attachment_view = VK_NULL_HANDLE;  // made when the texture is first a render target
    bool attachment = false;                       // made with an attachment usage
    const FormatInfo* format = nullptr;
    VkFormat vk_format = VK_FORMAT_UNDEFINED;  // the format made (a depth format may fall back)
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

// Placeholders for the binding layer, zeroed: a 16 KB buffer, and a 1x1 image of each kind a
// shader samples (texture_internal.h: a 2D colour image, a cube, a 2D depth image for shadow
// samplers) with a sampler (a comparing one for the depth image).
struct PlaceholderView {
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
};
struct Placeholders {
    PlaceholderView images[3];  // by detail::ViewKind
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory buffer_memory = VK_NULL_HANDLE;
    VkSampler sampler = VK_NULL_HANDLE, compare_sampler = VK_NULL_HANDLE;
    bool made = false;
} g_placeholders;
std::map<VkImageView, detail::ViewKind> g_view_kinds;  // every sampled view the resolver hands out

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

// A new image into the one layout every image keeps (texture_internal.h), zeroed unless compressed
// (no clears for those): NVN's texture memory is the game's, which a new render target finds as it
// was; zero is what the shim can give.
void InitializeImage(VkImage image, VkImageAspectFlags aspect, std::uint32_t levels, std::uint32_t layers, bool depth,
                     bool compressed) {
    ImmediateSubmit([&](VkCommandBuffer cmd) {
        VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = detail::kImageLayout;
        barrier.srcQueueFamilyIndex = barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange = {aspect, 0, levels, 0, layers};
        vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
                             nullptr, 1, &barrier);
        if (depth) {
            const VkClearDepthStencilValue zero{0.0f, 0};
            vkCmdClearDepthStencilImage(cmd, image, detail::kImageLayout, &zero, 1, &barrier.subresourceRange);
        } else if (!compressed) {
            const VkClearColorValue zero{};
            vkCmdClearColorImage(cmd, image, detail::kImageLayout, &zero, 1, &barrier.subresourceRange);
        }
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
        barrier.oldLayout = detail::kImageLayout;
        vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0,
                             nullptr, 1, &barrier);
    });
}

// The GPU may still use a texture the game finalises: wait for everything submitted (the game
// finalises textures when it unloads a scenario, not per frame).
void DestroyTexture(Texture& t) {
    const Backend* be = CurrentBackend();
    if (be && t.image)
        WaitFor(LastSubmitted(), UINT64_MAX);
    g_view_kinds.erase(t.view);
    if (be) {
        if (t.attachment_view)
            vkDestroyImageView(be->device, t.attachment_view, nullptr);
        if (t.view)
            vkDestroyImageView(be->device, t.view, nullptr);
        if (t.image)
            vkDestroyImage(be->device, t.image, nullptr);
        if (t.memory)
            vkFreeMemory(be->device, t.memory, nullptr);
    }
    t.view = VK_NULL_HANDLE;
    t.attachment_view = VK_NULL_HANDLE;
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
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).device = device;
}
void TextureBuilderSetDefaults(void* b) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_texture_builders.ResetKeepingDevice(b);
}
void TextureBuilderSetFlags(void* b, std::int32_t flags) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).flags = flags;
}
void TextureBuilderSetTarget(void* b, std::int32_t target) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).target = target;
}
void TextureBuilderSetFormat(void* b, std::int32_t format) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).format = format;
}
void TextureBuilderSetSize2D(void* b, std::int32_t w, std::int32_t h) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    TextureBuilder& t = g_texture_builders.Get(b);
    t.width = static_cast<std::uint32_t>(std::max(w, 1));
    t.height = static_cast<std::uint32_t>(std::max(h, 1));
    t.depth = 1;
}
void TextureBuilderSetSize3D(void* b, std::int32_t w, std::int32_t h, std::int32_t d) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    TextureBuilder& t = g_texture_builders.Get(b);
    t.width = static_cast<std::uint32_t>(std::max(w, 1));
    t.height = static_cast<std::uint32_t>(std::max(h, 1));
    t.depth = static_cast<std::uint32_t>(std::max(d, 1));
}
void TextureBuilderSetLevels(void* b, std::int32_t levels) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).levels = static_cast<std::uint32_t>(std::max(levels, 1));
}
void TextureBuilderSetSwizzle(void* b, std::int32_t r, std::int32_t g, std::int32_t bl, std::int32_t a) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    TextureBuilder& t = g_texture_builders.Get(b);
    t.swizzle[0] = r;
    t.swizzle[1] = g;
    t.swizzle[2] = bl;
    t.swizzle[3] = a;
}
void TextureBuilderSetStorage(void* b, const void* pool, std::int64_t offset) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    TextureBuilder& t = g_texture_builders.Get(b);
    t.pool = pool;
    t.offset = offset;
}
void TextureBuilderSetStride(void* b, std::int64_t stride) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).stride = stride;
}
void TextureBuilderSetPackagedTextureData(void* b, const void* data) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).packaged_data = data;
}
void TextureBuilderSetPackagedTextureLayout(void* b, const void* layout) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_texture_builders.Get(b).packaged_layout = layout;
}
std::uint64_t TextureBuilderGetStorageSize(const void* b) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    TextureBuilder* t = g_texture_builders.Find(b);
    return t ? StorageSize(*t) : 0;
}
std::uint64_t TextureBuilderGetStorageAlignment(const void*) {
    return kStorageAlignment;
}

std::int32_t TextureInitialize(void* texture, const void* builder) {
    const Backend* be = CurrentBackend();
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
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
    VkFormatFeatureFlags needed = VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
    if (f->depth) {
        usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        needed |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
    } else if (!compressed) {
        usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        if (FormatSupported(f->vk, VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT))
            needed |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
        else
            usage &= ~static_cast<VkImageUsageFlags>(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);  // a texture only
    }
    VkFormat vk_format = f->vk;
    if (f->depth && !FormatSupported(vk_format, needed))
        vk_format = VK_FORMAT_D24_UNORM_S8_UINT;  // the other depth-stencil format devices have
    if (!FormatSupported(vk_format, needed)) {
        if (g_reported_formats.insert(b->format).second)
            std::fprintf(stderr, "nvn shim: the device cannot use texture format %#x (Vulkan %d); nvnTextureInitialize fails\n",
                         static_cast<unsigned>(b->format), static_cast<int>(vk_format));
        g_textures.Erase(texture);
        return 0;
    }
    const VkImageAspectFlags aspect = f->depth ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
    // The game's swizzle, depth included (fn_71001604e0 gives depth R,R,R,A; Vulkan applies a view's
    // swizzle to a sampled depth aspect as well).
    const VkComponentMapping swizzle{Swizzle(b->swizzle[0]), Swizzle(b->swizzle[1]), Swizzle(b->swizzle[2]),
                                     Swizzle(b->swizzle[3])};
    t.vk_format = vk_format;
    t.attachment = (usage & (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)) != 0;
    if (!CreateImage(*be, vk_format, t.width, t.height, t.layers, t.levels, t.cube, usage, aspect, swizzle, &t.image,
                     &t.memory, &t.view)) {
        std::fprintf(stderr, "nvn shim: no Vulkan image for a %ux%u texture of format %#x\n", t.width, t.height,
                     static_cast<unsigned>(b->format));
        g_textures.Erase(texture);
        return 0;
    }
    InitializeImage(t.image, aspect | (f->depth ? VK_IMAGE_ASPECT_STENCIL_BIT : 0), t.levels, t.layers, f->depth, compressed);
    g_view_kinds[t.view] = t.cube ? detail::ViewKind::Cube : f->depth ? detail::ViewKind::Depth2D : detail::ViewKind::Color2D;
    return 1;
}

// The region the game passes: x, y, z, width, height, depth (fn_7100160e80 writes one texel at
// (x, y) with a region of (x, y, 0, 1, 1, 1); fn_7100160ef0 a whole level 0 with (0, 0, 0, w, h, 1)).
// The data is packed rows of the texture's format. A layered texture takes z and depth as layers.
struct CopyRegion {
    std::int32_t x, y, z, width, height, depth;
};

void TextureWriteTexels(const void* texture, const void*, const CopyRegion* region, const void* data) {
    const Backend* be = CurrentBackend();
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    Texture* t = g_textures.Find(texture);
    if (!be || !t || !t->image || !region || !data || region->width <= 0 || region->height <= 0 || region->depth <= 0 ||
        region->x < 0 || region->y < 0 || region->z < 0)
        return;
    const std::uint32_t bs = t->format->block_size;
    const bool layered = t->layers > 1;
    const std::uint32_t w = static_cast<std::uint32_t>(region->width), h = static_cast<std::uint32_t>(region->height),
                        d = static_cast<std::uint32_t>(region->depth);
    if (region->x + w > t->width || region->y + h > t->height ||
        static_cast<std::uint32_t>(region->z) + d > (layered ? t->layers : 1u)) {
        std::fprintf(stderr, "nvn shim: nvnTextureWriteTexels outside the texture\n");
        return;
    }
    const VkDeviceSize bytes =
        static_cast<VkDeviceSize>((w + bs - 1) / bs) * ((h + bs - 1) / bs) * d * t->format->block_bytes;
    VkBuffer staging = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkBufferCreateInfo bci{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    bci.size = bytes;
    bci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    if (vkCreateBuffer(be->device, &bci, nullptr, &staging) != VK_SUCCESS)
        return;
    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(be->device, staging, &req);
    VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mai.allocationSize = req.size;
    mai.memoryTypeIndex =
        be->MemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* mapped = nullptr;
    if (mai.memoryTypeIndex != UINT32_MAX && vkAllocateMemory(be->device, &mai, nullptr, &memory) == VK_SUCCESS &&
        vkBindBufferMemory(be->device, staging, memory, 0) == VK_SUCCESS &&
        vkMapMemory(be->device, memory, 0, VK_WHOLE_SIZE, 0, &mapped) == VK_SUCCESS) {
        std::memcpy(mapped, data, static_cast<std::size_t>(bytes));
        const VkImageAspectFlags aspect = t->format->depth ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
        const VkImage image = t->image;
        // After everything submitted before it (the barrier's first scope is all earlier work on
        // the queue), as NVN's CPU write is after the game waited for the GPU.
        ImmediateSubmit([&](VkCommandBuffer cmd) {
            VkMemoryBarrier mb{VK_STRUCTURE_TYPE_MEMORY_BARRIER};
            mb.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
            mb.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 1, &mb, 0,
                                 nullptr, 0, nullptr);
            VkBufferImageCopy copy{};
            copy.imageSubresource = {aspect, 0, layered ? static_cast<std::uint32_t>(region->z) : 0u, layered ? d : 1u};
            copy.imageOffset = {region->x, region->y, 0};
            copy.imageExtent = {w, h, 1};
            vkCmdCopyBufferToImage(cmd, staging, image, detail::kImageLayout, 1, &copy);
            mb.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            mb.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 1, &mb, 0,
                                 nullptr, 0, nullptr);
        });
    }
    if (memory)
        vkFreeMemory(be->device, memory, nullptr);
    vkDestroyBuffer(be->device, staging, nullptr);
}

void TextureFinalize(void* texture) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
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
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_texture_pool_size = static_cast<std::uint32_t>(std::max(count, 0));
    g_texture_ids.clear();
    return 1;
}

void TexturePoolRegisterTexture(void*, std::int32_t id, const void* texture, const void*) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (id < 0 || static_cast<std::uint32_t>(id) >= g_texture_pool_size) {
        std::fprintf(stderr, "nvn shim: texture id %d outside the texture pool (%u)\n", id, g_texture_pool_size);
        return;
    }
    g_texture_ids[static_cast<std::uint32_t>(id)] = texture;
}

std::int32_t SamplerPoolInitialize(void*, const void*, std::int64_t, std::int32_t count) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_sampler_pool_size = static_cast<std::uint32_t>(std::max(count, 0));
    return 1;
}

void SamplerPoolSetDebugLabel(void*, const char*) {}
void TexturePoolSetDebugLabel(void*, const char*) {}

// fn_710015e9fc initialises a sampler on its stack, registers it and finalises it at once: the pool
// entry keeps its own Vulkan sampler, made from the sampler's settings here.
void SamplerPoolRegisterSampler(void*, std::int32_t id, const void* sampler) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
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
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_sampler_builders.Get(b).device = device;
}
void SamplerBuilderSetDefaults(void* b) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_sampler_builders.ResetKeepingDevice(b);
}
void SamplerBuilderSetMinMagFilter(void* b, std::int32_t min, std::int32_t mag) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    SamplerBuilder& s = g_sampler_builders.Get(b);
    s.min_filter = min;
    s.mag_filter = mag;
}
void SamplerBuilderSetWrapMode(void* b, std::int32_t s_, std::int32_t t, std::int32_t r) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    SamplerBuilder& s = g_sampler_builders.Get(b);
    s.wrap[0] = s_;
    s.wrap[1] = t;
    s.wrap[2] = r;
}
void SamplerBuilderSetLodClamp(void* b, float min, float max) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    SamplerBuilder& s = g_sampler_builders.Get(b);
    s.lod_min = min;
    s.lod_max = max;
}
void SamplerBuilderSetLodBias(void* b, float bias) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_sampler_builders.Get(b).lod_bias = bias;
}
void SamplerBuilderSetBorderColor(void* b, const float* color) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    SamplerBuilder& s = g_sampler_builders.Get(b);
    for (int i = 0; i < 4 && color; ++i)
        s.border[i] = color[i];
}
void SamplerBuilderSetMaxAnisotropy(void* b, float aniso) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_sampler_builders.Get(b).max_anisotropy = aniso;
}
void SamplerBuilderSetReductionFilter(void* b, std::int32_t reduction) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_sampler_builders.Get(b).reduction = reduction;
}
void SamplerBuilderSetCompare(void* b, std::int32_t mode, std::int32_t func) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    SamplerBuilder& s = g_sampler_builders.Get(b);
    s.compare_mode = mode;
    s.compare_func = func;
}

std::int32_t SamplerInitialize(void* sampler, const void* builder) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    SamplerBuilder* b = g_sampler_builders.Find(builder);
    if (!b)
        return 0;
    g_samplers.Reset(sampler).settings = *b;
    return 1;
}

void SamplerFinalize(void* sampler) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
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
        std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
        auto it = g_texture_ids.find(id);
        Texture* t = it == g_texture_ids.end() ? nullptr : g_textures.Find(it->second);
        if (!t || !t->view)
            return false;
        *view = t->view;
        *layout = detail::kImageLayout;
        return true;
    }
    bool ResolveSampler(std::uint32_t id, VkSampler* sampler) override {
        std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
        auto it = g_sampler_ids.find(id);
        if (it == g_sampler_ids.end() || !it->second.sampler)
            return false;
        *sampler = it->second.sampler;
        return true;
    }
    VkDescriptorBufferInfo PlaceholderBuffer() override {
        EnsurePlaceholders();
        return {g_placeholders.buffer, 0, kPlaceholderBufferSize};
    }
    VkDescriptorImageInfo PlaceholderImage() override { return detail::PlaceholderImageFor(ImageKind::Image2D); }

    static void EnsurePlaceholders() {
        std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
        const Backend* be = CurrentBackend();
        if (!be || g_placeholders.made)
            return;
        g_placeholders.made = true;
        // 16 KB, zeroed: more than the largest uniform block of the game's shaders (1,792 bytes in
        // the manifest) and within the 16 KB maxUniformBufferRange every Vulkan device has.
        VkBufferCreateInfo bci{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bci.size = kPlaceholderBufferSize;
        // Also the vertex buffer that feeds a shader input no attribute does (command.cpp).
        bci.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        if (vkCreateBuffer(be->device, &bci, nullptr, &g_placeholders.buffer) == VK_SUCCESS) {
            VkMemoryRequirements req;
            vkGetBufferMemoryRequirements(be->device, g_placeholders.buffer, &req);
            VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
            mai.allocationSize = req.size;
            mai.memoryTypeIndex = be->MemoryType(req.memoryTypeBits, 0);
            if (vkAllocateMemory(be->device, &mai, nullptr, &g_placeholders.buffer_memory) == VK_SUCCESS)
                vkBindBufferMemory(be->device, g_placeholders.buffer, g_placeholders.buffer_memory, 0);
        }
        const VkBuffer buffer = g_placeholders.buffer;
        if (buffer)
            ImmediateSubmit([&](VkCommandBuffer cmd) { vkCmdFillBuffer(cmd, buffer, 0, kPlaceholderBufferSize, 0); });
        const VkFormat depth_format =
            FormatSupported(VK_FORMAT_D32_SFLOAT, VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) ? VK_FORMAT_D32_SFLOAT : VK_FORMAT_D16_UNORM;
        const struct {
            VkFormat format;
            bool cube;
            VkImageAspectFlags aspect;
            detail::ViewKind kind;
        } kinds[3] = {
            {VK_FORMAT_R8G8B8A8_UNORM, false, VK_IMAGE_ASPECT_COLOR_BIT, detail::ViewKind::Color2D},
            {VK_FORMAT_R8G8B8A8_UNORM, true, VK_IMAGE_ASPECT_COLOR_BIT, detail::ViewKind::Cube},
            {depth_format, false, VK_IMAGE_ASPECT_DEPTH_BIT, detail::ViewKind::Depth2D},
        };
        for (const auto& k : kinds) {
            PlaceholderView& p = g_placeholders.images[static_cast<int>(k.kind)];
            if (!CreateImage(*be, k.format, 1, 1, k.cube ? 6 : 1, 1, k.cube,
                             VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, k.aspect, VkComponentMapping{},
                             &p.image, &p.memory, &p.view))
                continue;
            InitializeImage(p.image, k.aspect, 1, k.cube ? 6 : 1, k.kind == detail::ViewKind::Depth2D, false);
            g_view_kinds[p.view] = k.kind;
        }
        g_placeholders.sampler = MakeSampler(SamplerBuilder{});
        SamplerBuilder compare;
        compare.compare_mode = 1;
        g_placeholders.compare_sampler = MakeSampler(compare);
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
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    TextureInfo out;
    if (Texture* t = g_textures.Find(texture)) {
        out.exists = true;
        out.image = t->image;
        out.view = t->view;
        out.format = t->vk_format;
        out.width = t->width;
        out.height = t->height;
        out.layers = t->layers;
        out.levels = t->levels;
        out.cube = t->cube;
    }
    return out;
}

bool detail::ViewKindOf(VkImageView view, ViewKind* kind) {
    auto it = g_view_kinds.find(view);
    if (it == g_view_kinds.end())
        return false;
    *kind = it->second;
    return true;
}

VkDescriptorImageInfo detail::PlaceholderImageFor(ImageKind kind) {
    Resolver::EnsurePlaceholders();
    const ViewKind view = kind == ImageKind::ImageCube       ? ViewKind::Cube
                          : kind == ImageKind::Image2DShadow ? ViewKind::Depth2D
                                                             : ViewKind::Color2D;
    return {view == ViewKind::Depth2D ? g_placeholders.compare_sampler : g_placeholders.sampler,
            g_placeholders.images[static_cast<int>(view)].view, kImageLayout};
}

bool detail::TextureAttachment(const void* texture, AttachmentInfo* out) {
    const Backend* be = CurrentBackend();
    Texture* t = g_textures.Find(texture);
    if (!be || !t || !t->image || !t->attachment)
        return false;
    const bool depth = t->format->depth;
    if (!t->attachment_view) {
        VkImageViewCreateInfo vci{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        vci.image = t->image;
        vci.viewType = VK_IMAGE_VIEW_TYPE_2D;
        vci.format = t->vk_format;
        vci.subresourceRange = {depth ? VkImageAspectFlags(VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT)
                                      : VkImageAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT),
                                0, 1, 0, 1};
        if (vkCreateImageView(be->device, &vci, nullptr, &t->attachment_view) != VK_SUCCESS) {
            t->attachment_view = VK_NULL_HANDLE;
            return false;
        }
    }
    out->image = t->image;
    out->view = t->attachment_view;
    out->format = t->vk_format;
    out->width = t->width;
    out->height = t->height;
    out->depth = depth;
    out->stencil = depth;  // both depth formats the shim makes have stencil (texture.h)
    return true;
}

bool ReadTexels(const void* texture, std::vector<std::uint8_t>* out) {
    const Backend* be = CurrentBackend();
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    Texture* t = g_textures.Find(texture);
    if (!be || !t || !t->image || t->format->depth || t->format->block_size != 1)
        return false;
    const VkDeviceSize bytes = static_cast<VkDeviceSize>(t->width) * t->height * t->format->block_bytes;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkBufferCreateInfo bci{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    bci.size = bytes;
    bci.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    if (vkCreateBuffer(be->device, &bci, nullptr, &buffer) != VK_SUCCESS)
        return false;
    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(be->device, buffer, &req);
    VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mai.allocationSize = req.size;
    mai.memoryTypeIndex =
        be->MemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    bool ok = false;
    void* mapped = nullptr;
    if (mai.memoryTypeIndex != UINT32_MAX && vkAllocateMemory(be->device, &mai, nullptr, &memory) == VK_SUCCESS &&
        vkBindBufferMemory(be->device, buffer, memory, 0) == VK_SUCCESS) {
        const VkImage image = t->image;
        const std::uint32_t w = t->width, h = t->height;
        ok = ImmediateSubmit([&](VkCommandBuffer cmd) {
            VkMemoryBarrier mb{VK_STRUCTURE_TYPE_MEMORY_BARRIER};
            mb.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
            mb.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 1, &mb, 0,
                                 nullptr, 0, nullptr);
            VkBufferImageCopy copy{};
            copy.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
            copy.imageExtent = {w, h, 1};
            vkCmdCopyImageToBuffer(cmd, image, detail::kImageLayout, buffer, 1, &copy);
            mb.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            mb.dstAccessMask = VK_ACCESS_HOST_READ_BIT;
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT, 0, 1, &mb, 0, nullptr, 0,
                                 nullptr);
        });
        if (ok && vkMapMemory(be->device, memory, 0, VK_WHOLE_SIZE, 0, &mapped) == VK_SUCCESS) {
            const auto* p = static_cast<const std::uint8_t*>(mapped);
            out->assign(p, p + bytes);
        } else {
            ok = false;
        }
    }
    if (memory)
        vkFreeMemory(be->device, memory, nullptr);
    vkDestroyBuffer(be->device, buffer, nullptr);
    return ok;
}

const void* RegisteredTexture(std::uint32_t id) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    auto it = g_texture_ids.find(id);
    return it == g_texture_ids.end() ? nullptr : it->second;
}

bool RegisteredSampler(std::uint32_t id) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    auto it = g_sampler_ids.find(id);
    return it != g_sampler_ids.end() && it->second.sampler != VK_NULL_HANDLE;
}

void detail::ReleaseTextureVulkanObjects() {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    const Backend* be = CurrentBackend();
    g_textures.ForEach([](const void*, Texture& t) { DestroyTexture(t); });
    for (auto& kv : g_sampler_ids) {
        if (be && kv.second.sampler)
            vkDestroySampler(be->device, kv.second.sampler, nullptr);
        kv.second.sampler = VK_NULL_HANDLE;
    }
    if (be) {
        for (PlaceholderView& p : g_placeholders.images) {
            if (p.view)
                vkDestroyImageView(be->device, p.view, nullptr);
            if (p.image)
                vkDestroyImage(be->device, p.image, nullptr);
            if (p.memory)
                vkFreeMemory(be->device, p.memory, nullptr);
        }
        if (g_placeholders.buffer)
            vkDestroyBuffer(be->device, g_placeholders.buffer, nullptr);
        if (g_placeholders.buffer_memory)
            vkFreeMemory(be->device, g_placeholders.buffer_memory, nullptr);
        if (g_placeholders.sampler)
            vkDestroySampler(be->device, g_placeholders.sampler, nullptr);
        if (g_placeholders.compare_sampler)
            vkDestroySampler(be->device, g_placeholders.compare_sampler, nullptr);
    }
    g_placeholders = Placeholders{};
    g_view_kinds.clear();
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
    Register("nvnTextureWriteTexels", &TextureWriteTexels);
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
