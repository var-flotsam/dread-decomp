#pragma once

// The table of compiled shaders the port's build installs beside the executable:
// <shader dir>/shaders.manifest and the SPIR-V it names, written by `tools/bshdat.py --install`
// (the `shaders` target; the line format is in that tool's install()). Everything in it is derived
// from the user's own dump at build time and stays under build/.
//
// The shim uses it twice. nvnProgramSetShaders hands over each stage's control section, which
// FindStage turns into the stage's SPIR-V and binding layout. The engine's own variant selection
// (fn_71001b0f24, docs/PORT_PLAN.md) runs unchanged on the .bshdat packages it loads, so the
// shim never selects a variant itself; the packages, keys and defines are kept for tools and
// diagnostics.

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "nvn_shim/nvn_types.h"

namespace dread::nvn {

// A uniform block or a sampler of one stage. nvn_slot is the stage's NVN slot: for a block, the
// index nvnCommandBufferBindUniformBuffer binds (the compiler numbered the stage's active blocks
// 0, 1, 2, ... in declaration order); for a sampler, the first texture slot (an array of `count`
// takes count consecutive slots). It is -1 for a resource the NVN compiler found unused that the
// SPIR-V still declares: it gets a placeholder descriptor. vk_binding is the binding in the stage's
// descriptor set.
struct StageResource {
    std::int32_t nvn_slot;
    std::uint32_t vk_binding;
    std::uint32_t size_or_count;  // a block's size in bytes; a sampler's array length
    std::string name;
};

struct StageInterface {
    std::int32_t location;  // -1: a built-in
    std::string name;
};

struct StageInfo {
    Stage stage;
    std::string spirv_path;  // relative to the shader directory
    std::uint32_t spirv_size;
    std::uint64_t control_key;
    std::uint32_t control_size;
    std::vector<StageResource> blocks;
    std::vector<StageResource> samplers;
    std::vector<StageInterface> inputs;
    std::vector<StageInterface> outputs;
};

struct ProgramInfo {
    std::uint32_t key;
    std::uint32_t key2;   // the variant map's second word; the engine compares key & key2
    std::uint32_t index;  // the program's position in its package
    std::vector<std::string> defines;
    std::vector<StageInfo> stages;  // vertex, then fragment
};

struct NameId {
    std::uint32_t id;
    std::string name;
};

struct PackageInfo {
    std::string name;
    std::vector<NameId> attributes;
    std::vector<NameId> uniforms[2];
    std::vector<ProgramInfo> programs;
};

class ShaderManifest {
public:
    // The first bytes of a control section the key covers (tools/bshdat.py CONTROL_KEY_BYTES);
    // every control section is longer.
    static constexpr std::size_t kControlKeyBytes = 2048;

    // Reads <dir>/shaders.manifest. On failure returns false with a message in `error` (the line
    // and what was wrong), and the manifest is left empty.
    bool Load(const std::string& dir, std::string* error);

    // The stage whose control section starts at `control`. The caller vouches that at least
    // kControlKeyBytes are readable there (a control section always has more). Null when no
    // stage of the manifest has that key. The install proved that the key tells every distinct
    // control section of the dump apart, so a match needs no further check.
    const StageInfo* FindStage(const void* control) const;

    // FNV-1a 64 over the first kControlKeyBytes bytes, as tools/bshdat.py computes the key.
    static std::uint64_t ControlKey(const void* control);

    const std::string& dir() const { return dir_; }
    const std::string& game_version() const { return game_version_; }
    std::uint32_t vertex_set() const { return vertex_set_; }
    std::uint32_t fragment_set() const { return fragment_set_; }
    std::uint32_t sampler_base() const { return sampler_base_; }
    const std::vector<PackageInfo>& packages() const { return packages_; }

private:
    std::string dir_;
    std::string game_version_;
    std::uint32_t vertex_set_ = 0;
    std::uint32_t fragment_set_ = 1;
    std::uint32_t sampler_base_ = 16;
    std::vector<PackageInfo> packages_;
    // control key -> (package, program, stage) of the first stage with it; stages with one key
    // are the same compiled stage (tools/bshdat.py checks it).
    struct StageRef {
        std::uint32_t package, program, stage;
    };
    std::unordered_map<std::uint64_t, StageRef> by_control_;
};

}  // namespace dread::nvn
