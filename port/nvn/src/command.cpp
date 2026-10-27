// NVN command buffers, recorded into Vulkan command buffers. Signatures are those of the game's
// calls: fn_71001587f0 for Initialize, AddCommandMemory, AddControlMemory, SetMemoryCallback,
// SetViewport and SetScissor; the frame's begin fn_7100159920 for BeginRecording, SetSampleMask and
// (through fn_710015c2d0) SetRenderTargets; its end fn_7100159ac0 for EndRecording, whose handle goes
// to nvnQueueSubmitCommands; the program binding fn_710015d900 for BindProgram and
// BindUniformBuffer; fn_7100161250 for BindTexture; the render-state flush fn_7100159ce0 for the
// Bind*State calls and SetAlphaRef; the clears fn_7100159660; the vertex setup fn_7100161800 and
// fn_7100162330 for BindVertexAttribState, BindVertexStreamState and BindVertexBuffer; the draws
// fn_7100159f90 (DrawElements) and fn_710015ac00 (DrawElementsInstanced), which also set the depth
// bounds; the stencil setters fn_710015acf0, fn_710015b160, fn_710015b650 and fn_710015b8a0 for
// SetStencilMask and SetStencilRef; the uniform flush fn_710015e700 for UpdateUniformBuffer;
// fn_710015c3f0 for Barrier; fn_710015e980 for SetShaderScratchMemory. What each number means is in
// nvn_shim/draw.h; port/nvn/README.md, "Drawing", has the design. In short:
//
//  - The state the game sets and binds is the queue's, not one command buffer's: the engine binds a
//    state only when it changed (its render-state flags, fn_7100159ce0) and records each frame into
//    one of two command buffers in turn, so what one recording bound holds in the next. The shim
//    keeps one such state (g_state), as the console's GPU does. The game records on one thread.
//  - Each recording is a Vulkan command buffer. A draw makes or finds the pipeline for the state
//    (pipeline.cpp), sets the dynamic state, writes the stages' descriptor sets from what the game
//    bound (the binding layer, bindings.h), binds the vertex streams and the index buffer, and draws.
//  - Rendering begins at the first draw or clear after nvnCommandBufferSetRenderTargets and ends at
//    the next SetRenderTargets, Barrier or EndRecording, each of which is followed by a full memory
//    barrier: images keep one layout (texture_internal.h), so the barriers order a target's writes
//    before a later pass samples it.
//  - nvnCommandBufferUpdateUniformBuffer changes a uniform buffer in command order without touching
//    the game's memory. The shim keeps the buffer's contents as the updates leave them (a shadow) and
//    points each later draw's descriptor at a copy taken at that point in the recording.
//  - A recording's Vulkan objects (command buffer, descriptor pools, uniform copies) are reused once
//    the GPU has finished the submit that used them (the queue's timeline, device.h).

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <deque>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "draw_state.h"
#include "nvn_shim/bindings.h"
#include "nvn_shim/command.h"
#include "nvn_shim/device.h"
#include "nvn_shim/draw.h"
#include "nvn_shim/memory.h"
#include "nvn_shim/procs.h"
#include "nvn_shim/texture.h"
#include "objects.h"
#include "pipeline.h"
#include "program.h"
#include "texture_internal.h"

