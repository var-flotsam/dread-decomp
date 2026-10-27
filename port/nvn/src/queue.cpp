// NVN queues and syncs. Signatures are those of the game's calls: fn_71001587f0 for the queue
// builder, QueueInitialize, SyncInitialize, QueueFenceSync, QueueFlush and SyncWait; the frame's end
// fn_7100159ac0 for QueueSubmitCommands; the present fn_7100159bf0 for QueuePresentTexture.
//
// What runs on the GPU is not built yet (the command buffers record no draws so far), so a submit
// counts the command handles it is given and a fence is signalled at once: the next sync wait finds
// it signalled. When command buffers turn into Vulkan command buffers, the fence becomes a Vulkan
// timeline value and SyncWait waits for it. No caller looks at SyncWait's result (fn_71001587f0,
// fn_7100159ac0 both pass a timeout of -1 and go on).

#include <cstdio>

#include "nvn_shim/procs.h"
#include "nvn_shim/queue.h"
#include "objects.h"

namespace dread::nvn {

namespace {

struct QueueBuilder {
    const void* device = nullptr;
    std::int64_t command_memory_size = 0;
    std::int64_t compute_memory_size = 0;
    std::int64_t flush_threshold = 0;
    void* memory = nullptr;
    std::uint64_t memory_size = 0;
};

struct Queue {
    std::uint64_t submitted_handles = 0;
    std::uint64_t submits = 0;
    std::uint64_t flushes = 0;
    std::uint64_t presents = 0;
    std::uint64_t fences = 0;
};

struct Sync {
    bool signalled = false;
    std::uint64_t fence_value = 0;
};

detail::Registry<QueueBuilder> g_builders;
detail::Registry<Queue> g_queues;
detail::Registry<Sync> g_syncs;

// The queue memory the game must give (fn_71001587f0 allocates this much, 4 KB aligned): the shim
// keeps its queue state itself, so a small block suffices.
constexpr std::uint64_t kQueueMemorySize = 0x1000;

void QueueBuilderSetDefaults(void* builder) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_builders.Reset(builder);
}
void QueueBuilderSetDevice(void* builder, const void* device) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_builders.Get(builder).device = device;
}
void QueueBuilderSetCommandMemorySize(void* builder, std::int64_t size) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_builders.Get(builder).command_memory_size = size;
}
void QueueBuilderSetComputeMemorySize(void* builder, std::int64_t size) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_builders.Get(builder).compute_memory_size = size;
}
void QueueBuilderSetCommandFlushThreshold(void* builder, std::int64_t size) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_builders.Get(builder).flush_threshold = size;
}
std::uint64_t QueueBuilderGetQueueMemorySize(const void*) {
    return kQueueMemorySize;
}
void QueueBuilderSetQueueMemory(void* builder, void* memory, std::uint64_t size) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    QueueBuilder& b = g_builders.Get(builder);
    b.memory = memory;
    b.memory_size = size;
}

std::int32_t QueueInitialize(void* queue, const void* builder) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    QueueBuilder* b = g_builders.Find(builder);
    if (!b || !b->memory || b->memory_size < kQueueMemorySize) {
        std::fprintf(stderr, "nvn shim: nvnQueueInitialize without queue memory\n");
        return 0;
    }
    g_queues.Reset(queue);
    return 1;
}

void QueueFinalize(void* queue) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_queues.Erase(queue);
}

void QueueSubmitCommands(void* queue, std::int32_t count, const std::uint64_t* handles) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    Queue& q = g_queues.Get(queue);
    q.submits++;
    if (count > 0 && handles)
        q.submitted_handles += static_cast<std::uint64_t>(count);
}

void QueueFlush(void* queue) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_queues.Get(queue).flushes++;
}

void QueueFinish(void* queue) {
    QueueFlush(queue);
}

void QueueFenceSync(void* queue, void* sync, std::int32_t, std::int32_t) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    Queue& q = g_queues.Get(queue);
    Sync& s = g_syncs.Get(sync);
    s.fence_value = ++q.fences;
    s.signalled = true;  // nothing is in flight yet (header comment)
}

void QueuePresentTexture(void* queue, const void*, std::int32_t) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_queues.Get(queue).presents++;
}

void QueueSetDebugLabel(void*, const char*) {}

std::int32_t SyncInitialize(void* sync, const void*) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_syncs.Reset(sync);
    return 1;
}

void SyncFinalize(void* sync) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_syncs.Erase(sync);
}

// Returns 0, the shim's "signalled" (the game never looks at the result).
std::int32_t SyncWait(const void*, std::uint64_t) {
    return 0;  // every fence is signalled when it is set (header comment)
}

void SyncSetDebugLabel(void*, const char*) {}

}  // namespace

QueueStats GetQueueStats(const void* queue) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    QueueStats out;
    if (Queue* q = g_queues.Find(queue)) {
        out.submits = q->submits;
        out.submitted_handles = q->submitted_handles;
        out.flushes = q->flushes;
        out.fences = q->fences;
        out.presents = q->presents;
        out.exists = true;
    }
    return out;
}

void detail::RegisterQueueProcs() {
    Register("nvnQueueBuilderSetDefaults", &QueueBuilderSetDefaults);
    Register("nvnQueueBuilderSetDevice", &QueueBuilderSetDevice);
    Register("nvnQueueBuilderSetCommandMemorySize", &QueueBuilderSetCommandMemorySize);
    Register("nvnQueueBuilderSetComputeMemorySize", &QueueBuilderSetComputeMemorySize);
    Register("nvnQueueBuilderSetCommandFlushThreshold", &QueueBuilderSetCommandFlushThreshold);
    Register("nvnQueueBuilderGetQueueMemorySize", &QueueBuilderGetQueueMemorySize);
    Register("nvnQueueBuilderSetQueueMemory", &QueueBuilderSetQueueMemory);
    Register("nvnQueueInitialize", &QueueInitialize);
    Register("nvnQueueFinalize", &QueueFinalize);
    Register("nvnQueueSubmitCommands", &QueueSubmitCommands);
    Register("nvnQueueFlush", &QueueFlush);
    Register("nvnQueueFinish", &QueueFinish);
    Register("nvnQueueFenceSync", &QueueFenceSync);
    Register("nvnQueuePresentTexture", &QueuePresentTexture);
    Register("nvnQueueSetDebugLabel", &QueueSetDebugLabel);
    Register("nvnSyncInitialize", &SyncInitialize);
    Register("nvnSyncFinalize", &SyncFinalize);
    Register("nvnSyncWait", &SyncWait);
    Register("nvnSyncSetDebugLabel", &SyncSetDebugLabel);
}

}  // namespace dread::nvn
