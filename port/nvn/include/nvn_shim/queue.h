#pragma once

// NVN queues and syncs (src/queue.cpp has what each function does so far).

#include <cstdint>

namespace dread::nvn {

// What the shim has seen a queue do (for checks).
struct QueueStats {
    bool exists = false;
    std::uint64_t submits = 0, submitted_handles = 0, flushes = 0, fences = 0, presents = 0;
};
QueueStats GetQueueStats(const void* queue);

}  // namespace dread::nvn
