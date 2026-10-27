#pragma once

// NVN textures, texture and sampler pools, samplers, over Vulkan. Signatures are those of the game's
// calls: the render targets fn_7100160f60 (builder, storage, TextureInitialize,
// TexturePoolRegisterTexture), the cube target fn_710015f880, the linear texture fn_710015fff0, the
// packaged (asset) texture fn_7100160260, the pools fn_710015f100, the samplers fn_710015e9fc, the
// swizzles fn_71001604e0, the texture handles fn_7100161250.
//
// What each NVN number means is taken from evidence, listed per entry in texture.cpp's table:
//  - formats: the engine's converter fn_7100161410 turns a texture header's type, sRGB flag and
//    channel layout into the number it passes to nvnTextureBuilderSetFormat (fn_7100160260); the
//    layouts are the engine's reflected enum EComponents (fn_710016468c names them: R8_G8_B8,
//    R8_G8_B8_A8, L8, L8_A8, R5_G6_B5, R5_G5_B5_A1, U16_V16, R32F, ...), and with the sRGB flag the
//    converter returns the sRGB numbers (0x37, 0x38, 0x46, 0x48: 2,063 of the 5,754 .bctex, the
//    base colours). For the linear files the result equals the format mercury-engine-data-structures
//    names (XTX_Tegra_Format). The depth target's 0x35 and its neighbour 0x33 are depth formats by
//    use (fn_71001587f0's depth target, fn_71001604e0's R,R,R,A swizzle), precision not known:
//    provisionally D32 float with 8-bit stencil (the game sets stencil state), D24/S8 where a device
//    lacks it. 0x3f is a colour render target by use (a fast-clear colour is registered for it),
//    layout not known: provisionally RGBA16 float. Any other number fails TextureInitialize, naming
//    it, rather than guessing (the render-target table at 0x7101647fec also holds 0x19, 0x29, 0x2a,
//    0x2e, 0x32, 0x34, 0x36). A format the device cannot use also fails by name.
//  - targets: 1 is 2D (5,480 of the 5,754 .bctex, and the render targets), 8 a cube map (the other
//    274: every one square, a header depth of 1, six faces of data, BC6H, a cube/IBL/environment
//    map; fn_710015f880 makes one with a depth of 1). No .bctex has another target; others fail.
// The swizzle values are those fn_71001604e0 passes: 2 3 4 5 the identity (R G B A), 1 one;
// 0 is taken as zero.
//
// Samplers: the game's filter, wrap and compare numbers are its own packing of NVN's enumerations,
// which nothing in the game names; until a use shows them, every sampler is linear, repeat, with the
// game's LOD clamp, bias and anisotropy (numbers, so exact) and a compare op when it asks for one.
// New images are moved to the layout the resolver reports. Known gaps: a slot the game leaves
// empty gets a 2D placeholder even where the shader declares a cube or a shadow sampler; one Vulkan
// allocation per texture (the asset set will need suballocation).
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