namespace dread::nvn {

namespace {

using detail::kColorTargets;
using detail::kVertexAttribs;
using detail::kVertexStreams;

// --- the queue's state, as the game sets and binds it ---

struct VertexBufferBinding {
    BufferAddress address = 0;
    std::uint64_t size = 0;
};

struct GpuState {
    const void* program = nullptr;
    std::int32_t program_stages = 0;
    BindingState bindings;
    detail::BlendTargetState blend[kColorTargets];
    detail::ColorState color;
    detail::ChannelMaskState channel_mask;
    detail::DepthStencilState depth_stencil;
    detail::MultisampleState multisample;
    detail::PolygonState polygon;
    detail::VertexAttribState attribs[kVertexAttribs];
    std::uint32_t attrib_count = 0;
    detail::VertexStreamState streams[kVertexStreams];
    std::uint32_t stream_count = 0;
    VertexBufferBinding vertex_buffers[kVertexStreams];
    std::int32_t viewport[4] = {};
    std::int32_t scissor[4] = {};
    std::int32_t sample_mask = -1;
    float alpha_ref = 0.0f;
    std::uint32_t stencil_write_mask[2] = {0xff, 0xff};  // front, back
    std::uint32_t stencil_ref[2] = {};
    bool depth_bounds = false;
    float depth_bounds_min = 0.0f, depth_bounds_max = 1.0f;
    const void* colors[kColorTargets] = {};
    std::uint32_t color_count = 0;
    const void* depth = nullptr;
    const void* texture_pool = nullptr;
    const void* sampler_pool = nullptr;
    const void* scratch_pool = nullptr;
    std::int64_t scratch_offset = 0, scratch_size = 0;
};
GpuState g_state;

// The uniform buffers nvnCommandBufferUpdateUniformBuffer changed: their contents as the updates
// left them (header comment).
struct Shadow {
    std::vector<std::uint8_t> data;
    std::uint64_t version = 0;
};
std::map<BufferAddress, Shadow> g_shadows;  // by the buffer's GPU address

// --- Vulkan objects a recording uses ---

constexpr VkDeviceSize kUploadChunkSize = 1 << 20;
constexpr std::uint32_t kSetsPerPool = 256;

struct UploadChunk {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    std::uint8_t* mapped = nullptr;
    VkDeviceSize used = 0;
};

struct Recording {
    VkCommandBuffer vk = VK_NULL_HANDLE;
    std::vector<VkDescriptorPool> pools;
    std::vector<UploadChunk> chunks;
    std::vector<std::pair<BufferAddress, std::uint64_t>> reads;  // for pools that are copies (memory.h)
    std::map<std::pair<BufferAddress, std::uint64_t>, std::pair<VkBuffer, VkDeviceSize>> shadow_copies;
    std::uint64_t retire = 0;  // the timeline value of the submit that used it
};

VkCommandPool g_pool = VK_NULL_HANDLE;
std::vector<VkCommandBuffer> g_free_command_buffers;
std::vector<VkDescriptorPool> g_free_pools;
std::vector<UploadChunk> g_free_chunks;
std::map<std::uint64_t, Recording> g_recorded;  // ended, not submitted yet, by handle
std::deque<Recording> g_in_flight;              // submitted, by timeline value
DrawStats g_stats;
std::set<std::string> g_reported;

void ReportOnce(const std::string& what) {
    if (g_reported.insert(what).second)
        std::fprintf(stderr, "nvn shim: %s\n", what.c_str());
}

// --- command buffers ---

struct CommandBuffer {
    const void* device = nullptr;
    const void* command_pool = nullptr;
    std::int64_t command_offset = 0;
    std::uint64_t command_size = 0;
    void* control_memory = nullptr;
    std::uint64_t control_size = 0;
    void* memory_callback = nullptr;
    bool recording = false;
    std::uint64_t recordings = 0;
    // The recording in progress.
    Recording rec;
    bool rendering = false;
    VkRect2D render_area{};
    VkPipeline bound_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout bound_layout = VK_NULL_HANDLE;
    std::uint64_t bound_shadow_version = 0;
};

detail::Registry<CommandBuffer> g_commands;
std::uint64_t g_next_handle = 1;
std::uint64_t g_shadow_version = 0;  // bumped by every uniform update: descriptors are rewritten

void FreeRecording(const Backend& be, Recording& r) {
    if (r.vk) {
        vkResetCommandBuffer(r.vk, 0);
        g_free_command_buffers.push_back(r.vk);
    }
    for (VkDescriptorPool p : r.pools) {
        vkResetDescriptorPool(be.device, p, 0);
        g_free_pools.push_back(p);
    }
    for (UploadChunk& c : r.chunks) {
        c.used = 0;
        g_free_chunks.push_back(c);
    }
    r = Recording();
}

// Reuses the objects of every submit the GPU has finished.
void Recycle(const Backend& be) {
    const std::uint64_t done = Completed();
    while (!g_in_flight.empty() && g_in_flight.front().retire <= done) {
        FreeRecording(be, g_in_flight.front());
        g_in_flight.pop_front();
    }
}

VkDescriptorPool NewDescriptorPool(const Backend& be) {
    if (!g_free_pools.empty()) {
        VkDescriptorPool p = g_free_pools.back();
        g_free_pools.pop_back();
        return p;
    }
    // Room for the largest stage of the game: five blocks and 13 samplers (nvn_types.h), with
    // placeholders for declared but unused resources.
    const VkDescriptorPoolSize sizes[] = {
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, kSetsPerPool * 16},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, kSetsPerPool * 32},
    };
    VkDescriptorPoolCreateInfo dpi{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    dpi.maxSets = kSetsPerPool;
    dpi.poolSizeCount = 2;
    dpi.pPoolSizes = sizes;
    VkDescriptorPool p = VK_NULL_HANDLE;
    if (vkCreateDescriptorPool(be.device, &dpi, nullptr, &p) != VK_SUCCESS)
        return VK_NULL_HANDLE;
    return p;
}

VkDescriptorSet AllocateSet(const Backend& be, Recording& r, VkDescriptorSetLayout layout) {
    VkDescriptorSetAllocateInfo ai{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
    ai.descriptorSetCount = 1;
    ai.pSetLayouts = &layout;
    VkDescriptorSet set = VK_NULL_HANDLE;
    if (!r.pools.empty()) {
        ai.descriptorPool = r.pools.back();
        if (vkAllocateDescriptorSets(be.device, &ai, &set) == VK_SUCCESS)
            return set;
    }
    VkDescriptorPool p = NewDescriptorPool(be);
    if (!p)
        return VK_NULL_HANDLE;
    r.pools.push_back(p);
    ai.descriptorPool = p;
    return vkAllocateDescriptorSets(be.device, &ai, &set) == VK_SUCCESS ? set : VK_NULL_HANDLE;
}

bool NewUploadChunk(const Backend& be, UploadChunk* c) {
    if (!g_free_chunks.empty()) {
        *c = g_free_chunks.back();
        g_free_chunks.pop_back();
        return true;
    }
    VkBufferCreateInfo bci{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    bci.size = kUploadChunkSize;
    bci.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    if (vkCreateBuffer(be.device, &bci, nullptr, &c->buffer) != VK_SUCCESS)
        return false;
    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(be.device, c->buffer, &req);
    VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mai.allocationSize = req.size;
    mai.memoryTypeIndex =
        be.MemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* mapped = nullptr;
    if (mai.memoryTypeIndex == UINT32_MAX || vkAllocateMemory(be.device, &mai, nullptr, &c->memory) != VK_SUCCESS ||
        vkBindBufferMemory(be.device, c->buffer, c->memory, 0) != VK_SUCCESS ||
        vkMapMemory(be.device, c->memory, 0, VK_WHOLE_SIZE, 0, &mapped) != VK_SUCCESS) {
        if (c->memory)
            vkFreeMemory(be.device, c->memory, nullptr);
        vkDestroyBuffer(be.device, c->buffer, nullptr);
        *c = UploadChunk();
        return false;
    }
    c->mapped = static_cast<std::uint8_t*>(mapped);
    c->used = 0;
    return true;
}

// A copy of `size` bytes in the recording's upload memory; false if there is none.
bool Upload(const Backend& be, Recording& r, const void* data, VkDeviceSize size, VkBuffer* buffer, VkDeviceSize* offset) {
    const VkDeviceSize align = std::max<VkDeviceSize>(be.properties.limits.minUniformBufferOffsetAlignment, 16);
    if (size > kUploadChunkSize)
        return false;
    if (r.chunks.empty() || (r.chunks.back().used + align - 1) / align * align + size > kUploadChunkSize) {
        UploadChunk c;
        if (!NewUploadChunk(be, &c))
            return false;
        r.chunks.push_back(c);
    }
    UploadChunk& c = r.chunks.back();
    const VkDeviceSize at = (c.used + align - 1) / align * align;
    std::memcpy(c.mapped + at, data, static_cast<std::size_t>(size));
    c.used = at + size;
    *buffer = c.buffer;
    *offset = at;
    return true;
}

// The shadow holding [address, address + size), if any.
std::pair<BufferAddress, Shadow*> ShadowFor(BufferAddress address, std::uint64_t size) {
    auto it = g_shadows.upper_bound(address);
    if (it == g_shadows.begin())
        return {0, nullptr};
    --it;
    if (address + size > it->first + it->second.data.size())
        return {0, nullptr};
    return {it->first, &it->second};
}

// The binding layer's resolver for a draw: uniform buffers the game updated in command order come
// from a copy of their shadow; everything else from the shim's objects (texture.h's ShimResolver).
class DrawResolver final : public ResourceResolver {
public:
    DrawResolver(const Backend& be, Recording& r) : be_(be), r_(r) {}
    bool ResolveBuffer(BufferAddress address, std::uint64_t size, VkBuffer* buffer, VkDeviceSize* offset) override {
        auto [base, shadow] = ShadowFor(address, size);
        if (!shadow) {
            r_.reads.emplace_back(address, size);
            return ShimResolver().ResolveBuffer(address, size, buffer, offset);
        }
        const auto key = std::make_pair(base, shadow->version);
        auto it = r_.shadow_copies.find(key);
        if (it == r_.shadow_copies.end()) {
            std::pair<VkBuffer, VkDeviceSize> copy;
            if (!Upload(be_, r_, shadow->data.data(), shadow->data.size(), &copy.first, &copy.second))
                return false;
            it = r_.shadow_copies.emplace(key, copy).first;
        }
        *buffer = it->second.first;
        *offset = it->second.second + (address - base);
        return true;
    }
    bool ResolveTexture(std::uint32_t id, VkImageView* view, VkImageLayout* layout) override {
        return ShimResolver().ResolveTexture(id, view, layout);
    }
    bool ResolveSampler(std::uint32_t id, VkSampler* sampler) override { return ShimResolver().ResolveSampler(id, sampler); }
    VkDescriptorBufferInfo PlaceholderBuffer() override { return ShimResolver().PlaceholderBuffer(); }
    VkDescriptorImageInfo PlaceholderImage() override { return ShimResolver().PlaceholderImage(); }

private:
    const Backend& be_;
    Recording& r_;
};

// Every sampler's descriptor must be of the kind the shader declares (Vulkan requires a cube view
// for a samplerCube and a depth format for a sampler2DShadow): an empty or mismatched slot gets the
// placeholder of the right kind (texture_internal.h). A depth view serves a sampler2D.
void MatchImageKinds(const std::map<std::uint32_t, ImageKind>& samplers, DescriptorWrites* w) {
    for (const VkWriteDescriptorSet& write : w->writes) {
        if (write.descriptorType != VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER || !write.pImageInfo)
            continue;
        auto it = samplers.find(write.dstBinding);
        const ImageKind want = it == samplers.end() ? ImageKind::Image2D : it->second;
        const std::size_t first = static_cast<std::size_t>(write.pImageInfo - w->images.data());
        for (std::uint32_t k = 0; k < write.descriptorCount; ++k) {
            VkDescriptorImageInfo& info = w->images[first + k];
            detail::ViewKind have;
            const bool known = detail::ViewKindOf(info.imageView, &have);
            const bool fits = known && (want == ImageKind::ImageCube       ? have == detail::ViewKind::Cube
                                        : want == ImageKind::Image2DShadow ? have == detail::ViewKind::Depth2D
                                                                           : have != detail::ViewKind::Cube);
            if (!fits)
                info = detail::PlaceholderImageFor(want);
        }
    }
}

void FullBarrier(VkCommandBuffer vk) {
    VkMemoryBarrier mb{VK_STRUCTURE_TYPE_MEMORY_BARRIER};
    mb.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
    mb.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
    vkCmdPipelineBarrier(vk, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 1, &mb, 0, nullptr,
                         0, nullptr);
    g_stats.barriers++;
}

void EndRendering(CommandBuffer& c) {
    if (!c.rendering)
        return;
    vkCmdEndRendering(c.rec.vk);
    c.rendering = false;
    FullBarrier(c.rec.vk);
}

// Begins rendering into the bound render targets; false (reported once) if there are none.
bool BeginRendering(CommandBuffer& c) {
    if (c.rendering)
        return true;
    VkRenderingAttachmentInfo colors[kColorTargets]{};
    VkRenderingAttachmentInfo depth{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
    std::uint32_t width = UINT32_MAX, height = UINT32_MAX;
    bool any = false, has_depth = false;
    for (std::uint32_t i = 0; i < g_state.color_count; ++i) {
        colors[i].sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        detail::AttachmentInfo a;
        if (!g_state.colors[i] || !detail::TextureAttachment(g_state.colors[i], &a) || a.depth)
            continue;
        colors[i].imageView = a.view;
        colors[i].imageLayout = detail::kImageLayout;
        colors[i].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        colors[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        width = std::min(width, a.width);
        height = std::min(height, a.height);
        any = true;
    }
    detail::AttachmentInfo d;
    if (g_state.depth && detail::TextureAttachment(g_state.depth, &d) && d.depth) {
        depth.imageView = d.view;
        depth.imageLayout = detail::kImageLayout;
        depth.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        depth.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        width = std::min(width, d.width);
        height = std::min(height, d.height);
        any = has_depth = true;
    }
    if (!any) {
        ReportOnce("a draw or clear without render targets (skipped)");
        return false;
    }
    VkRenderingInfo ri{VK_STRUCTURE_TYPE_RENDERING_INFO};
    ri.renderArea = {{0, 0}, {width, height}};
    ri.layerCount = 1;
    ri.colorAttachmentCount = g_state.color_count;
    ri.pColorAttachments = colors;
    ri.pDepthAttachment = has_depth ? &depth : nullptr;
    ri.pStencilAttachment = has_depth && d.stencil ? &depth : nullptr;
    vkCmdBeginRendering(c.rec.vk, &ri);
    c.rendering = true;
    c.render_area = ri.renderArea;
    g_stats.render_passes++;
    return true;
}

// The command buffer being recorded at `cmd`, else null (reported once).
CommandBuffer* ActiveCommandBuffer(void* cmd, const char* what) {
    CommandBuffer* c = g_commands.Find(cmd);
    if (!c || !c->recording || !c->rec.vk) {
        ReportOnce(std::string(what) + " outside a recording (ignored)");
        return nullptr;
    }
    return c;
}

VkFormat AttachmentFormat(const void* texture, bool want_depth) {
    detail::AttachmentInfo a;
    if (!texture || !detail::TextureAttachment(texture, &a) || a.depth != want_depth)
        return VK_FORMAT_UNDEFINED;
    return a.format;
}

// The pipeline key of the current state for a draw of `topology_class`.
void MakeKey(const detail::ProgramObjects* prog, std::int32_t topology_class, detail::PipelineKey* key) {
    key->Reset();
    key->program = prog;
    key->vertex_only = !(g_state.program_stages & kStageBitFragment) || !prog->modules[1];
    key->color_count = g_state.color_count;
    for (std::uint32_t i = 0; i < g_state.color_count; ++i) {
        key->colors[i] = AttachmentFormat(g_state.colors[i], false);
        // Blending off unless the target is drawn to and the game enabled it (one key for all
        // states that draw alike).
        detail::PipelineKey::Blend& b = key->blend[i];
        b.src = b.src_alpha = kBlendOne;
        b.dst = b.dst_alpha = kBlendZero;
        b.color_equation = b.alpha_equation = kBlendAdd;
        if (key->colors[i] == VK_FORMAT_UNDEFINED || key->vertex_only)
            continue;
        b.mask = g_state.channel_mask.mask[i];
        b.enable = g_state.color.blend[i];
        if (b.enable) {
            const detail::BlendTargetState& s = g_state.blend[i];
            b.src = s.src;
            b.dst = s.dst;
            b.src_alpha = s.src_alpha;
            b.dst_alpha = s.dst_alpha;
            b.color_equation = s.color_equation;
            b.alpha_equation = s.alpha_equation;
        }
    }
    key->depth = AttachmentFormat(g_state.depth, true);
    key->topology_class = topology_class;
    key->polygon_mode = g_state.polygon.polygon_mode;
    key->stream_count = std::min<std::uint32_t>(g_state.stream_count, kVertexStreams);
    for (std::uint32_t s = 0; s < key->stream_count; ++s)
        if (g_state.streams[s].divisor > 0)
            key->instanced_streams |= 1u << s;
    for (const InterfaceVar& v : prog->vertex_inputs) {
        for (std::uint32_t l = v.location; l < v.location + v.locations && l < kVertexAttribs; ++l) {
            if (l >= g_state.attrib_count)
                continue;
            const detail::VertexAttribState& a = g_state.attribs[l];
            key->attribs[l].format = a.format;
            key->attribs[l].stream = static_cast<std::uint32_t>(a.stream);
            key->attribs[l].offset = static_cast<std::uint32_t>(a.relative_offset);
        }
    }
}

std::uint32_t IndexSize(std::int32_t type) {
    return type == kIndexUint8 ? 1 : type == kIndexUint16 ? 2 : 4;
}

void Draw(void* cmd, std::int32_t primitive, std::int32_t index_type, std::int32_t count, BufferAddress indices,
          std::int32_t base_vertex, std::uint32_t base_instance, std::uint32_t instances) {
    const Backend* be = CurrentBackend();
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    CommandBuffer* c = ActiveCommandBuffer(cmd, "a draw");
    if (!be || !c || count <= 0 || instances == 0)
        return;
    auto skip = [](const char* why) {
        ReportOnce(std::string("a draw ") + why + " (skipped)");
        g_stats.skipped_draws++;
    };
    const detail::ProgramObjects* prog = g_state.program ? detail::FindProgramObjects(g_state.program) : nullptr;
    VkPrimitiveTopology topology;
    std::int32_t topology_class;
    if (!prog)
        return skip("without a program the shim knows");
    if (!detail::Topology(primitive, &topology, &topology_class))
        return skip("of an unknown primitive");
    if (index_type < kIndexUint8 || index_type > kIndexUint32 || (index_type == kIndexUint8 && !be->index_uint8))
        return skip("with an index type the device cannot use");
    if (g_state.viewport[2] <= 0 || g_state.viewport[3] <= 0)
        return skip("with an empty viewport");
    VkBuffer index_buffer;
    VkDeviceSize index_offset;
    const std::uint64_t index_bytes = static_cast<std::uint64_t>(count) * IndexSize(index_type);
    if (!ShimResolver().ResolveBuffer(indices, index_bytes, &index_buffer, &index_offset))
        return skip("whose index buffer is in no memory pool");
    if (!BeginRendering(*c))
        return skip("without render targets");
    const VkCommandBuffer vk = c->rec.vk;

    detail::PipelineKey key;
    MakeKey(prog, topology_class, &key);
    const VkPipeline pipeline = detail::GetPipeline(key);
    if (!pipeline)
        return skip("whose pipeline could not be made");
    g_stats.pipelines = detail::PipelinesMade();
    if (pipeline != c->bound_pipeline) {
        vkCmdBindPipeline(vk, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
        c->bound_pipeline = pipeline;
    }

    // Dynamic state. The viewport is flipped (draw.h: row 0 is clip-space y = +1).
    const VkViewport viewport{static_cast<float>(g_state.viewport[0]),
                              static_cast<float>(g_state.viewport[1] + g_state.viewport[3]),
                              static_cast<float>(g_state.viewport[2]),
                              -static_cast<float>(g_state.viewport[3]),
                              0.0f,
                              1.0f};
    vkCmdSetViewport(vk, 0, 1, &viewport);
    const std::int32_t sx = std::max(g_state.scissor[0], 0), sy = std::max(g_state.scissor[1], 0);
    const VkRect2D scissor{{sx, sy},
                           {static_cast<std::uint32_t>(std::max(g_state.scissor[2] - (sx - g_state.scissor[0]), 0)),
                            static_cast<std::uint32_t>(std::max(g_state.scissor[3] - (sy - g_state.scissor[1]), 0))}};
    vkCmdSetScissor(vk, 0, 1, &scissor);
    vkCmdSetCullMode(vk, detail::CullMode(g_state.polygon.cull_face));
    vkCmdSetFrontFace(vk, detail::FrontFace(g_state.polygon.front_face));
    vkCmdSetPrimitiveTopology(vk, topology);
    const detail::DepthStencilState& ds = g_state.depth_stencil;
    const bool has_depth = key.depth != VK_FORMAT_UNDEFINED;
    vkCmdSetDepthTestEnable(vk, has_depth && ds.depth_test);
    vkCmdSetDepthWriteEnable(vk, has_depth && ds.depth_write);
    vkCmdSetDepthCompareOp(vk, detail::CompareOp(ds.depth_func));
    vkCmdSetStencilTestEnable(vk, has_depth && ds.stencil_test);
    for (int f = 0; f < 2; ++f) {
        const detail::StencilFaceState& s = f == 0 ? ds.front : ds.back;
        const VkStencilFaceFlags face = f == 0 ? VK_STENCIL_FACE_FRONT_BIT : VK_STENCIL_FACE_BACK_BIT;
        vkCmdSetStencilOp(vk, face, detail::StencilOperation(s.fail), detail::StencilOperation(s.depth_pass),
                          detail::StencilOperation(s.depth_fail), detail::CompareOp(s.func));
        vkCmdSetStencilCompareMask(vk, face, 0xff);  // nvnCommandBufferSetStencilValueMask is never called
        vkCmdSetStencilWriteMask(vk, face, g_state.stencil_write_mask[f]);
        vkCmdSetStencilReference(vk, face, g_state.stencil_ref[f]);
    }
    if (be->depth_bounds) {
        vkCmdSetDepthBoundsTestEnable(vk, has_depth && g_state.depth_bounds);
        vkCmdSetDepthBounds(vk, g_state.depth_bounds_min, g_state.depth_bounds_max);
    } else if (g_state.depth_bounds) {
        ReportOnce("the device has no depth bounds test; draws that ask for one run without it");
    }
    if (g_state.color.alpha_func != kCompareAlways)
        ReportOnce("the alpha test is not applied yet (port/nvn/README.md, \"Drawing\")");

    // Descriptor sets: rewritten when the bindings, the program's layout or a uniform update changed.
    const ShaderManifest* manifest = detail::LoadedManifest();
    const bool new_layout = prog->layout != c->bound_layout;
    const bool updated = c->bound_shadow_version != g_shadow_version;
    for (int i = 0; i < 2; ++i) {
        const Stage stage = static_cast<Stage>(i);
        if (!prog->stages[i] || (i == 1 && key.vertex_only))
            continue;
        if (!new_layout && !updated && !g_state.bindings.Dirty(stage))
            continue;
        const VkDescriptorSet set = AllocateSet(*be, c->rec, prog->set_layouts[i]);
        if (!set)
            return skip("without a descriptor set");
        DrawResolver resolver(*be, c->rec);
        DescriptorWrites writes;
        g_state.bindings.BuildWrites(*prog->stages[i], set, resolver, &writes);
        MatchImageKinds(prog->samplers[i], &writes);
        g_stats.descriptor_warnings += writes.warnings.size();
        vkUpdateDescriptorSets(be->device, static_cast<std::uint32_t>(writes.writes.size()), writes.writes.data(), 0,
                               nullptr);
        vkCmdBindDescriptorSets(vk, VK_PIPELINE_BIND_POINT_GRAPHICS, prog->layout, DescriptorSetIndex(*manifest, stage), 1,
                                &set, 0, nullptr);
        g_state.bindings.MarkClean(stage);
    }
    c->bound_layout = prog->layout;
    c->bound_shadow_version = g_shadow_version;

    // Vertex streams, and the zero buffer for inputs no attribute feeds.
    VkBuffer buffers[kVertexStreams + 1];
    VkDeviceSize offsets[kVertexStreams + 1], sizes[kVertexStreams + 1], strides[kVertexStreams + 1];
    const VkDescriptorBufferInfo zero = ShimResolver().PlaceholderBuffer();
    for (std::uint32_t s = 0; s <= kVertexStreams; ++s) {
        buffers[s] = zero.buffer;
        offsets[s] = 0;
        sizes[s] = zero.range;
        strides[s] = 0;
        if (s >= key.stream_count)
            continue;
        const VertexBufferBinding& vb = g_state.vertex_buffers[s];
        VkBuffer b;
        VkDeviceSize o;
        if (vb.size && ShimResolver().ResolveBuffer(vb.address, vb.size, &b, &o)) {
            buffers[s] = b;
            offsets[s] = o;
            sizes[s] = vb.size;
            strides[s] = static_cast<VkDeviceSize>(std::max(g_state.streams[s].stride, 0));
            c->rec.reads.emplace_back(vb.address, vb.size);
        }
    }
    vkCmdBindVertexBuffers2(vk, 0, kVertexStreams + 1, buffers, offsets, sizes, strides);
    vkCmdBindIndexBuffer(vk, index_buffer, index_offset,
                         index_type == kIndexUint8    ? VK_INDEX_TYPE_UINT8_KHR
                         : index_type == kIndexUint16 ? VK_INDEX_TYPE_UINT16
                                                      : VK_INDEX_TYPE_UINT32);
    c->rec.reads.emplace_back(indices, index_bytes);
    vkCmdDrawIndexed(vk, static_cast<std::uint32_t>(count), instances, 0, base_vertex, base_instance);
    g_stats.draws++;
}

// --- the NVN functions ---

std::int32_t CommandBufferInitialize(void* cmd, const void* device) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_commands.Reset(cmd).device = device;
    return 1;
}

void CommandBufferFinalize(void* cmd) {
    const Backend* be = CurrentBackend();
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (CommandBuffer* c = g_commands.Find(cmd)) {
        if (c->rec.vk && be) {  // finalised while recording: nothing of it was submitted
            if (c->rendering)
                vkCmdEndRendering(c->rec.vk);
            vkEndCommandBuffer(c->rec.vk);
            FreeRecording(*be, c->rec);
        }
    }
    g_commands.Erase(cmd);
}

void CommandBufferAddCommandMemory(void* cmd, const void* pool, std::int64_t offset, std::uint64_t size) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    CommandBuffer& c = g_commands.Get(cmd);
    c.command_pool = pool;
    c.command_offset = offset;
    c.command_size = size;
}

void CommandBufferAddControlMemory(void* cmd, void* memory, std::uint64_t size) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    CommandBuffer& c = g_commands.Get(cmd);
    c.control_memory = memory;
    c.control_size = size;
}

// Called by NVN when a command buffer runs out of memory; the shim's never does.
void CommandBufferSetMemoryCallback(void* cmd, void* callback) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_commands.Get(cmd).memory_callback = callback;
}

void CommandBufferBeginRecording(void* cmd) {
    const Backend* be = CurrentBackend();
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    CommandBuffer& c = g_commands.Get(cmd);
    if (!c.command_size || !c.control_size)
        std::fprintf(stderr, "nvn shim: nvnCommandBufferBeginRecording without command or control memory\n");
    c.recording = true;
    if (!be)
        return;
    if (c.rec.vk) {  // begun twice without an end: drop the first
        if (c.rendering)
            vkCmdEndRendering(c.rec.vk);
        vkEndCommandBuffer(c.rec.vk);
        FreeRecording(*be, c.rec);
    }
    Recycle(*be);
    if (!g_pool) {
        VkCommandPoolCreateInfo cpi{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
        cpi.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        cpi.queueFamilyIndex = be->queue_family;
        if (vkCreateCommandPool(be->device, &cpi, nullptr, &g_pool) != VK_SUCCESS) {
            g_pool = VK_NULL_HANDLE;
            return;
        }
    }
    if (!g_free_command_buffers.empty()) {
        c.rec.vk = g_free_command_buffers.back();
        g_free_command_buffers.pop_back();
    } else {
        VkCommandBufferAllocateInfo ai{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
        ai.commandPool = g_pool;
        ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        ai.commandBufferCount = 1;
        if (vkAllocateCommandBuffers(be->device, &ai, &c.rec.vk) != VK_SUCCESS) {
            c.rec.vk = VK_NULL_HANDLE;
            return;
        }
    }
    VkCommandBufferBeginInfo bi{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(c.rec.vk, &bi);
    // Work submitted earlier may write what this recording reads (the queue orders submits, the
    // barrier their memory).
    FullBarrier(c.rec.vk);
    c.rendering = false;
    c.bound_pipeline = VK_NULL_HANDLE;
    c.bound_layout = VK_NULL_HANDLE;  // a new Vulkan command buffer has no sets bound: the next draw writes them
}

// The handle nvnQueueSubmitCommands takes: a number of the shim's, never 0.
std::uint64_t CommandBufferEndRecording(void* cmd) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    CommandBuffer& c = g_commands.Get(cmd);
    c.recording = false;
    c.recordings++;
    const std::uint64_t handle = g_next_handle++;
    if (c.rec.vk) {
        EndRendering(c);
        vkEndCommandBuffer(c.rec.vk);
        g_recorded[handle] = std::move(c.rec);
        c.rec = Recording();
        g_stats.recordings++;
    }
    return handle;
}

void CommandBufferSetViewport(void* cmd, std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_commands.Get(cmd);
    g_state.viewport[0] = x;
    g_state.viewport[1] = y;
    g_state.viewport[2] = w;
    g_state.viewport[3] = h;
}

void CommandBufferSetScissor(void* cmd, std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_commands.Get(cmd);
    g_state.scissor[0] = x;
    g_state.scissor[1] = y;
    g_state.scissor[2] = w;
    g_state.scissor[3] = h;
}

// With one sample per pixel (the shim's targets), only bit 0 could matter; kept.
void CommandBufferSetSampleMask(void*, std::int32_t mask) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_state.sample_mask = mask;
}

void CommandBufferSetTexturePool(void*, const void* pool) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_state.texture_pool = pool;
}

void CommandBufferSetSamplerPool(void*, const void* pool) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_state.sampler_pool = pool;
}

// The console's shader scratch memory (fn_710015e980: 1 MB of a pool); Vulkan needs none.
void CommandBufferSetShaderScratchMemory(void*, const void* pool, std::int64_t offset, std::int64_t size) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_state.scratch_pool = pool;
    g_state.scratch_offset = offset;
    g_state.scratch_size = size;
}

