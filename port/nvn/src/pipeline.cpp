// Vulkan pipelines for the game's draws (pipeline.h), and NVN's drawing numbers in Vulkan's terms.
// The meaning of every NVN number used here, and the evidence for it, is in nvn_shim/draw.h.

#include "pipeline.h"

#include <cstdio>
#include <cstring>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

#include <vulkan/vulkan.h>

#include "nvn_shim/device.h"
#include "nvn_shim/draw.h"
#include "nvn_shim/shader_manifest.h"

namespace dread::nvn::detail {

namespace {

struct KeyHash {
    std::size_t operator()(const PipelineKey& k) const {
        // FNV-1a over the key's bytes (zero-initialised, so padding is zero).
        const auto* p = reinterpret_cast<const unsigned char*>(&k);
        std::uint64_t h = 0xcbf29ce484222325ull;
        for (std::size_t i = 0; i < sizeof(k); ++i)
            h = (h ^ p[i]) * 0x100000001b3ull;
        return static_cast<std::size_t>(h);
    }
};
struct KeyEqual {
    bool operator()(const PipelineKey& a, const PipelineKey& b) const { return std::memcmp(&a, &b, sizeof(a)) == 0; }
};

std::unordered_map<PipelineKey, VkPipeline, KeyHash, KeyEqual> g_pipelines;
std::uint64_t g_made = 0;
std::set<std::pair<int, std::int32_t>> g_reported;  // (what, value) reported unknown

void Unknown(int what, const char* name, std::int32_t value) {
    if (g_reported.insert({what, value}).second)
        std::fprintf(stderr, "nvn shim: %s %d is not known (nvn_shim/draw.h); a default stands in\n", name, value);
}

VkBlendFactor BlendFactorOf(std::int32_t f) {
    switch (f) {
    case kBlendZero: return VK_BLEND_FACTOR_ZERO;
    case kBlendOne: return VK_BLEND_FACTOR_ONE;
    case kBlendSrcColor: return VK_BLEND_FACTOR_SRC_COLOR;
    case kBlendOneMinusSrcColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case kBlendSrcAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
    case kBlendOneMinusSrcAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case kBlendDstAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
    case kBlendOneMinusDstAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case kBlendDstColor: return VK_BLEND_FACTOR_DST_COLOR;
    case kBlendOneMinusDstColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    default: Unknown(0, "blend factor", f); return VK_BLEND_FACTOR_ONE;
    }
}

VkBlendOp BlendOpOf(std::int32_t e) {
    switch (e) {
    case kBlendAdd: return VK_BLEND_OP_ADD;
    case kBlendSubtract: return VK_BLEND_OP_SUBTRACT;
    case kBlendReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
    case kBlendMin: return VK_BLEND_OP_MIN;
    case kBlendMax: return VK_BLEND_OP_MAX;
    default: Unknown(1, "blend equation", e); return VK_BLEND_OP_ADD;
    }
}

// The representative topology of a class: the primitive itself is dynamic state.
VkPrimitiveTopology ClassTopology(std::int32_t topology_class) {
    switch (topology_class) {
    case 0: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case 1: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    default: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    }
}

// The format a placeholder input is fed with: four components of the input's kind (Vulkan fills or
// drops components the input does not have).
VkFormat PlaceholderFormat(ScalarKind kind) {
    switch (kind) {
    case ScalarKind::Int: return VK_FORMAT_R32G32B32A32_SINT;
    case ScalarKind::Uint: return VK_FORMAT_R32G32B32A32_UINT;
    default: return VK_FORMAT_R32G32B32A32_SFLOAT;
    }
}

VkPipeline MakePipeline(const Backend& be, const PipelineKey& key) {
    const ProgramObjects& prog = *key.program;
    const ShaderManifest* manifest = LoadedManifest();
    if (!manifest)
        return VK_NULL_HANDLE;

    VkPipelineShaderStageCreateInfo stages[2]{};
    std::uint32_t stage_count = 0;
    for (int i = 0; i < 2; ++i) {
        if (!prog.modules[i] || (i == 1 && key.vertex_only))
            continue;
        VkPipelineShaderStageCreateInfo& s = stages[stage_count++];
        s.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        s.stage = i == 0 ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT;
        s.module = prog.modules[i];
        s.pName = "main";
    }
    const bool fragment = stage_count == 2;

    // Vertex input: every location the vertex stage reads, from the attribute the game set there,
    // else from the zero buffer at kPlaceholderBinding. Strides are dynamic (vkCmdBindVertexBuffers2).
    std::vector<VkVertexInputAttributeDescription> attributes;
    std::uint32_t used_streams = 0;
    bool placeholder = false;
    for (const InterfaceVar& v : prog.vertex_inputs) {
        for (std::uint32_t l = v.location; l < v.location + v.locations; ++l) {
            VkVertexInputAttributeDescription a{};
            a.location = l;
            const PipelineKey::Attrib* fed = l < kVertexAttribs ? &key.attribs[l] : nullptr;
            if (fed && fed->format && fed->stream < key.stream_count && v.kind == ScalarKind::Float) {
                a.binding = fed->stream;
                a.format = VertexFormat(fed->format);
                a.offset = fed->offset;
                used_streams |= 1u << fed->stream;
            } else {
                a.binding = kPlaceholderBinding;
                a.format = PlaceholderFormat(v.kind);
                placeholder = true;
            }
            attributes.push_back(a);
        }
    }
    std::vector<VkVertexInputBindingDescription> bindings;
    for (std::uint32_t s = 0; s < key.stream_count; ++s)
        if (used_streams & (1u << s))
            bindings.push_back({s, 0, (key.instanced_streams >> s) & 1 ? VK_VERTEX_INPUT_RATE_INSTANCE
                                                                       : VK_VERTEX_INPUT_RATE_VERTEX});
    if (placeholder)
        bindings.push_back({kPlaceholderBinding, 0, VK_VERTEX_INPUT_RATE_VERTEX});
    VkPipelineVertexInputStateCreateInfo vis{VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
    vis.vertexBindingDescriptionCount = static_cast<std::uint32_t>(bindings.size());
    vis.pVertexBindingDescriptions = bindings.data();
    vis.vertexAttributeDescriptionCount = static_cast<std::uint32_t>(attributes.size());
    vis.pVertexAttributeDescriptions = attributes.data();

    VkPipelineInputAssemblyStateCreateInfo ias{VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
    ias.topology = ClassTopology(key.topology_class);
    // The game's -1..1 depth range (draw.h).
    VkPipelineViewportDepthClipControlCreateInfoEXT clip{
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_DEPTH_CLIP_CONTROL_CREATE_INFO_EXT};
    clip.negativeOneToOne = VK_TRUE;
    VkPipelineViewportStateCreateInfo vps{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
    vps.pNext = be.depth_clip_control ? &clip : nullptr;
    vps.viewportCount = 1;
    vps.scissorCount = 1;
    VkPipelineRasterizationStateCreateInfo rs{VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
    rs.polygonMode = key.polygon_mode == kPolygonLine && be.fill_mode_non_solid ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
    rs.lineWidth = 1.0f;
    VkPipelineMultisampleStateCreateInfo ms{VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
    ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    VkPipelineDepthStencilStateCreateInfo ds{VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
    ds.maxDepthBounds = 1.0f;

    // Colour targets: the game's blend and channel mask, nothing written where no fragment output
    // goes (Vulkan leaves such a target undefined otherwise) or without a fragment stage.
    std::vector<VkPipelineColorBlendAttachmentState> blend(key.color_count);
    for (std::uint32_t i = 0; i < key.color_count; ++i) {
        const PipelineKey::Blend& b = key.blend[be.independent_blend ? i : 0];
        bool written = false;
        if (fragment)
            for (const InterfaceVar& o : prog.fragment_outputs)
                written |= i >= o.location && i < o.location + o.locations;
        VkPipelineColorBlendAttachmentState& a = blend[i];
        a.colorWriteMask = written && key.colors[i] != VK_FORMAT_UNDEFINED ? b.mask : 0;
        if (!be.independent_blend && i > 0)
            a.colorWriteMask = blend[0].colorWriteMask;  // every attachment alike without the feature
        a.blendEnable = b.enable ? VK_TRUE : VK_FALSE;
        a.srcColorBlendFactor = BlendFactorOf(b.src);
        a.dstColorBlendFactor = BlendFactorOf(b.dst);
        a.colorBlendOp = BlendOpOf(b.color_equation);
        a.srcAlphaBlendFactor = BlendFactorOf(b.src_alpha);
        a.dstAlphaBlendFactor = BlendFactorOf(b.dst_alpha);
        a.alphaBlendOp = BlendOpOf(b.alpha_equation);
    }
    VkPipelineColorBlendStateCreateInfo cbs{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
    cbs.attachmentCount = key.color_count;
    cbs.pAttachments = blend.data();

    std::vector<VkDynamicState> dynamic = {
        VK_DYNAMIC_STATE_VIEWPORT,          VK_DYNAMIC_STATE_SCISSOR,           VK_DYNAMIC_STATE_CULL_MODE,
        VK_DYNAMIC_STATE_FRONT_FACE,        VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY, VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE,
        VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE, VK_DYNAMIC_STATE_DEPTH_COMPARE_OP, VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE,
        VK_DYNAMIC_STATE_STENCIL_OP,        VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK, VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
        VK_DYNAMIC_STATE_STENCIL_REFERENCE, VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE,
    };
    if (be.depth_bounds) {
        dynamic.push_back(VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE);
        dynamic.push_back(VK_DYNAMIC_STATE_DEPTH_BOUNDS);
    }
    VkPipelineDynamicStateCreateInfo dys{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
    dys.dynamicStateCount = static_cast<std::uint32_t>(dynamic.size());
    dys.pDynamicStates = dynamic.data();

    VkPipelineRenderingCreateInfo rci{VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
    rci.colorAttachmentCount = key.color_count;
    rci.pColorAttachmentFormats = key.colors;
    rci.depthAttachmentFormat = key.depth;
    rci.stencilAttachmentFormat = key.depth;  // the depth formats the shim makes all have stencil

    VkGraphicsPipelineCreateInfo gci{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
    gci.pNext = &rci;
    gci.stageCount = stage_count;
    gci.pStages = stages;
    gci.pVertexInputState = &vis;
    gci.pInputAssemblyState = &ias;
    gci.pViewportState = &vps;
    gci.pRasterizationState = &rs;
    gci.pMultisampleState = &ms;
    gci.pDepthStencilState = &ds;
    gci.pColorBlendState = &cbs;
    gci.pDynamicState = &dys;
    gci.layout = prog.layout;
    VkPipeline pipeline = VK_NULL_HANDLE;
    if (vkCreateGraphicsPipelines(be.device, VK_NULL_HANDLE, 1, &gci, nullptr, &pipeline) != VK_SUCCESS)
        return VK_NULL_HANDLE;
    g_made++;
    return pipeline;
}

}  // namespace

void PipelineKey::Reset() {
    std::memset(this, 0, sizeof(*this));
}

VkPipeline GetPipeline(const PipelineKey& key) {
    auto it = g_pipelines.find(key);
    if (it != g_pipelines.end())
        return it->second;
    const Backend* be = CurrentBackend();
    VkPipeline p = be && key.program ? MakePipeline(*be, key) : VK_NULL_HANDLE;
    if (!p)
        std::fprintf(stderr, "nvn shim: no Vulkan pipeline for a draw (program stages %s, %s)\n",
                     key.program && key.program->stages[0] ? key.program->stages[0]->spirv_path.c_str() : "-",
                     key.program && key.program->stages[1] ? key.program->stages[1]->spirv_path.c_str() : "-");
    g_pipelines.emplace(key, p);  // a failure is not retried
    return p;
}

std::uint64_t PipelinesMade() {
    return g_made;
}

void ReleasePipelines() {
    const Backend* be = CurrentBackend();
    if (be)
        for (auto& kv : g_pipelines)
            if (kv.second)
                vkDestroyPipeline(be->device, kv.second, nullptr);
    g_pipelines.clear();
}

VkCompareOp CompareOp(std::int32_t f) {
    switch (f) {
    case kCompareNever: return VK_COMPARE_OP_NEVER;
    case kCompareLess: return VK_COMPARE_OP_LESS;
    case kCompareEqual: return VK_COMPARE_OP_EQUAL;
    case kCompareLessEqual: return VK_COMPARE_OP_LESS_OR_EQUAL;
    case kCompareGreater: return VK_COMPARE_OP_GREATER;
    case kCompareNotEqual: return VK_COMPARE_OP_NOT_EQUAL;
    case kCompareGreaterEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case kCompareAlways: return VK_COMPARE_OP_ALWAYS;
    default: Unknown(2, "comparison function", f); return VK_COMPARE_OP_ALWAYS;
    }
}

VkStencilOp StencilOperation(std::int32_t op) {
    switch (op) {
    case kStencilKeep: return VK_STENCIL_OP_KEEP;
    case kStencilZero: return VK_STENCIL_OP_ZERO;
    case kStencilReplace: return VK_STENCIL_OP_REPLACE;
    case kStencilIncrementWrap: return VK_STENCIL_OP_INCREMENT_AND_WRAP;
    case kStencilDecrementWrap: return VK_STENCIL_OP_DECREMENT_AND_WRAP;
    case kStencilInvert: return VK_STENCIL_OP_INVERT;
    case kStencilIncrementClamp: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
    case kStencilDecrementClamp: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
    default: Unknown(3, "stencil operation", op); return VK_STENCIL_OP_KEEP;
    }
}

VkCullModeFlags CullMode(std::int32_t face) {
    switch (face) {
    case kFaceNone: return VK_CULL_MODE_NONE;
    case kFaceBack: return VK_CULL_MODE_BACK_BIT;
    case kFaceFront: return VK_CULL_MODE_FRONT_BIT;
    case kFaceBoth: return VK_CULL_MODE_FRONT_AND_BACK;
    default: Unknown(4, "cull face", face); return VK_CULL_MODE_NONE;
    }
}

VkFrontFace FrontFace(std::int32_t face) {
    if (face != kFrontFaceCounterClockwise)
        Unknown(5, "front face", face);
    return face == kFrontFaceCounterClockwise ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
}

bool Topology(std::int32_t primitive, VkPrimitiveTopology* topology, std::int32_t* topology_class) {
    switch (primitive) {
    case kPrimitiveLines: *topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST; *topology_class = 1; return true;
    case kPrimitiveLineStrip: *topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP; *topology_class = 1; return true;
    case kPrimitiveTriangles: *topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; *topology_class = 2; return true;
    case kPrimitiveTriangleStrip: *topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP; *topology_class = 2; return true;
    case kPrimitiveTriangleFan: *topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN; *topology_class = 2; return true;
    default: Unknown(6, "primitive", primitive); return false;
    }
}

VkFormat VertexFormat(std::int32_t format) {
    switch (format) {
    case kVertexR32Float: return VK_FORMAT_R32_SFLOAT;
    case kVertexRG32Float: return VK_FORMAT_R32G32_SFLOAT;
    case kVertexRGB32Float: return VK_FORMAT_R32G32B32_SFLOAT;
    case kVertexRGBA32Float: return VK_FORMAT_R32G32B32A32_SFLOAT;
    case kVertexRGBA8Unorm: return VK_FORMAT_R8G8B8A8_UNORM;
    default: Unknown(7, "vertex attribute format", format); return VK_FORMAT_R32G32B32A32_SFLOAT;
    }
}

}  // namespace dread::nvn::detail
