// nn::os for the port: the tick from a monotonic host clock, mutexes over the host's, and a
// per-thread stand-in for the thread control block. Every stub names the boot-path function
// that depends on it in a TODO.

#include "nn/os.h"

#include <chrono>
#include <mutex>
#include <new>

namespace nn::os {

namespace {

// The host lock lives behind MutexType's 32 bytes: std::recursive_mutex is larger than that on
// most hosts, so the storage holds a pointer to one allocated on initialisation.
struct MutexImpl {
    std::recursive_mutex* lock;
    bool recursive;
    s32 lock_level;
};
static_assert(sizeof(MutexImpl) <= sizeof(MutexType), "must fit the game's 32-byte mutex");

MutexImpl& Impl(MutexType* mutex) { return *reinterpret_cast<MutexImpl*>(mutex->storage); }

s64 MonotonicNanoseconds() {
    using namespace std::chrono;
    return duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count();
}

}  // namespace

// Whole seconds first, then the remainder, so the products never overflow 64 bits.
Tick ConvertToTick(TimeSpan span) {
    const s64 ns = span.nanoseconds;
    return Tick(ns / 1000000000 * SystemTickFrequency + (ns % 1000000000) * SystemTickFrequency / 1000000000);
}

TimeSpan ConvertToTimeSpan(Tick tick) {
    const s64 t = tick.value;
    return TimeSpan{t / SystemTickFrequency * 1000000000 + (t % SystemTickFrequency) * 1000000000 / SystemTickFrequency};
}

Tick GetSystemTick() {
    // TODO(M1): the frame timers (fn_7100080c40, fn_7100080f2c, their caller fn_7100015c70)
    // read this every frame. A monotonic host clock is right; counting from the first call keeps
    // the values small, as the console's counter is near zero at boot.
    static const s64 s_origin = MonotonicNanoseconds();
    return ConvertToTick(TimeSpan{MonotonicNanoseconds() - s_origin});
}

void InitializeMutex(MutexType* mutex, bool recursive, s32 lock_level) {
    // TODO(M1): the engine's mutex wrappers (fn_71000668f8 initialises recursive mutexes,
    // include/base/UnkDefaultAllocator.h), which the default allocator uses around every
    // allocation. A non-recursive mutex gets the same host lock until a caller needs the
    // difference: re-locking one is an error on the console, not a feature.
    Impl(mutex) = MutexImpl{new (std::nothrow) std::recursive_mutex, recursive, lock_level};
    if (!Impl(mutex).lock)
        __builtin_trap();
}

void FinalizeMutex(MutexType* mutex) {
    // TODO(M1): fn_7100066918, the finalize wrapper, from destructors such as fn_71000707dc.
    delete Impl(mutex).lock;
    Impl(mutex).lock = nullptr;
}

void LockMutex(MutexType* mutex) {
    // TODO(M1): fn_710006691c, the lock wrapper (the default allocator's slots 0x40 and 0x48).
    Impl(mutex).lock->lock();
}

void UnlockMutex(MutexType* mutex) {
    // TODO(M1): fn_7100066920, the unlock wrapper.
    Impl(mutex).lock->unlock();
}

struct ThreadType {
    u64 host_id;
};

ThreadType* GetCurrentThread() {
    // TODO(M1): the default allocator keys a sub-allocator on the calling thread
    // (src/base/UnkDefaultAllocatorGlobal.cpp). Until CreateThread exists, every host thread gets
    // a stand-in block, so the pointer is stable and distinct per thread, which is all that caller
    // compares.
    thread_local ThreadType s_thread{};
    return &s_thread;
}

}  // namespace nn::os
