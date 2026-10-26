// nn::os for the port: the tick from a monotonic host clock, mutexes, events, message queues and
// threads over the host's, and the host's own arguments. Every stub names the boot-path function
// that depends on it in a TODO.

#include "nn/os.h"

#include "dread_port.h"

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <new>
#include <thread>

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

std::chrono::nanoseconds Duration(TimeSpan span) {
    return std::chrono::nanoseconds(span.nanoseconds > 0 ? span.nanoseconds : 0);
}

// The other objects the game embeds (threads, events, light events, system events, message
// queues) hold a pointer to the port's host object at +0 and a tag word at +8, written and read
// byte-wise: a light event is only 4-aligned. A tag that does not match means nobody initialised
// the object (or it was finalised), which on the console would be undefined behaviour; the port
// stops with the call's name instead.
enum Tag : u32 {
    kTagThread = 0x44525448,        // 'DRTH'
    kTagLightEvent = 0x44524c45,    // 'DRLE'
    kTagEvent = 0x44524556,         // 'DREV'
    kTagSystemEvent = 0x44525345,   // 'DRSE'
    kTagMessageQueue = 0x44524d51,  // 'DRMQ'
};

void Attach(void* object, void* impl, Tag tag) {
    auto* bytes = static_cast<u8*>(object);
    std::memcpy(bytes, &impl, sizeof(impl));
    const u32 word = tag;
    std::memcpy(bytes + 8, &word, sizeof(word));
}

void Detach(void* object) {
    static const u8 zero[12] = {};
    std::memcpy(object, zero, sizeof(zero));
}

[[noreturn]] void NotInitialised(const char* call, const void* object) {
    std::fprintf(stderr, "dread: nn::os::%s on an object the port did not initialise (%p)\n", call, object);
    std::fflush(stderr);
    std::abort();
}

template <typename T>
T* Find(const void* object, Tag tag, const char* call) {
    const auto* bytes = static_cast<const u8*>(object);
    void* impl;
    u32 word;
    std::memcpy(&impl, bytes, sizeof(impl));
    std::memcpy(&word, bytes + 8, sizeof(word));
    if (word != tag || impl == nullptr)
        NotInitialised(call, object);
    return static_cast<T*>(impl);
}

// Events, light events and system events share one host event. Auto clear: a wait consumes the
// signal and a signal wakes one waiter; manual clear: the signal stays until cleared and wakes all.
struct HostEvent {
    std::mutex lock;
    std::condition_variable changed;
    bool signaled;
    bool auto_clear;

    HostEvent(bool s, EventClearMode mode) : signaled(s), auto_clear(mode == EventClearMode_AutoClear) {}

    void Signal() {
        {
            std::lock_guard<std::mutex> hold(lock);
            signaled = true;
        }
        if (auto_clear)
            changed.notify_one();
        else
            changed.notify_all();
    }

    void Clear() {
        std::lock_guard<std::mutex> hold(lock);
        signaled = false;
    }

    // Under the lock, after the signal was seen.
    bool Consume() {
        if (auto_clear)
            signaled = false;
        return true;
    }

    void Wait() {
        std::unique_lock<std::mutex> hold(lock);
        changed.wait(hold, [this] { return signaled; });
        Consume();
    }

    bool TryWait() {
        std::lock_guard<std::mutex> hold(lock);
        return signaled && Consume();
    }

    bool TimedWait(TimeSpan timeout) {
        std::unique_lock<std::mutex> hold(lock);
        return changed.wait_for(hold, Duration(timeout), [this] { return signaled; }) && Consume();
    }
};

HostEvent* NewEvent(bool signaled, EventClearMode mode) {
    auto* event = new (std::nothrow) HostEvent(signaled, mode);
    if (!event)
        __builtin_trap();
    return event;
}

struct HostThread {
    std::thread thread;
    ThreadType* block;
    ThreadFunction function;
    void* argument;
    s32 priority;
    s32 ideal_core;
    u64 affinity_mask;
    const char* name;
};