void CommandBufferBindProgram(void*, const void* program, std::int32_t stages) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (program) {
        g_state.program = program;
        g_state.program_stages = stages;
    } else {
        g_state.program_stages &= ~stages;  // fn_710015d900 unbinds stages with (null, bits)
        if (!g_state.program_stages)
            g_state.program = nullptr;
    }
}

void CommandBufferBindUniformBuffer(void*, std::int32_t stage, std::int32_t slot, BufferAddress address,
                                    std::uint64_t size) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_state.bindings.BindUniformBuffer(static_cast<Stage>(stage), static_cast<std::uint32_t>(slot), address, size);
}

void CommandBufferBindTexture(void*, std::int32_t stage, std::int32_t slot, TextureHandle handle) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_state.bindings.BindTexture(static_cast<Stage>(stage), static_cast<std::uint32_t>(slot), handle);
}

// fn_710015e700 updates a whole block: (buffer address, its size, offset 0, the size, the data).
void CommandBufferUpdateUniformBuffer(void* cmd, BufferAddress buffer, std::uint64_t buffer_size, std::int64_t offset,
                                      std::uint64_t size, const void* data) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (!ActiveCommandBuffer(cmd, "nvnCommandBufferUpdateUniformBuffer") || !data || offset < 0 ||
        static_cast<std::uint64_t>(offset) + size > buffer_size || buffer_size > kUploadChunkSize)
        return;
    Shadow& s = g_shadows[buffer];
    if (s.data.size() != buffer_size) {
        // A new shadow starts from the game's memory (what the GPU would read without updates).
        s.data.assign(buffer_size, 0);
        if (const void* storage = GpuAddressToStorage(buffer))
            std::memcpy(s.data.data(), storage, buffer_size);
    }
    std::memcpy(s.data.data() + offset, data, size);
    s.version = ++g_shadow_version;
    g_stats.uniform_updates++;
}

