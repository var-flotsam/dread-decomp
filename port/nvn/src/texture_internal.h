#pragma once

// Textures as render targets, for the command buffer (texture.cpp). Under detail::StateLock().

#include <cstdint>

#include <vulkan/vulkan_core.h>

#include "nvn_shim/spirv_scan.h"

namespace dread::nvn::detail {

// Every image of the shim stays in VK_IMAGE_LAYOUT_GENERAL: the game samples its render targets
// (the deferred passes read the G-buffer), so a texture is a target and a source by turns, and NVN
// has no layouts; the command buffer orders the uses with barriers instead (command.cpp).
constexpr VkImageLayout kImageLayout = VK_IMAGE_LAYOUT_GENERAL;

struct AttachmentInfo {
    VkImage image = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;  // level 0, layer 0, identity swizzle (an attachment needs one)
    VkFormat format = VK_FORMAT_UNDEFINED;
    std::uint32_t width = 0, height = 0;
    bool depth = false, stencil = false;
};
// The texture at `texture` as a render target; false if it has no image or cannot be one.
bool TextureAttachment(const void* texture, AttachmentInfo* out);

// What a sampled view is, for matching it to the sampler a shader declares (Vulkan requires a cube
// view for a samplerCube and a depth format for a sampler2DShadow): every texture's view and the
// placeholders'. A depth view serves a sampler2D too.
enum class ViewKind { Color2D = 0, Cube = 1, Depth2D = 2 };
bool ViewKindOf(VkImageView view, ViewKind* kind);
// The placeholder for an empty (or mismatched) slot of a sampler of `kind`.
VkDescriptorImageInfo PlaceholderImageFor(ImageKind kind);

}  // namespace dread::nvn::detail
