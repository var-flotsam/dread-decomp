#pragma once

// The NVN shim's binding layer over Vulkan: NVN's per-stage uniform-buffer and texture slots,
// mapped onto one Vulkan descriptor set per stage. port/nvn/README.md has the design; in short:
//
//   NVN (as the game uses it)                       Vulkan (as tools/bshdat.py --install compiles)
//   uniform buffer slot b of stage s           ->   set S(s), binding b, UNIFORM_BUFFER
//   texture slot t of stage s, handle h        ->   set S(s), binding 16 + t0, element t - t0,
//     (t in a sampler array declared at t0)          COMBINED_IMAGE_SAMPLER: the texture pool entry
//                                                     TextureId(h) with the sampler SamplerId(h)
//   a block or sampler the NVN compiler found  ->   its binding gets a placeholder descriptor
//     unused but the SPIR-V still declares
//
// S(vertex) = 0 and S(fragment) = 1 (the manifest's layout record). The slot numbers are the ones
// NVN's compiler assigned and the engine binds with (its program binding, fn_710015d900, binds
// each block at the slot the stage's reflection gives it; fn_7100161250 binds texture slots 0-15),
// so the shim passes them through without knowing anything about the engine's resources.

#include <cstdint>
#include <string>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "nvn_shim/nvn_types.h"
#include "nvn_shim/shader_manifest.h"

namespace dread::nvn {

// The shim's other parts answer these: memory pools and buffers own the GPU address space, the
// texture and sampler pools own the pool indices. Placeholders stand in for what a shader declares
// but nothing binds (an unused block or sampler, a slot the game left empty).
class ResourceResolver {
public:
    virtual ~ResourceResolver() = default;
    // The Vulkan buffer and offset behind [address, address + size). False if no buffer holds it.
    virtual bool ResolveBuffer(BufferAddress address, std::uint64_t size, VkBuffer* buffer,
                               VkDeviceSize* offset) = 0;
    // The view and layout of texture pool entry `id` (nvnTexturePoolRegisterTexture).
    virtual bool ResolveTexture(std::uint32_t id, VkImageView* view, VkImageLayout* layout) = 0;
    // Sampler pool entry `id` (nvnSamplerPoolRegisterSampler).
    virtual bool ResolveSampler(std::uint32_t id, VkSampler* sampler) = 0;
    // Placeholders: a uniform buffer at least as large as the largest block of the manifest
    // (StageResource::size_or_count, unused blocks included, so a shader that reads one stays in
    // bounds; a range of VK_WHOLE_SIZE is fine), and a 2D image with a sampler.
    virtual VkDescriptorBufferInfo PlaceholderBuffer() = 0;
    virtual VkDescriptorImageInfo PlaceholderImage() = 0;
};

// The descriptor set layout of one stage: blocks first, then samplers, in the manifest's order.
std::vector<VkDescriptorSetLayoutBinding> StageLayoutBindings(const StageInfo& stage);

// The VkShaderStageFlags of an NVN stage.
VkShaderStageFlags VulkanStageFlags(Stage stage);

// The descriptor set index of an NVN stage in the manifest's layout; kNoDescriptorSet for the
// geometry stage, which no shader of the game has.
constexpr std::uint32_t kNoDescriptorSet = 0xFFFFFFFF;
std::uint32_t DescriptorSetIndex(const ShaderManifest& manifest, Stage stage);

// Descriptor writes for one stage's set, with the infos they point into. Build() fills all three
// vectors at once; the pointers in `writes` stay valid while the object is not modified.
struct DescriptorWrites {
    std::vector<VkWriteDescriptorSet> writes;
    std::vector<VkDescriptorBufferInfo> buffers;
    std::vector<VkDescriptorImageInfo> images;
    std::vector<std::string> warnings;  // slots the game left empty, unresolved addresses or ids
};

// What the game has bound, per NVN stage and slot, as nvnCommandBufferBindUniformBuffer and
// nvnCommandBufferBindTexture leave it. A command buffer owns one; binding a program does not
// reset it (NVN keeps bindings across program changes). fn_710015d900 rebinds a program's blocks
// only when asked to and the program or its depth-only flag changed; for a depth-only pass it binds
// the program and then unbinds the fragment stage (nvnCommandBufferBindProgram(cmd, null, 2)) and
// binds no fragment blocks, so the pipeline layer needs vertex-only pipelines and the fragment set
// is then not used.
class BindingState {
public:
    // nvnCommandBufferBindUniformBuffer(cmd, stage, slot, address, size).
    void BindUniformBuffer(Stage stage, std::uint32_t slot, BufferAddress address, std::uint64_t size);
    // nvnCommandBufferBindTexture(cmd, stage, slot, handle).
    void BindTexture(Stage stage, std::uint32_t slot, TextureHandle handle);
    // Forget everything (a new command buffer).
    void Reset();

    // Whether anything of `stage` changed since MarkClean(stage).
    bool Dirty(Stage stage) const { return Valid(stage) && dirty_[Index(stage)]; }
    void MarkClean(Stage stage) {
        if (Valid(stage))
            dirty_[Index(stage)] = false;
    }

    // The writes that fill `set` (laid out by StageLayoutBindings(stage)) from the current
    // bindings of stage.stage. Slots the game did not bind and resources the resolver cannot
    // find get placeholders and a warning; the set is always complete.
    void BuildWrites(const StageInfo& stage, VkDescriptorSet set, ResourceResolver& resolver,
                     DescriptorWrites* out) const;

private:
    static constexpr int kStages = 3;
    static bool Valid(Stage stage) { return static_cast<std::uint32_t>(stage) < kStages; }
    static int Index(Stage stage) { return static_cast<int>(stage); }

    struct UniformBinding {
        BufferAddress address = 0;
        std::uint64_t size = 0;
        bool bound = false;
    };
    struct TextureBinding {
        TextureHandle handle = 0;
        bool bound = false;
    };
    UniformBinding uniforms_[kStages][kUniformBufferSlots];
    TextureBinding textures_[kStages][kTextureSlots];
    bool dirty_[kStages] = {true, true, true};
};

}  // namespace dread::nvn
