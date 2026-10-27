// NVN programs. Signatures are those of the game's calls: the engine's program creation
// fn_710015d760 (nvnProgramInitialize(program, device), then nvnProgramSetShaders(program, count,
// ShaderData[count]) with one entry per present stage) and its release fn_710015c690
// (nvnProgramFinalize). A ShaderData's control section names the stage (ShaderManifest::FindStage),
// so the shim never reads the GPU code the engine copied for the console; the SPIR-V is the one the
// build installed from the same package (tools/bshdat.py --install).

#include "program.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include <vulkan/vulkan.h>

#include "nvn_shim/bindings.h"
#include "nvn_shim/device.h"
#include "nvn_shim/draw.h"
#include "nvn_shim/procs.h"
#include "objects.h"

namespace dread::nvn {

namespace {

std::string g_shader_dir;
ShaderManifest g_manifest;
bool g_manifest_loaded = false, g_manifest_failed = false;

// Per stage of the manifest: its module, set layout and interface, made on first use.
struct StageObjects {
    VkShaderModule module = VK_NULL_HANDLE;
    VkDescriptorSetLayout set_layout = VK_NULL_HANDLE;
    SpirvScan scan;
    bool failed = false;
};
std::map<const StageInfo*, StageObjects> g_stages;
// Per pair of stages (vertex, fragment).
std::map<std::pair<const StageInfo*, const StageInfo*>, std::unique_ptr<detail::ProgramObjects>> g_programs;
VkDescriptorSetLayout g_empty_layout = VK_NULL_HANDLE;  // the fragment set of a vertex-only program

struct Program {
    const detail::ProgramObjects* objects = nullptr;
};
detail::Registry<Program> g_program_registry;

bool LoadManifest() {
    if (g_manifest_loaded)
        return true;
    if (g_manifest_failed)
        return false;
    std::string dir = g_shader_dir;
    if (dir.empty()) {
        const char* env = std::getenv("DREAD_NVN_SHADERS");
        dir = env ? env : "";
    }
    std::string error;
    if (dir.empty())
        error = "no shader directory (SetShaderDirectory or $DREAD_NVN_SHADERS)";
    else if (g_manifest.Load(dir, &error))
        return g_manifest_loaded = true;
    std::fprintf(stderr, "nvn shim: no shader manifest: %s; every nvnProgramSetShaders fails\n", error.c_str());
    g_manifest_failed = true;
    return false;
}

bool ReadFile(const std::string& path, std::vector<unsigned char>* out) {
    std::ifstream in(path, std::ios::binary);
    if (!in)
        return false;
    out->assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    return true;
}

StageObjects* GetStage(const Backend& be, const StageInfo* stage) {
    StageObjects& s = g_stages[stage];
    if (s.module || s.failed)
        return s.failed ? nullptr : &s;
    std::vector<unsigned char> spv;
    const std::string path = g_manifest.dir() + "/" + stage->spirv_path;
    VkShaderModuleCreateInfo mci{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    if (!ReadFile(path, &spv) || !ScanSpirv(spv, &s.scan)) {
        std::fprintf(stderr, "nvn shim: cannot read %s\n", path.c_str());
        s.failed = true;
        return nullptr;
    }
    mci.codeSize = spv.size();
    mci.pCode = reinterpret_cast<const std::uint32_t*>(spv.data());
    const std::vector<VkDescriptorSetLayoutBinding> bindings = StageLayoutBindings(*stage);
    VkDescriptorSetLayoutCreateInfo lci{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    lci.bindingCount = static_cast<std::uint32_t>(bindings.size());
    lci.pBindings = bindings.data();
    if (vkCreateShaderModule(be.device, &mci, nullptr, &s.module) != VK_SUCCESS ||
        vkCreateDescriptorSetLayout(be.device, &lci, nullptr, &s.set_layout) != VK_SUCCESS) {
        std::fprintf(stderr, "nvn shim: no Vulkan shader module or set layout for %s\n", path.c_str());
        s.failed = true;
        return nullptr;
    }
    return &s;
}

const detail::ProgramObjects* GetProgram(const Backend& be, const StageInfo* vertex, const StageInfo* fragment) {
    std::unique_ptr<detail::ProgramObjects>& slot = g_programs[{vertex, fragment}];
    if (slot)
        return slot.get();
    auto p = std::make_unique<detail::ProgramObjects>();
    const StageInfo* stages[2] = {vertex, fragment};
    for (int i = 0; i < 2; ++i) {
        if (!stages[i])
            continue;
        StageObjects* s = GetStage(be, stages[i]);
        if (!s) {
            g_programs.erase({vertex, fragment});
            return nullptr;
        }
        p->stages[i] = stages[i];
        p->modules[i] = s->module;
        p->set_layouts[i] = s->set_layout;
        (i == 0 ? p->vertex_inputs : p->fragment_outputs) = i == 0 ? s->scan.inputs : s->scan.outputs;
        for (const auto& kv : s->scan.images)
            p->samplers[i][kv.first.second] = kv.second;  // one set per stage
    }
    if (!g_empty_layout) {
        VkDescriptorSetLayoutCreateInfo lci{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
        vkCreateDescriptorSetLayout(be.device, &lci, nullptr, &g_empty_layout);
    }
    // Both sets at the manifest's indices; a missing stage's set is an empty one.
    const std::uint32_t vertex_set = g_manifest.vertex_set(), fragment_set = g_manifest.fragment_set();
    std::vector<VkDescriptorSetLayout> sets(std::max(vertex_set, fragment_set) + 1, g_empty_layout);
    sets[vertex_set] = p->set_layouts[0] ? p->set_layouts[0] : g_empty_layout;
    sets[fragment_set] = p->set_layouts[1] ? p->set_layouts[1] : g_empty_layout;
    VkPipelineLayoutCreateInfo pci{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    pci.setLayoutCount = static_cast<std::uint32_t>(sets.size());
    pci.pSetLayouts = sets.data();
    if (vkCreatePipelineLayout(be.device, &pci, nullptr, &p->layout) != VK_SUCCESS) {
        g_programs.erase({vertex, fragment});
        return nullptr;
    }
    slot = std::move(p);
    return slot.get();
}

std::int32_t ProgramInitialize(void* program, const void*) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_program_registry.Reset(program);
    return 1;
}

void ProgramFinalize(void* program) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_program_registry.Erase(program);
}

// Returns NVN's boolean (fn_710015d760 ignores it). Fails, naming the stage, when a control section
// is not one the build installed (another game version, or the shaders target was not built).
std::int32_t ProgramSetShaders(void* program, std::int32_t count, const ShaderData* stages) {
    const Backend* be = CurrentBackend();
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    Program& p = g_program_registry.Get(program);
    p.objects = nullptr;
    if (!be || !LoadManifest() || count <= 0 || !stages)
        return 0;
    const StageInfo* found[2] = {};
    for (std::int32_t i = 0; i < count; ++i) {
        const StageInfo* s = stages[i].control ? g_manifest.FindStage(stages[i].control) : nullptr;
        if (!s || (s->stage != Stage::Vertex && s->stage != Stage::Fragment)) {
            std::fprintf(stderr, "nvn shim: nvnProgramSetShaders: stage %d (control key %016llx) is not in the manifest\n",
                         static_cast<int>(i),
                         stages[i].control ? static_cast<unsigned long long>(ShaderManifest::ControlKey(stages[i].control))
                                           : 0ull);
            return 0;
        }
        found[static_cast<int>(s->stage)] = s;
    }
    if (!found[0]) {
        std::fprintf(stderr, "nvn shim: nvnProgramSetShaders: a program without a vertex stage\n");
        return 0;
    }
    p.objects = GetProgram(*be, found[0], found[1]);
    return p.objects ? 1 : 0;
}

void ProgramSetDebugLabel(void*, const char*) {}

}  // namespace

void SetShaderDirectory(const char* dir) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_shader_dir = dir ? dir : "";
    g_manifest_loaded = g_manifest_failed = false;
    g_manifest = ShaderManifest();
}

bool ShadersLoaded() {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    return g_manifest_loaded;
}

const detail::ProgramObjects* detail::FindProgramObjects(const void* program) {
    Program* p = g_program_registry.Find(program);
    return p ? p->objects : nullptr;
}

const ShaderManifest* detail::LoadedManifest() {
    return g_manifest_loaded ? &g_manifest : nullptr;
}

void detail::ReleaseProgramVulkanObjects() {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    const Backend* be = CurrentBackend();
    if (be) {
        for (auto& kv : g_programs)
            if (kv.second && kv.second->layout)
                vkDestroyPipelineLayout(be->device, kv.second->layout, nullptr);
        for (auto& kv : g_stages) {
            if (kv.second.module)
                vkDestroyShaderModule(be->device, kv.second.module, nullptr);
            if (kv.second.set_layout)
                vkDestroyDescriptorSetLayout(be->device, kv.second.set_layout, nullptr);
        }
        if (g_empty_layout)
            vkDestroyDescriptorSetLayout(be->device, g_empty_layout, nullptr);
    }
    g_programs.clear();
    g_stages.clear();
    g_empty_layout = VK_NULL_HANDLE;
    // The programs keep their registry entries but lose their Vulkan objects.
    g_program_registry.ForEach([](const void*, Program& p) { p.objects = nullptr; });
}

void detail::RegisterProgramProcs() {
    Register("nvnProgramInitialize", &ProgramInitialize);
    Register("nvnProgramFinalize", &ProgramFinalize);
    Register("nvnProgramSetShaders", &ProgramSetShaders);
    Register("nvnProgramSetDebugLabel", &ProgramSetDebugLabel);
}

}  // namespace dread::nvn
