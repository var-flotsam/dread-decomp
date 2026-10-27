#pragma once

// NVN command buffers (src/command.cpp has what each function does so far).

#include <cstdint>

namespace dread::nvn {

// What the shim has seen a command buffer do (for checks).
struct CommandBufferStats {
    bool exists = false;
    bool recording = false;
    std::uint64_t recordings = 0;
    bool has_command_memory = false, has_control_memory = false, has_memory_callback = false;
    std::int32_t viewport[4] = {}, scissor[4] = {};
};
CommandBufferStats GetCommandBufferStats(const void* cmd);

}  // namespace dread::nvn
