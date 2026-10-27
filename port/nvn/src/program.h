#pragma once

// The Vulkan side of NVN programs (program.cpp), for the command buffer's draws.

#include <cstdint>
#include <map>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "nvn_shim/shader_manifest.h"
#include "nvn_shim/spirv_scan.h"

namespace dread::nvn::detail {

// A program's stages as Vulkan objects: per NVN stage, the manifest's stage, its shader module and
// descriptor set layout, and the pipeline layout over both sets (at the manifest's set indices).
// Shared by every program with the same stages and kept until the device goes, so a pointer to one
// is a stable pipeline-cache key.
struct ProgramObjects {
    const StageInfo* stages[2] = {};  // vertex, fragment; null where the program has no such stage
    VkShaderModule modules[2] = {};
    VkDescriptorSetLayout set_layouts[2] = {};
    VkPipelineLayout layout = VK_NULL_HANDLE;
    std::vector<InterfaceVar> vertex_inputs;     // what a pipeline's vertex input must feed
    std::vector<InterfaceVar> fragment_outputs;  // the colour targets the fragment stage writes
    std::map<std::uint32_t, ImageKind> samplers[2];  // per stage: binding -> what its sampler samples
};

// The program the game initialised at `program` with nvnProgramSetShaders; null if it has none (or
// its stages were not found). Under StateLock().
const ProgramObjects* FindProgramObjects(const void* program);

// The loaded manifest (after a successful nvnProgramSetShaders), else null.
const ShaderManifest* LoadedManifest();

// Destroys the modules and layouts (nvnDeviceFinalize).
void ReleaseProgramVulkanObjects();

}  // namespace dread::nvn::detail