void CommandBufferBindBlendState(void*, const void* state) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    const detail::BlendState* s = detail::FindBlendState(state);
    if (s && s->target >= 0 && s->target < kColorTargets)
        g_state.blend[s->target] = s->blend;
}

void CommandBufferBindChannelMaskState(void*, const void* state) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (const detail::ChannelMaskState* s = detail::FindChannelMaskState(state))
        g_state.channel_mask = *s;
}

void CommandBufferBindColorState(void*, const void* state) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (const detail::ColorState* s = detail::FindColorState(state))
        g_state.color = *s;
}

void CommandBufferBindMultisampleState(void*, const void* state) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (const detail::MultisampleState* s = detail::FindMultisampleState(state))
        g_state.multisample = *s;
}

void CommandBufferBindPolygonState(void*, const void* state) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (const detail::PolygonState* s = detail::FindPolygonState(state))
        g_state.polygon = *s;
}

void CommandBufferBindDepthStencilState(void*, const void* state) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (const detail::DepthStencilState* s = detail::FindDepthStencilState(state))
        g_state.depth_stencil = *s;
}

// The game passes arrays of its attribute (4 bytes each) and stream (8 bytes each) states
// (fn_7100161800: count, then the array at its vertex buffer + 0xb8 and + 0xf8).
constexpr std::size_t kVertexAttribStateSize = 4;
constexpr std::size_t kVertexStreamStateSize = 8;

