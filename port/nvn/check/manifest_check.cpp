// nvn_manifest_check <shader dir> <dump's romfs/system/shd dir>
//
// Runs the shim's manifest reader and binding layer over the real build output, with no GPU and no
// Vulkan loader (the binding layer only fills Vulkan structs). For every stage of every program:
//  - its SPIR-V file is there, with the manifest's size, and the descriptor bindings the module
//    itself declares (set, binding, kind, array length, read from its OpDecorate instructions)
//    are exactly the manifest's blocks and samplers in the stage's set: the independent truth
//    the manifest's layout is compared with;
//  - the control section, read from the package at the manifest's offset, hashes to the stage's
//    key, and ShaderManifest::FindStage finds that stage or one with the same SPIR-V bytes,
//    resources and interface (the shim hands out the one it finds);
//  - StageLayoutBindings gives one binding per resource, no binding twice, no count of 0;
//  - with every NVN slot bound (a fake resolver standing in for the pools and buffers),
//    BindingState::BuildWrites writes every binding, resolves every resource NVN uses to what was
//    bound at its slot, uses placeholders only for the unused ones, and its pointers stay inside its
//    own info arrays.
// Exit status 0 when every check passes. Reads game data from build/ and orig/; prints only counts
// and the names of failing stages. Block sizes are not checked here: tools/bshdat.py --install
// compares them with the SPIR-V layout when it writes the manifest.

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <set>
#include <string>
#include <vector>

#include "nvn_shim/bindings.h"
#include "nvn_shim/shader_manifest.h"
#include "spirv_scan.h"

using namespace dread::nvn;
using check::ScanSpirv;
using check::SpirvBindings;
using check::SpirvScan;

namespace {

bool ReadFile(const std::string& path, std::vector<unsigned char>* out) {
    std::FILE* f = std::fopen(path.c_str(), "rb");
    if (!f)
        return false;
    out->clear();
    unsigned char buf[1 << 16];
    std::size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), f)) > 0)
        out->insert(out->end(), buf, buf + n);
    bool ok = !std::ferror(f);
    std::fclose(f);
    return ok;
}

// What the fake resolver hands out: handles and offsets that encode where they came from.
constexpr BufferAddress kBufferBase = 0x10000000;
BufferAddress SlotAddress(Stage stage, std::uint32_t slot) {
    return kBufferBase + (static_cast<BufferAddress>(stage) << 20) + slot * 0x1000;
}
constexpr std::uint32_t kSamplerOffset = 100;

template <class Handle>
Handle FakeHandle(std::uint64_t v) {
    // Non-dispatchable handles are pointers to opaque structs on 64-bit hosts and uint64_t on 32-bit
    // ones; a C-style cast is a reinterpret_cast for the first and a conversion for the second.
    // Never dereferenced.
    return (Handle)(std::uintptr_t)v;
}

