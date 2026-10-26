// The NVN shim's binding layer over Vulkan (include/nvn_shim/bindings.h, README.md).

#include "nvn_shim/bindings.h"

namespace dread::nvn {

VkShaderStageFlags VulkanStageFlags(Stage stage) {
    switch (stage) {
    case Stage::Vertex:
        return VK_SHADER_STAGE_VERTEX_BIT;
    case Stage::Fragment:
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    case Stage::Geometry:
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    }
    return 0;
}

std::uint32_t DescriptorSetIndex(const ShaderManifest& manifest, Stage stage) {
    switch (stage) {
    case Stage::Vertex:
        return manifest.vertex_set();
    case Stage::Fragment:
        return manifest.fragment_set();
    case Stage::Geometry:
        break;
    }
    return kNoDescriptorSet;
}

std::vector<VkDescriptorSetLayoutBinding> StageLayoutBindings(const StageInfo& stage) {
    std::vector<VkDescriptorSetLayoutBinding> out;
    out.reserve(stage.blocks.size() + stage.samplers.size());
    const VkShaderStageFlags flags = VulkanStageFlags(stage.stage);
    for (const StageResource& b : stage.blocks)
        out.push_back(VkDescriptorSetLayoutBinding{b.vk_binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, flags, nullptr});
    for (const StageResource& s : stage.samplers)
        out.push_back(VkDescriptorSetLayoutBinding{s.vk_binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                   s.size_or_count, flags, nullptr});
    return out;
}

void BindingState::BindUniformBuffer(Stage stage, std::uint32_t slot, BufferAddress address, std::uint64_t size) {
    if (!Valid(stage) || slot >= kUniformBufferSlots)
        return;
    UniformBinding& u = uniforms_[Index(stage)][slot];
    if (u.bound && u.address == address && u.size == size)
        return;
    u = UniformBinding{address, size, true};
    dirty_[Index(stage)] = true;
}

void BindingState::BindTexture(Stage stage, std::uint32_t slot, TextureHandle handle) {
    if (!Valid(stage) || slot >= kTextureSlots)
        return;
    TextureBinding& t = textures_[Index(stage)][slot];
    if (t.bound && t.handle == handle)
        return;
    t = TextureBinding{handle, true};
    dirty_[Index(stage)] = true;
}

void BindingState::Reset() {
    *this = BindingState();
}

void BindingState::BuildWrites(const StageInfo& stage, VkDescriptorSet set, ResourceResolver& resolver,
                               DescriptorWrites* out) const {
    out->writes.clear();
    out->buffers.clear();
    out->images.clear();
    out->warnings.clear();
    const int s = Valid(stage.stage) ? Index(stage.stage) : 0;

    // Infos first, all of them, so that the vectors do not move while writes point into them.
    std::size_t image_count = 0;
    for (const StageResource& r : stage.samplers)
        image_count += r.size_or_count;
    out->buffers.reserve(stage.blocks.size());
    out->images.reserve(image_count);

    for (const StageResource& b : stage.blocks) {
        VkDescriptorBufferInfo info = resolver.PlaceholderBuffer();
        if (b.nvn_slot >= 0) {
            const UniformBinding* u =
                static_cast<std::uint32_t>(b.nvn_slot) < kUniformBufferSlots ? &uniforms_[s][b.nvn_slot] : nullptr;
            VkBuffer buffer;
            VkDeviceSize offset;
            if (!u || !u->bound)
                out->warnings.push_back("uniform block " + b.name + ": slot " + std::to_string(b.nvn_slot) +
                                        " not bound");
            else if (!resolver.ResolveBuffer(u->address, u->size, &buffer, &offset))
                out->warnings.push_back("uniform block " + b.name + ": no buffer at the bound address");
            else
                info = VkDescriptorBufferInfo{buffer, offset, u->size};
        }
        out->buffers.push_back(info);
    }
    for (const StageResource& r : stage.samplers) {
        for (std::uint32_t i = 0; i < r.size_or_count; ++i) {
            VkDescriptorImageInfo info = resolver.PlaceholderImage();
            if (r.nvn_slot >= 0) {
                const std::uint32_t slot = static_cast<std::uint32_t>(r.nvn_slot) + i;
                const TextureBinding* t = slot < kTextureSlots ? &textures_[s][slot] : nullptr;
                VkImageView view;
                VkImageLayout layout;
                VkSampler sampler;
                if (!t || !t->bound)
                    out->warnings.push_back("sampler " + r.name + ": texture slot " + std::to_string(slot) +
                                            " not bound");
                else if (!resolver.ResolveTexture(TextureId(t->handle), &view, &layout) ||
                         !resolver.ResolveSampler(SamplerId(t->handle), &sampler))
                    out->warnings.push_back("sampler " + r.name + ": texture or sampler pool entry not registered");
                else
                    info = VkDescriptorImageInfo{sampler, view, layout};
            }
            out->images.push_back(info);
        }
    }

    // One write per binding; an array's elements are consecutive in `images`.
    out->writes.reserve(stage.blocks.size() + stage.samplers.size());
    for (std::size_t i = 0; i < stage.blocks.size(); ++i) {
        VkWriteDescriptorSet w{};
        w.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        w.dstSet = set;
        w.dstBinding = stage.blocks[i].vk_binding;
        w.descriptorCount = 1;
        w.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        w.pBufferInfo = &out->buffers[i];
        out->writes.push_back(w);
    }
    std::size_t first = 0;
    for (const StageResource& r : stage.samplers) {
        VkWriteDescriptorSet w{};
        w.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        w.dstSet = set;
        w.dstBinding = r.vk_binding;
        w.descriptorCount = r.size_or_count;
        w.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        w.pImageInfo = &out->images[first];
        first += r.size_or_count;
        out->writes.push_back(w);
    }
}

}  // namespace dread::nvn