thread_local ThreadType* t_current_thread = nullptr;

void RunThread(HostThread* host) {
    t_current_thread = host->block;
    host->function(host->argument);
}

// A ring of words in the caller's buffer.
struct HostQueue {
    std::mutex lock;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    u64* buffer;
    u64 capacity;
    u64 first;
    u64 count;

    void PushBack(u64 message) {
        buffer[(first + count) % capacity] = message;
        ++count;
    }

    void PushFront(u64 message) {
        first = (first + capacity - 1) % capacity;
        buffer[first] = message;
        ++count;
    }

    u64 PopFront() {
        const u64 message = buffer[first];
        first = (first + 1) % capacity;
        --count;
        return message;
    }
};

void Send(HostQueue* q, u64 message, bool front) {
    {
        std::unique_lock<std::mutex> hold(q->lock);
        q->not_full.wait(hold, [q] { return q->count < q->capacity; });
        front ? q->PushFront(message) : q->PushBack(message);
    }
    q->not_empty.notify_one();
}

int g_host_argc = 0;
char* g_no_arguments[] = {nullptr};
char** g_host_argv = g_no_arguments;

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

Result CreateThread(ThreadType* thread, ThreadFunction function, void* argument, void* stack, u64 stack_size,
                    s32 priority) {
    // TODO(M1): the engine's thread wrapper (fn_7100066744) creates its worker threads here with a
    // stack of its own; the host thread uses the host's stack, so stack and size are unused.
    (void)stack;
    (void)stack_size;
    auto* host = new (std::nothrow) HostThread{{}, thread, function, argument, priority, -1, 0, nullptr};
    if (!host)
        return ResultNotPorted();
    Attach(thread, host, kTagThread);
    return ResultSuccess();
}

void StartThread(ThreadType* thread) {
    // TODO(M1): fn_7100066744 starts each thread right after setting its core mask.
    HostThread* host = Find<HostThread>(thread, kTagThread, "StartThread");
    host->thread = std::thread(RunThread, host);
}

void WaitThread(ThreadType* thread) {
    // TODO(M1): the lifted fn_710002d298, a worker's teardown: signal its event, wait, destroy.
    HostThread* host = Find<HostThread>(thread, kTagThread, "WaitThread");
    if (host->thread.joinable())
        host->thread.join();
}

void DestroyThread(ThreadType* thread) {
    // TODO(M1): fn_710002d298, as WaitThread. The console waits for the thread to exit here too.
    HostThread* host = Find<HostThread>(thread, kTagThread, "DestroyThread");
    if (host->thread.joinable())
        host->thread.join();
    delete host;
    Detach(thread);
}

ThreadType* GetCurrentThread() {
    // TODO(M1): the default allocator keys a sub-allocator on the calling thread
    // (src/base/UnkDefaultAllocatorGlobal.cpp; fn_71000839c0, fn_7100132cd0, fn_7100132d30). A
    // thread the game created gets its own block back; the main thread and any other host thread
    // a block of the runtime's, so the pointer is stable and distinct per thread.
    if (t_current_thread)
        return t_current_thread;
    thread_local ThreadType s_thread{};
    return &s_thread;
}

void SleepThread(TimeSpan time) {
    // TODO(M1): fn_7100066854 (milliseconds), the sound system's buffer wait
    // (src/base/snd/CSoundSystemATK.cpp, 1 ms), fn_71000810b0, fn_71000810c0 and fn_71000faac4.
    if (time.nanoseconds > 0)
        std::this_thread::sleep_for(Duration(time));
    else
        std::this_thread::yield();
}

void SetThreadCoreMask(ThreadType* thread, s32 ideal_core, u64 affinity_mask) {
    // TODO(M1): fn_7100066744 pins each worker to core 0 or 1. The host schedules its threads
    // itself; the values are kept for a debugger.
    HostThread* host = Find<HostThread>(thread, kTagThread, "SetThreadCoreMask");
    host->ideal_core = ideal_core;
    host->affinity_mask = affinity_mask;
}