class FakeResolver : public ResourceResolver {
public:
    bool ResolveBuffer(BufferAddress address, std::uint64_t, VkBuffer* buffer, VkDeviceSize* offset) override {
        *buffer = FakeHandle<VkBuffer>(0xB0000000);
        *offset = address - kBufferBase;
        return true;
    }
    bool ResolveTexture(std::uint32_t id, VkImageView* view, VkImageLayout* layout) override {
        *view = FakeHandle<VkImageView>(0x1000000 + id);
        *layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        return true;
    }
    bool ResolveSampler(std::uint32_t id, VkSampler* sampler) override {
        *sampler = FakeHandle<VkSampler>(0x2000000 + id);
        return true;
    }
    VkDescriptorBufferInfo PlaceholderBuffer() override {
        return VkDescriptorBufferInfo{FakeHandle<VkBuffer>(0xDEAD0000), 0, VK_WHOLE_SIZE};
    }
    VkDescriptorImageInfo PlaceholderImage() override {
        return VkDescriptorImageInfo{FakeHandle<VkSampler>(0xDEAD0001), FakeHandle<VkImageView>(0xDEAD0002),
                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
    }
};

int g_failures = 0;  // failed checks; a stage can fail several
std::set<std::string> g_failed_stages;
void Fail(const std::string& where, const std::string& what) {
    g_failed_stages.insert(where);
    if (++g_failures <= 20)
        std::fprintf(stderr, "FAIL %s: %s\n", where.c_str(), what.c_str());
}

bool SameResources(const std::vector<StageResource>& a, const std::vector<StageResource>& b) {
    if (a.size() != b.size())
        return false;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (a[i].nvn_slot != b[i].nvn_slot || a[i].vk_binding != b[i].vk_binding ||
            a[i].size_or_count != b[i].size_or_count || a[i].name != b[i].name)
            return false;
    return true;
}

bool SameInterface(const std::vector<StageInterface>& a, const std::vector<StageInterface>& b) {
    if (a.size() != b.size())
        return false;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (a[i].location != b[i].location || a[i].name != b[i].name)
            return false;
    return true;
}

// What the shim takes from the stage FindStage returns, compared field by field.
bool SameStage(const StageInfo& a, const StageInfo& b) {
    return a.stage == b.stage && a.spirv_size == b.spirv_size && SameResources(a.blocks, b.blocks) &&
           SameResources(a.samplers, b.samplers) && SameInterface(a.inputs, b.inputs) &&
           SameInterface(a.outputs, b.outputs);
}

template <class T>
bool Inside(const T* p, const std::vector<T>& v) {
    return !v.empty() && p >= v.data() && p < v.data() + v.size();
}

void CheckBindings(const std::string& where, const StageInfo& st) {
    const std::vector<VkDescriptorSetLayoutBinding> layout = StageLayoutBindings(st);
    if (layout.size() != st.blocks.size() + st.samplers.size())
        Fail(where, "layout has another number of bindings than resources");
    std::set<std::uint32_t> seen;
    for (const VkDescriptorSetLayoutBinding& b : layout) {
        if (!seen.insert(b.binding).second)
            Fail(where, "binding " + std::to_string(b.binding) + " twice");
        if (b.descriptorCount == 0)
            Fail(where, "binding " + std::to_string(b.binding) + " with count 0");
        if (b.stageFlags != VulkanStageFlags(st.stage))
            Fail(where, "stage flags");
    }

    BindingState state;
    for (std::uint32_t slot = 0; slot < kUniformBufferSlots; ++slot)
        state.BindUniformBuffer(st.stage, slot, SlotAddress(st.stage, slot), 0x10000);
    for (std::uint32_t slot = 0; slot < kTextureSlots; ++slot)
        state.BindTexture(st.stage, slot, MakeTextureHandle(slot, slot + kSamplerOffset));
    FakeResolver resolver;
    DescriptorWrites out;
    const VkDescriptorSet set = FakeHandle<VkDescriptorSet>(0x5E7);
    state.BuildWrites(st, set, resolver, &out);
    if (!out.warnings.empty())
        Fail(where, "warning with every slot bound: " + out.warnings[0]);
    if (out.writes.size() != layout.size())
        Fail(where, "writes do not cover the layout");
    for (std::size_t i = 0; i < out.writes.size() && i < layout.size(); ++i) {
        const VkWriteDescriptorSet& w = out.writes[i];
        if (w.dstSet != set || w.dstBinding != layout[i].binding || w.descriptorType != layout[i].descriptorType ||
            w.descriptorCount != layout[i].descriptorCount)
            Fail(where, "write " + std::to_string(i) + " does not match its layout binding");
        if (w.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
            if (!Inside(w.pBufferInfo, out.buffers) || w.pImageInfo)
                Fail(where, "buffer write points outside its infos");
        } else if (!Inside(w.pImageInfo, out.images) || !Inside(w.pImageInfo + w.descriptorCount - 1, out.images) ||
                   w.pBufferInfo) {
            Fail(where, "image write points outside its infos");
        }
    }
    // Each resource NVN uses resolves to what was bound at its slot; the unused ones get placeholders.
    for (std::size_t i = 0; i < st.blocks.size() && i < out.buffers.size(); ++i) {
        const StageResource& b = st.blocks[i];
        const VkDescriptorBufferInfo& info = out.buffers[i];
        if (b.nvn_slot >= 0) {
            if (info.buffer != FakeHandle<VkBuffer>(0xB0000000) ||
                info.offset != SlotAddress(st.stage, static_cast<std::uint32_t>(b.nvn_slot)) - kBufferBase)
                Fail(where, "block " + b.name + " not bound to its slot's buffer");
            if (b.vk_binding != static_cast<std::uint32_t>(b.nvn_slot))
                Fail(where, "block " + b.name + " not at binding = its slot");
        } else if (info.buffer != FakeHandle<VkBuffer>(0xDEAD0000)) {
            Fail(where, "unused block " + b.name + " without the placeholder");
        }
    }
    std::size_t image = 0;
    for (const StageResource& r : st.samplers) {
        for (std::uint32_t e = 0; e < r.size_or_count && image < out.images.size(); ++e, ++image) {
            const VkDescriptorImageInfo& info = out.images[image];
            if (r.nvn_slot >= 0) {
                const std::uint32_t slot = static_cast<std::uint32_t>(r.nvn_slot) + e;
                if (info.imageView != FakeHandle<VkImageView>(0x1000000 + slot) ||
                    info.sampler != FakeHandle<VkSampler>(0x2000000 + slot + kSamplerOffset))
                    Fail(where, "sampler " + r.name + " element " + std::to_string(e) + " not bound to its slot");
                if (r.vk_binding != 16 + static_cast<std::uint32_t>(r.nvn_slot))
                    Fail(where, "sampler " + r.name + " not at binding 16 + its slot");
            } else if (info.imageView != FakeHandle<VkImageView>(0xDEAD0002) ||
                       info.sampler != FakeHandle<VkSampler>(0xDEAD0001)) {
                Fail(where, "unused sampler " + r.name + " without the placeholder");
            }
        }
    }
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 3) {
        std::fprintf(stderr, "usage: %s <shader dir> <romfs/system/shd dir>\n", argv[0]);
        return 2;
    }
    ShaderManifest manifest;
    std::string error;
    if (!manifest.Load(argv[1], &error)) {
        std::fprintf(stderr, "%s\n", error.c_str());
        return 1;
    }
    std::size_t programs = 0, stages = 0, found_self = 0;
    std::set<std::uint64_t> keys;
    std::vector<unsigned char> pkg, spv;
    for (const PackageInfo& p : manifest.packages()) {
        const std::string pkg_path = std::string(argv[2]) + "/" + p.name + ".bshdat";
        if (!ReadFile(pkg_path, &pkg)) {
            Fail(p.name, "cannot read " + pkg_path);
            continue;
        }
        for (const ProgramInfo& prog : p.programs) {
            ++programs;
            for (const StageInfo& st : prog.stages) {
                ++stages;
                const std::string where = p.name + "/p" + std::to_string(prog.key) +
                                          (st.stage == Stage::Vertex ? ".vert" : ".frag");
                SpirvScan scan;
                if (!ReadFile(manifest.dir() + "/" + st.spirv_path, &spv) || spv.size() != st.spirv_size) {
                    Fail(where, "SPIR-V file missing or of another size");
                } else if (!ScanSpirv(spv, &scan)) {
                    Fail(where, "SPIR-V module unreadable or binding something else");
                } else {
                    SpirvBindings want;
                    const std::uint32_t set = DescriptorSetIndex(manifest, st.stage);
                    for (const StageResource& b : st.blocks)
                        want[{set, b.vk_binding}] = {true, 1u};
                    for (const StageResource& r : st.samplers)
                        want[{set, r.vk_binding}] = {false, r.size_or_count};
                    if (scan.bindings != want || want.size() != st.blocks.size() + st.samplers.size())
                        Fail(where, "the manifest's blocks and samplers are not what the SPIR-V binds");
                }
                if (static_cast<std::uint64_t>(st.control_offset) + st.control_size > pkg.size()) {
                    Fail(where, "control section outside the package");
                } else {
                    const unsigned char* control = pkg.data() + st.control_offset;
                    if (ShaderManifest::ControlKey(control) != st.control_key)
                        Fail(where, "the package's control section does not hash to the stage's key");
                    const StageInfo* found = manifest.FindStage(control);
                    std::vector<unsigned char> found_spv;
                    if (!found || found->control_key != st.control_key)
                        Fail(where, "FindStage does not find the stage");
                    else if (found == &st)
                        ++found_self;
                    else if (!SameStage(*found, st) ||
                             !ReadFile(manifest.dir() + "/" + found->spirv_path, &found_spv) || found_spv != spv)
                        Fail(where, "FindStage returns " + found->spirv_path + ", which differs from this stage");
                }
                keys.insert(st.control_key);
                CheckBindings(where, st);
            }
        }
    }
    std::printf("%zu packages, %zu programs, %zu stages (%zu distinct control keys; FindStage returned the "
                "stage itself for %zu, an identical one for the rest): %zu stages failed %d checks\n",
                manifest.packages().size(), programs, stages, keys.size(), found_self, g_failed_stages.size(),
                g_failures);
    return g_failures ? 1 : 0;
}
