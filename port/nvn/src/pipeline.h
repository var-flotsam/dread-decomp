#pragma once

// Vulkan pipelines for the game's draws (pipeline.cpp), and the translation of NVN's numbers
// (nvn_shim/draw.h) into Vulkan's. Under detail::StateLock().

#include <cstdint>

#include <vulkan/vulkan_core.h>

#include "draw_state.h"
#include "program.h"

namespace dread::nvn::detail {

// What a pipeline is made from: everything of the draw's state that Vulkan cannot set dynamically.
// The depth and stencil tests, cull mode, front face, viewport, scissor and the primitive (within its
// class) are dynamic state (Vulkan 1.3), set per draw. Zero-initialised and compared byte for byte:
// fill one with Reset() and set only the fields that apply (MakeKey in command.cpp normalises).
struct PipelineKey {
    const ProgramObjects* program;
    std::uint32_t vertex_only;  // the depth-only pass unbinds the fragment stage (fn_710015d900)
    std::uint32_t color_count;
    VkFormat colors[kColorTargets];  // VK_FORMAT_UNDEFINED for an empty target
    VkFormat depth;
    std::int32_t topology_class;  // 0 points, 1 lines, 2 triangles
    std::int32_t polygon_mode;
    struct Blend {
        std::uint32_t enable, mask;
        std::int32_t src, dst, src_alpha, dst_alpha, color_equation, alpha_equation;
    } blend[kColorTargets];
    struct Attrib {
        std::int32_t format;  // 0: not fed by the game (a shader input there reads zeros)
        std::uint32_t stream;
        std::uint32_t offset;
    } attribs[kVertexAttribs];
    std::uint32_t stream_count;
    std::uint32_t instanced_streams;  // bit s: stream s advances per instance

    void Reset();
};

// The pipeline for `key`, made on first use; VK_NULL_HANDLE if it cannot be (reported once).
VkPipeline GetPipeline(const PipelineKey& key);
std::uint64_t PipelinesMade();
void ReleasePipelines();

// The vertex input binding of the zero buffer that feeds a shader input no attribute does.
constexpr std::uint32_t kPlaceholderBinding = kVertexStreams;

// NVN numbers to Vulkan; an unknown number gives a harmless default and is reported once.
VkCompareOp CompareOp(std::int32_t func);
VkStencilOp StencilOperation(std::int32_t op);
VkCullModeFlags CullMode(std::int32_t face);
VkFrontFace FrontFace(std::int32_t face);
// The topology of an NVN primitive, and its class (PipelineKey::topology_class); false if unknown.
bool Topology(std::int32_t primitive, VkPrimitiveTopology* topology, std::int32_t* topology_class);
VkFormat VertexFormat(std::int32_t format);

}  // namespace dread::nvn::detail