void SetThreadNamePointer(ThreadType* thread, const char* name) {
    // No caller in the boot slice yet; kept for a debugger like the core mask.
    Find<HostThread>(thread, kTagThread, "SetThreadNamePointer")->name = name;
}

s32 GetHostArgc() {
    // TODO(M1): fn_71000005d0, nnMain's first call, walks the arguments and ignores them.
    return g_host_argc;
}

char** GetHostArgv() {
    // TODO(M1): fn_71000005d0, as GetHostArgc.
    return g_host_argv;
}

void InitializeLightEvent(LightEventType* event, bool signaled, EventClearMode clear_mode) {
    // TODO(M1): fn_7100066744 (the two light events of every engine thread, unsignalled, auto
    // clear) and the event wrapper fn_7100066674.
    Attach(event, NewEvent(signaled, clear_mode), kTagLightEvent);
}

void FinalizeLightEvent(LightEventType* event) {
    // TODO(M1): the wrapper fn_7100066694, from fn_710021ea60.
    delete Find<HostEvent>(event, kTagLightEvent, "FinalizeLightEvent");
    Detach(event);
}

void SignalLightEvent(LightEventType* event) {
    // TODO(M1): the wrapper fn_7100066698.
    Find<HostEvent>(event, kTagLightEvent, "SignalLightEvent")->Signal();
}

void ClearLightEvent(LightEventType* event) {
    // No caller with a source yet.
    Find<HostEvent>(event, kTagLightEvent, "ClearLightEvent")->Clear();
}

void WaitLightEvent(LightEventType* event) {
    // TODO(M1): the wrappers fn_710006669c and fn_7100066864 (the thread object's second event).
    Find<HostEvent>(event, kTagLightEvent, "WaitLightEvent")->Wait();
}

bool TimedWaitLightEvent(LightEventType* event, TimeSpan timeout) {
    // TODO(M1): the wrapper fn_71000666a0, which converts milliseconds.
    return Find<HostEvent>(event, kTagLightEvent, "TimedWaitLightEvent")->TimedWait(timeout);
}

void InitializeEvent(EventType* event, bool signaled, EventClearMode clear_mode) {
    // No caller with a source yet; its users below need it first.
    Attach(event, NewEvent(signaled, clear_mode), kTagEvent);
}

void FinalizeEvent(EventType* event) {
    // TODO(M1): fn_710002d298, after the thread is destroyed.
    delete Find<HostEvent>(event, kTagEvent, "FinalizeEvent");
    Detach(event);
}

void SignalEvent(EventType* event) {
    // TODO(M1): fn_710002d298 wakes its worker to make it exit.
    Find<HostEvent>(event, kTagEvent, "SignalEvent")->Signal();
}

void ClearEvent(EventType* event) {
    Find<HostEvent>(event, kTagEvent, "ClearEvent")->Clear();
}

void WaitEvent(EventType* event) {
    Find<HostEvent>(event, kTagEvent, "WaitEvent")->Wait();
}

bool TryWaitEvent(EventType* event) {
    // TODO(M1): the lifted fn_710002a6b0 and fn_710002b0b0 poll one.
    return Find<HostEvent>(event, kTagEvent, "TryWaitEvent")->TryWait();
}

void InitializeSystemEventForPort(SystemEventType* event, bool signaled, EventClearMode clear_mode) {
    Attach(event, NewEvent(signaled, clear_mode), kTagSystemEvent);
}

void SignalSystemEventForPort(SystemEventType* event) {
    Find<HostEvent>(event, kTagSystemEvent, "SignalSystemEvent")->Signal();
}

