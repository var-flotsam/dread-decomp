// NVN command buffers. Signatures are those of the game's calls: fn_71001587f0 for Initialize,
// AddCommandMemory, AddControlMemory, SetMemoryCallback, SetViewport and SetScissor; the frame's
// begin fn_7100159920 for BeginRecording and SetSampleMask; its end fn_7100159ac0 for EndRecording,
// whose handle goes to nvnQueueSubmitCommands; the program binding fn_710015d900 for BindProgram and
// BindUniformBuffer; fn_7100161250 for BindTexture.
//
// So far a command buffer keeps what the game binds (the binding layer's BindingState) and its
// dynamic state; draws and the Vulkan command buffers they go into are the next step. The command
// and control memory the game adds every frame is NVN's own encoding space; the shim records into
// its own structures and only checks that memory was given.

#include <cstdio>

#include "nvn_shim/bindings.h"
#include "nvn_shim/command.h"
#include "nvn_shim/procs.h"
#include "objects.h"

namespace dread::nvn {

namespace {

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
    const void* program = nullptr;
    std::int32_t program_stages = 0;
    std::int32_t viewport[4] = {};
    std::int32_t scissor[4] = {};
    std::int32_t sample_mask = -1;
    const void* texture_pool = nullptr;
    const void* sampler_pool = nullptr;
    BindingState bindings;
};

detail::Registry<CommandBuffer> g_commands;
std::uint64_t g_next_handle = 1;

std::int32_t CommandBufferInitialize(void* cmd, const void* device) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_commands.Reset(cmd).device = device;
    return 1;
}

void CommandBufferFinalize(void* cmd) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_commands.Erase(cmd);
}

void CommandBufferAddCommandMemory(void* cmd, const void* pool, std::int64_t offset, std::uint64_t size) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    CommandBuffer& c = g_commands.Get(cmd);
    c.command_pool = pool;
    c.command_offset = offset;
    c.command_size = size;
}

void CommandBufferAddControlMemory(void* cmd, void* memory, std::uint64_t size) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    CommandBuffer& c = g_commands.Get(cmd);
    c.control_memory = memory;
    c.control_size = size;
}

// Called by NVN when a command buffer runs out of memory; the shim's never does.
void CommandBufferSetMemoryCallback(void* cmd, void* callback) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_commands.Get(cmd).memory_callback = callback;
}

void CommandBufferBeginRecording(void* cmd) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    CommandBuffer& c = g_commands.Get(cmd);
    if (!c.command_size || !c.control_size)
        std::fprintf(stderr, "nvn shim: nvnCommandBufferBeginRecording without command or control memory\n");
    c.recording = true;
}

// The handle nvnQueueSubmitCommands takes: a number of the shim's, never 0.
std::uint64_t CommandBufferEndRecording(void* cmd) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    CommandBuffer& c = g_commands.Get(cmd);
    c.recording = false;
    c.recordings++;
    return g_next_handle++;
}

void CommandBufferSetViewport(void* cmd, std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    CommandBuffer& c = g_commands.Get(cmd);
    c.viewport[0] = x;
    c.viewport[1] = y;
    c.viewport[2] = w;
    c.viewport[3] = h;
}

void CommandBufferSetScissor(void* cmd, std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    CommandBuffer& c = g_commands.Get(cmd);
    c.scissor[0] = x;
    c.scissor[1] = y;
    c.scissor[2] = w;
    c.scissor[3] = h;
}

void CommandBufferSetSampleMask(void* cmd, std::int32_t mask) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_commands.Get(cmd).sample_mask = mask;
}

void CommandBufferSetTexturePool(void* cmd, const void* pool) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_commands.Get(cmd).texture_pool = pool;
}

void CommandBufferSetSamplerPool(void* cmd, const void* pool) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_commands.Get(cmd).sampler_pool = pool;
}

void CommandBufferBindProgram(void* cmd, const void* program, std::int32_t stages) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    CommandBuffer& c = g_commands.Get(cmd);
    if (program) {
        c.program = program;
        c.program_stages = stages;
    } else {
        c.program_stages &= ~stages;  // fn_710015d900 unbinds stages with (null, bits)
        if (!c.program_stages)
            c.program = nullptr;
    }
}

void CommandBufferBindUniformBuffer(void* cmd, std::int32_t stage, std::int32_t slot, BufferAddress address,
                                    std::uint64_t size) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_commands.Get(cmd).bindings.BindUniformBuffer(static_cast<Stage>(stage), static_cast<std::uint32_t>(slot), address,
                                                   size);
}

void CommandBufferBindTexture(void* cmd, std::int32_t stage, std::int32_t slot, TextureHandle handle) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_commands.Get(cmd).bindings.BindTexture(static_cast<Stage>(stage), static_cast<std::uint32_t>(slot), handle);
}

void CommandBufferSetDebugLabel(void*, const char*) {}

}  // namespace

CommandBufferStats GetCommandBufferStats(const void* cmd) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    CommandBufferStats out;
    if (CommandBuffer* c = g_commands.Find(cmd)) {
        out.exists = true;
        out.recording = c->recording;
        out.recordings = c->recordings;
        out.has_command_memory = c->command_size != 0;
        out.has_control_memory = c->control_size != 0;
        out.has_memory_callback = c->memory_callback != nullptr;
        for (int i = 0; i < 4; ++i) {
            out.viewport[i] = c->viewport[i];
            out.scissor[i] = c->scissor[i];
        }
    }
    return out;
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
    Register("nvnCommandBufferBindProgram", &CommandBufferBindProgram);
    Register("nvnCommandBufferBindUniformBuffer", &CommandBufferBindUniformBuffer);
    Register("nvnCommandBufferBindTexture", &CommandBufferBindTexture);
    Register("nvnCommandBufferSetDebugLabel", &CommandBufferSetDebugLabel);
}

}  // namespace dread::nvn