void CommandBufferBindVertexAttribState(void*, std::int32_t count, const void* attribs) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_state.attrib_count = static_cast<std::uint32_t>(std::clamp(count, 0, kVertexAttribs));
    for (std::uint32_t i = 0; i < g_state.attrib_count; ++i) {
        const detail::VertexAttribState* a =
            detail::FindVertexAttribState(static_cast<const char*>(attribs) + i * kVertexAttribStateSize);
        g_state.attribs[i] = a ? *a : detail::VertexAttribState();
    }
}

void CommandBufferBindVertexStreamState(void*, std::int32_t count, const void* streams) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_state.stream_count = static_cast<std::uint32_t>(std::clamp(count, 0, kVertexStreams));
    for (std::uint32_t i = 0; i < g_state.stream_count; ++i) {
        const detail::VertexStreamState* s =
            detail::FindVertexStreamState(static_cast<const char*>(streams) + i * kVertexStreamStateSize);
        g_state.streams[i] = s ? *s : detail::VertexStreamState();
    }
}

// fn_7100161800: (cmd, stream, address, size); the size is a 32-bit int (w3).
void CommandBufferBindVertexBuffer(void*, std::int32_t index, BufferAddress address, std::uint32_t size) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (index >= 0 && index < kVertexStreams)
        g_state.vertex_buffers[index] = {address, size};
}