void ClearSystemEvent(SystemEventType* event) {
    // TODO(M1): the lifted fn_710002b830, fn_710002b910 and fn_710002b9e0.
    Find<HostEvent>(event, kTagSystemEvent, "ClearSystemEvent")->Clear();
}

void DestroySystemEvent(SystemEventType* event) {
    delete Find<HostEvent>(event, kTagSystemEvent, "DestroySystemEvent");
    Detach(event);
}

void WaitSystemEvent(SystemEventType* event) {
    Find<HostEvent>(event, kTagSystemEvent, "WaitSystemEvent")->Wait();
}

bool TryWaitSystemEvent(SystemEventType* event) {
    return Find<HostEvent>(event, kTagSystemEvent, "TryWaitSystemEvent")->TryWait();
}

bool TimedWaitSystemEvent(SystemEventType* event, TimeSpan timeout) {
    // TODO(M1): the lifted fn_710002b130 and fn_7101215104.
    return Find<HostEvent>(event, kTagSystemEvent, "TimedWaitSystemEvent")->TimedWait(timeout);
}

void InitializeMessageQueue(MessageQueueType* queue, u64* buffer, u64 count) {
    // No caller with a source yet; the movie player's queues need it first.
    auto* host = new (std::nothrow) HostQueue;
    if (!host || count == 0)
        __builtin_trap();
    host->buffer = buffer;
    host->capacity = count;
    host->first = 0;
    host->count = 0;
    Attach(queue, host, kTagMessageQueue);
}

void FinalizeMessageQueue(MessageQueueType* queue) {
    delete Find<HostQueue>(queue, kTagMessageQueue, "FinalizeMessageQueue");
    Detach(queue);
}

void SendMessageQueue(MessageQueueType* queue, u64 message) {
    // TODO(M1): the lifted fn_7100224904 (the movie player).
    Send(Find<HostQueue>(queue, kTagMessageQueue, "SendMessageQueue"), message, false);
}

bool TrySendMessageQueue(MessageQueueType* queue, u64 message) {
    HostQueue* q = Find<HostQueue>(queue, kTagMessageQueue, "TrySendMessageQueue");
    {
        std::lock_guard<std::mutex> hold(q->lock);
        if (q->count == q->capacity)
            return false;
        q->PushBack(message);
    }
    q->not_empty.notify_one();
    return true;
}

void JamMessageQueue(MessageQueueType* queue, u64 message) {
    Send(Find<HostQueue>(queue, kTagMessageQueue, "JamMessageQueue"), message, true);
}

void ReceiveMessageQueue(u64* out, MessageQueueType* queue) {
    HostQueue* q = Find<HostQueue>(queue, kTagMessageQueue, "ReceiveMessageQueue");
    {
        std::unique_lock<std::mutex> hold(q->lock);
        q->not_empty.wait(hold, [q] { return q->count != 0; });
        *out = q->PopFront();
    }
    q->not_full.notify_one();
}

bool TryReceiveMessageQueue(u64* out, MessageQueueType* queue) {
    HostQueue* q = Find<HostQueue>(queue, kTagMessageQueue, "TryReceiveMessageQueue");
    {
        std::lock_guard<std::mutex> hold(q->lock);
        if (q->count == 0)
            return false;
        *out = q->PopFront();
    }
    q->not_full.notify_one();
    return true;
}

bool TimedReceiveMessageQueue(u64* out, MessageQueueType* queue, TimeSpan timeout) {
    HostQueue* q = Find<HostQueue>(queue, kTagMessageQueue, "TimedReceiveMessageQueue");
    {
        std::unique_lock<std::mutex> hold(q->lock);
        if (!q->not_empty.wait_for(hold, Duration(timeout), [q] { return q->count != 0; }))
            return false;
        *out = q->PopFront();
    }
    q->not_full.notify_one();
    return true;
}

}  // namespace nn::os

extern "C" void dread_port_set_host_arguments(int argc, char** argv) {
    nn::os::g_host_argc = argc;
    nn::os::g_host_argv = argv;
}
