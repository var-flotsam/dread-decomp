#pragma once

// NVN textures, texture and sampler pools, samplers, over Vulkan. Signatures are those of the game's
// calls: the render targets fn_7100160f60 (builder, storage, TextureInitialize,
// TexturePoolRegisterTexture), the cube target fn_710015f880, the linear texture fn_710015fff0, the
// packaged (asset) texture fn_7100160260, the pools fn_710015f100, the samplers fn_710015e9fc, the
// swizzles fn_71001604e0, the texture handles fn_7100161250.
//
// What each NVN number means is taken from evidence, listed in FormatInfo and TargetOf:
//  - formats: mercury-engine-data-structures names the formats the game's .bctex files use
//    (XTX_Tegra_Format: 0x1 R8, 0xd R8G8, 0x25 R8G8B8A8, 0x42 BC1, 0x44 BC3, 0x4b BC5, 0x50 BC6H
//    unsigned float, 0x6d B8G8R8A8, all UNORM but BC6H); the depth target's 0x35 and its neighbour
//    0x33 are depth formats by use (fn_71001587f0 creates the depth target with it,
//    fn_71001604e0 gives both the swizzle R,R,R,A), their precision is not known: provisionally
//    D32 float with 8-bit stencil (the game sets stencil state). Any other number fails
//    TextureInitialize, naming it, rather than guessing.
//  - targets: 1 is 2D (1,411 of 1,500 sampled .bctex, and the render targets), 8 a cube map (every
//    target-8 .bctex is square, six faces, BC6H, named cube/ibl/env; fn_710015f880 makes one with
//    a depth of 1). Others fail.
// The swizzle values are those fn_71001604e0 passes: 2 3 4 5 the identity (R G B A), 1 one;
// 0 is taken as zero.
//
// Samplers: the game's filter, wrap and compare numbers are its own packing of NVN's enumerations,
// which nothing in the game names; until a use shows them, every sampler is linear, repeat, with the
// game's LOD clamp, bias and anisotropy (numbers, so exact) and a compare op when it asks for one.
// Packaged (asset) texture data is kept by pointer, not uploaded yet: the .bctex data is in the
// console's tiled layout, which the upload step will untile.

#include <cstdint>

#include <vulkan/vulkan_core.h>

#include "nvn_shim/bindings.h"

namespace dread::nvn {

struct FormatInfo {
    std::int32_t nvn;
    VkFormat vk;
    std::uint32_t block_bytes;  // bytes per block (per texel for uncompressed formats)
    std::uint32_t block_size;   // texels per block side (1 uncompressed, 4 for BC)
    bool depth;
    bool provisional;           // meaning inferred from use; precision not known
    const char* evidence;
};
// The table entry for an NVN format, null when the shim does not know it.
const FormatInfo* FindFormat(std::int32_t nvn);

// The binding layer's resolver over the shim's own objects: buffers through the memory pools'
// address space, textures and samplers through the pools the game registered them in, and
// placeholders made on first use. Valid while the device lives.
ResourceResolver& ShimResolver();

// For checks.
struct TextureInfo {
    bool exists = false;
    VkImage image = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    VkFormat format = VK_FORMAT_UNDEFINED;
    std::uint32_t width = 0, height = 0, layers = 0, levels = 0;
    bool cube = false;
};
TextureInfo GetTextureInfo(const void* texture);
// The texture registered at `id` in the texture pool (TexturePoolRegisterTexture), else null.
const void* RegisteredTexture(std::uint32_t id);
bool RegisteredSampler(std::uint32_t id);

}  // namespace dread::nvn