void CommandBufferSetAlphaRef(void*, float ref) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_state.alpha_ref = ref;
}

void CommandBufferSetStencilMask(void*, std::int32_t face, std::int32_t mask) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (detail::FaceHasFront(face))
        g_state.stencil_write_mask[0] = static_cast<std::uint32_t>(mask);
    if (detail::FaceHasBack(face))
        g_state.stencil_write_mask[1] = static_cast<std::uint32_t>(mask);
}

void CommandBufferSetStencilRef(void*, std::int32_t face, std::int32_t ref) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (detail::FaceHasFront(face))
        g_state.stencil_ref[0] = static_cast<std::uint32_t>(ref);
    if (detail::FaceHasBack(face))
        g_state.stencil_ref[1] = static_cast<std::uint32_t>(ref);
}

// fn_710015a9d0: (cmd, enable, min, max).
void CommandBufferSetDepthBounds(void*, std::uint8_t enable, float min, float max) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_state.depth_bounds = enable & 1;
    g_state.depth_bounds_min = min;
    g_state.depth_bounds_max = max;
}

// fn_710015c2d0: (cmd, count, the textures, their views (null), the depth texture, its view (null)).
void CommandBufferSetRenderTargets(void* cmd, std::int32_t count, const void* const* colors, const void* const*,
                                   const void* depth, const void*) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (CommandBuffer* c = g_commands.Find(cmd); c && c->recording)
        EndRendering(*c);
    g_state.color_count = static_cast<std::uint32_t>(std::clamp(count, 0, kColorTargets));
    for (std::uint32_t i = 0; i < kColorTargets; ++i)
        g_state.colors[i] = i < g_state.color_count && colors ? colors[i] : nullptr;
    g_state.depth = depth;
}

// fn_710015c3f0 passes 2; whatever its bits order, a full barrier orders more.
void CommandBufferBarrier(void* cmd, std::int32_t) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    CommandBuffer* c = ActiveCommandBuffer(cmd, "nvnCommandBufferBarrier");
    if (!c)
        return;
    if (c->rendering)
        EndRendering(*c);  // ends with a full barrier
    else
        FullBarrier(c->rec.vk);
}

// fn_7100159660: (cmd, target, the colour, 0xf). The whole render area; Vulkan's clear ignores the
// channel mask, and the game clears every channel.
void CommandBufferClearColor(void* cmd, std::int32_t index, const float* color, std::int32_t mask) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    CommandBuffer* c = ActiveCommandBuffer(cmd, "nvnCommandBufferClearColor");
    if (!c || !color || index < 0 || static_cast<std::uint32_t>(index) >= g_state.color_count || !g_state.colors[index] ||
        AttachmentFormat(g_state.colors[index], false) == VK_FORMAT_UNDEFINED || !BeginRendering(*c))
        return;
    if ((mask & 0xf) != 0xf)
        ReportOnce("a colour clear of some channels clears all of them");
    VkClearAttachment ca{};
    ca.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    ca.colorAttachment = static_cast<std::uint32_t>(index);
    std::memcpy(ca.clearValue.color.float32, color, sizeof(float) * 4);
    const VkClearRect rect{c->render_area, 0, 1};
    vkCmdClearAttachments(c->rec.vk, 1, &ca, 1, &rect);
    g_stats.clears++;
}

// fn_7100159660: (cmd, depth, whether to clear it, stencil, its mask: 0 or all ones).
void CommandBufferClearDepthStencil(void* cmd, float depth, std::uint8_t depth_mask, std::int32_t stencil,
                                    std::int32_t stencil_mask) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    CommandBuffer* c = ActiveCommandBuffer(cmd, "nvnCommandBufferClearDepthStencil");
    if (!c || AttachmentFormat(g_state.depth, true) == VK_FORMAT_UNDEFINED)
        return;
    VkClearAttachment ca{};
    ca.aspectMask = ((depth_mask & 1) ? VK_IMAGE_ASPECT_DEPTH_BIT : 0) | (stencil_mask ? VK_IMAGE_ASPECT_STENCIL_BIT : 0);
    if (!ca.aspectMask || !BeginRendering(*c))
        return;
    ca.clearValue.depthStencil = {depth, static_cast<std::uint32_t>(stencil)};
    const VkClearRect rect{c->render_area, 0, 1};
    vkCmdClearAttachments(c->rec.vk, 1, &ca, 1, &rect);
    g_stats.clears++;
}

// fn_7100159f90: (cmd, primitive, index type, count, index address).
void CommandBufferDrawElements(void* cmd, std::int32_t primitive, std::int32_t index_type, std::int32_t count,
                               BufferAddress indices) {
    Draw(cmd, primitive, index_type, count, indices, 0, 0, 1);
}

// fn_710015ac00: (cmd, primitive, index type, count, index address, base vertex, base instance,
// instance count); its one caller (fn_71001cf9ec) passes 0, 0 and its instance count.
void CommandBufferDrawElementsInstanced(void* cmd, std::int32_t primitive, std::int32_t index_type, std::int32_t count,
                                        BufferAddress indices, std::int32_t base_vertex, std::int32_t base_instance,
                                        std::int32_t instances) {
    Draw(cmd, primitive, index_type, count, indices, base_vertex, static_cast<std::uint32_t>(std::max(base_instance, 0)),
         static_cast<std::uint32_t>(std::max(instances, 0)));
}

void CommandBufferSetDebugLabel(void*, const char*) {}

}  // namespace

CommandBufferStats GetCommandBufferStats(const void* cmd) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    CommandBufferStats out;
    if (CommandBuffer* c = g_commands.Find(cmd)) {
        out.exists = true;
        out.recording = c->recording;
        out.recordings = c->recordings;
        out.has_command_memory = c->command_size != 0;
        out.has_control_memory = c->control_size != 0;
        out.has_memory_callback = c->memory_callback != nullptr;
        for (int i = 0; i < 4; ++i) {
            out.viewport[i] = g_state.viewport[i];
            out.scissor[i] = g_state.scissor[i];
        }
    }
    return out;
}

DrawStats GetDrawStats() {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    DrawStats out = g_stats;
    out.pipelines = detail::PipelinesMade();
    return out;
}

// nvnQueueSubmitCommands (queue.cpp): submits the recordings behind the handles, in order. A handle
// with no recording (made without a Vulkan device) is counted and skipped.
std::uint64_t detail::SubmitRecordings(std::int32_t count, const std::uint64_t* handles) {
    const Backend* be = CurrentBackend();
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (!be || count <= 0 || !handles)
        return 0;
    std::vector<Recording> recs;
    std::vector<VkCommandBuffer> cmds;
    for (std::int32_t i = 0; i < count; ++i) {
        auto it = g_recorded.find(handles[i]);
        if (it == g_recorded.end()) {
            ReportOnce("nvnQueueSubmitCommands with a handle that is not a finished recording (skipped)");
            continue;
        }
        recs.push_back(std::move(it->second));
        g_recorded.erase(it);
        cmds.push_back(recs.back().vk);
    }
    if (cmds.empty())
        return 0;
    if (!be->host_import)  // pools are copies: bring in what the recordings read (memory.h)
        for (const Recording& r : recs)
            for (const auto& range : r.reads)
                SyncPoolRange(range.first, range.second);
    const std::uint64_t value = Submit(cmds.data(), static_cast<std::uint32_t>(cmds.size()));
    if (!value) {
        std::fprintf(stderr, "nvn shim: vkQueueSubmit failed\n");
        WaitFor(LastSubmitted(), UINT64_MAX);
        for (Recording& r : recs)
            FreeRecording(*be, r);
        return 0;
    }
    for (Recording& r : recs) {
        r.retire = value;
        g_in_flight.push_back(std::move(r));
    }
    g_stats.submitted += cmds.size();
    return value;
}

void detail::ReleaseDrawVulkanObjects() {
    {
        std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
        const Backend* be = CurrentBackend();
        if (be) {
            g_commands.ForEach([&](const void*, CommandBuffer& c) {
                if (c.rec.vk)
                    FreeRecording(*be, c.rec);
                c.rendering = false;
                c.bound_pipeline = VK_NULL_HANDLE;
                c.bound_layout = VK_NULL_HANDLE;
            });
            for (auto& kv : g_recorded)
                FreeRecording(*be, kv.second);
            for (Recording& r : g_in_flight)
                FreeRecording(*be, r);
            for (VkDescriptorPool p : g_free_pools)
                vkDestroyDescriptorPool(be->device, p, nullptr);
            for (UploadChunk& c : g_free_chunks) {
                vkFreeMemory(be->device, c.memory, nullptr);
                vkDestroyBuffer(be->device, c.buffer, nullptr);
            }
            if (g_pool)
                vkDestroyCommandPool(be->device, g_pool, nullptr);  // frees its command buffers
        }
        g_recorded.clear();
        g_in_flight.clear();
        g_free_pools.clear();
        g_free_chunks.clear();
        g_free_command_buffers.clear();
        g_pool = VK_NULL_HANDLE;
        ReleasePipelines();
    }
    ReleaseProgramVulkanObjects();
}

void detail::RegisterCommandProcs() {
    Register("nvnCommandBufferInitialize", &CommandBufferInitialize);
    Register("nvnCommandBufferFinalize", &CommandBufferFinalize);
    Register("nvnCommandBufferAddCommandMemory", &CommandBufferAddCommandMemory);
    Register("nvnCommandBufferAddControlMemory", &CommandBufferAddControlMemory);
    Register("nvnCommandBufferSetMemoryCallback", &CommandBufferSetMemoryCallback);
    Register("nvnCommandBufferBeginRecording", &CommandBufferBeginRecording);
    Register("nvnCommandBufferEndRecording", &CommandBufferEndRecording);
    Register("nvnCommandBufferSetViewport", &CommandBufferSetViewport);
    Register("nvnCommandBufferSetScissor", &CommandBufferSetScissor);
    Register("nvnCommandBufferSetSampleMask", &CommandBufferSetSampleMask);
    Register("nvnCommandBufferSetTexturePool", &CommandBufferSetTexturePool);
    Register("nvnCommandBufferSetSamplerPool", &CommandBufferSetSamplerPool);
    Register("nvnCommandBufferSetShaderScratchMemory", &CommandBufferSetShaderScratchMemory);
    Register("nvnCommandBufferBindProgram", &CommandBufferBindProgram);
    Register("nvnCommandBufferBindUniformBuffer", &CommandBufferBindUniformBuffer);
    Register("nvnCommandBufferBindTexture", &CommandBufferBindTexture);
    Register("nvnCommandBufferUpdateUniformBuffer", &CommandBufferUpdateUniformBuffer);
    Register("nvnCommandBufferBindBlendState", &CommandBufferBindBlendState);
    Register("nvnCommandBufferBindChannelMaskState", &CommandBufferBindChannelMaskState);
    Register("nvnCommandBufferBindColorState", &CommandBufferBindColorState);
    Register("nvnCommandBufferBindMultisampleState", &CommandBufferBindMultisampleState);
    Register("nvnCommandBufferBindPolygonState", &CommandBufferBindPolygonState);
    Register("nvnCommandBufferBindDepthStencilState", &CommandBufferBindDepthStencilState);
    Register("nvnCommandBufferBindVertexAttribState", &CommandBufferBindVertexAttribState);
    Register("nvnCommandBufferBindVertexStreamState", &CommandBufferBindVertexStreamState);
    Register("nvnCommandBufferBindVertexBuffer", &CommandBufferBindVertexBuffer);
    Register("nvnCommandBufferSetAlphaRef", &CommandBufferSetAlphaRef);
    Register("nvnCommandBufferSetStencilMask", &CommandBufferSetStencilMask);
    Register("nvnCommandBufferSetStencilRef", &CommandBufferSetStencilRef);
    Register("nvnCommandBufferSetDepthBounds", &CommandBufferSetDepthBounds);
    Register("nvnCommandBufferSetRenderTargets", &CommandBufferSetRenderTargets);
    Register("nvnCommandBufferBarrier", &CommandBufferBarrier);
    Register("nvnCommandBufferClearColor", &CommandBufferClearColor);
    Register("nvnCommandBufferClearDepthStencil", &CommandBufferClearDepthStencil);
    Register("nvnCommandBufferDrawElements", &CommandBufferDrawElements);
    Register("nvnCommandBufferDrawElementsInstanced", &CommandBufferDrawElementsInstanced);
    Register("nvnCommandBufferSetDebugLabel", &CommandBufferSetDebugLabel);
}

}  // namespace dread::nvn
