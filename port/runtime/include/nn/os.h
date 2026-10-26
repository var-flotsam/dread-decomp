#pragma once

// nn::os: threads, synchronisation, time and memory. What the boot path needs, roughly in order:
// the host arguments (fn_71000005d0, from nnMain); the system tick (frame timers,
// src/game_s1/fn_7100080c40.cpp); mutexes (the default allocator, include/base/UnkDefaultAllocator.h,
// locks one around every allocation); the current thread (src/base/UnkDefaultAllocatorGlobal.cpp
// keys a sub-allocator on it); the engine's thread wrapper (fn_7100066744: CreateThread,
// SetThreadCoreMask, StartThread, two light events next to the thread) and its event wrappers
// (fn_7100066674 to fn_7100066864); then events, system events and message queues for the loader,
// sound and movie threads, and the memory block and heap calls behind the 1.82 GB .bss
// sub-allocation (docs/PORT_PLAN.md).
//
// `main` imports these (data/functions/, name_source symbol):
//   AllocateMemoryBlock, SetMemoryHeapSize, QueryMemoryInfo,
//   CreateThread, StartThread, WaitThread, DestroyThread, GetCurrentThread, SleepThread,
//   SetThreadCoreMask, SetThreadNamePointer,
//   InitializeMutex, FinalizeMutex, LockMutex, UnlockMutex,
//   InitializeEvent, FinalizeEvent, SignalEvent, ClearEvent, WaitEvent, TryWaitEvent,
//   InitializeLightEvent, FinalizeLightEvent, SignalLightEvent, ClearLightEvent, WaitLightEvent,
//   TimedWaitLightEvent,
//   InitializeMessageQueue, FinalizeMessageQueue, SendMessageQueue, TrySendMessageQueue,
//   JamMessageQueue, ReceiveMessageQueue, TryReceiveMessageQueue, TimedReceiveMessageQueue,
//   InitializeMultiWait, FinalizeMultiWait, InitializeMultiWaitHolder, FinalizeMultiWaitHolder,
//   LinkMultiWaitHolder, UnlinkMultiWaitHolder, WaitAny, GetMultiWaitHolderUserData,
//   SetMultiWaitHolderUserData,
//   ClearSystemEvent, DestroySystemEvent, WaitSystemEvent, TryWaitSystemEvent, TimedWaitSystemEvent,
//   GetSystemTick, ConvertToTick, ConvertToTimeSpan, GetHostArgc, GetHostArgv.
// Declared below: everything but the memory calls and the multi-waits, which no boot-path function
// and no source calls yet. src/ itself names only MutexType, GetCurrentThread, SleepThread,
// GetSystemTick and ConvertToTimeSpan; the rest is reached through the imports' four-byte thunks
// (fn_71012...), which tools/port_link.py aliases to these definitions by name. The engine reaches
// the mutex calls through four-byte wrappers (fn_71000668f8, fn_7100066918, fn_710006691c,
// fn_7100066920, declared in include/base/UnkDefaultAllocator.h).
//
// Every object the game embeds keeps the game's size, and the port stores a pointer to its own
// host object in the first bytes (with a tag word, so that a call on an object nobody initialised
// stops with a message instead of following garbage); the sizes below are the game's, from the
// offsets its objects place them at.

#include "nn/nn_common.h"
#include "nn/os/os_TickTypes.h"

namespace nn::os {

// A mutex as the game embeds it in its objects. Size and alignment are fixed by the game's
// layouts (include/base/UnkDefaultAllocator.h has one at +0x38 followed by a member at +0x58),
// so the port keeps the 32 bytes and stores a handle to its own host lock inside them.
struct alignas(8) MutexType {
    u8 storage[0x20];
};
static_assert(sizeof(MutexType) == 0x20, "layout fixed by the game's objects");

void InitializeMutex(MutexType* mutex, bool recursive, s32 lock_level);
void FinalizeMutex(MutexType* mutex);
void LockMutex(MutexType* mutex);
void UnlockMutex(MutexType* mutex);

// Whether a wait consumes the signal (auto clear) or leaves it set until ClearEvent (manual).
// The engine's thread wrapper creates its light events with mode 1 and its event wrapper
// (fn_7100066674) passes the caller's flag, so 1 is auto clear.
enum EventClearMode : s32 {
    EventClearMode_ManualClear = 0,
    EventClearMode_AutoClear = 1,
};

// A thread control block. fn_7100066744 places one at +0x0 of the engine's thread object and its
// two light events at +0x1c0 and +0x1cc, so the block is 0x1c0 bytes.
struct alignas(8) ThreadType {
    u8 storage[0x1c0];
};
static_assert(sizeof(ThreadType) == 0x1c0, "fn_7100066744 puts a light event right after it");

using ThreadFunction = void (*)(void* argument);

// The engine's thread wrapper (fn_7100066744) passes its own stack (0x10000 bytes from the default
// allocator) and priority 2 or 0x1b. The port runs the function on a host thread with the host's
// stack and priority.
Result CreateThread(ThreadType* thread, ThreadFunction function, void* argument, void* stack,
                    u64 stack_size, s32 priority);
void StartThread(ThreadType* thread);
void WaitThread(ThreadType* thread);
void DestroyThread(ThreadType* thread);
// The block CreateThread was given, on a thread the game created; on any other host thread (the
// main thread first) a block of the runtime's own, stable and distinct per thread.
ThreadType* GetCurrentThread();
void SleepThread(TimeSpan time);
void SetThreadCoreMask(ThreadType* thread, s32 ideal_core, u64 affinity_mask);
void SetThreadNamePointer(ThreadType* thread, const char* name);

// What nnMain's argument walk (fn_71000005d0) reads: the port's main passes its own arguments
// (dread_port_set_host_arguments in dread_port.h).
s32 GetHostArgc();
char** GetHostArgv();

// A light event: 12 bytes, 4-aligned (the engine's thread object has two back to back at +0x1c0
// and +0x1cc, then a pointer at +0x1d8).
struct alignas(4) LightEventType {
    u8 storage[0xc];
};
static_assert(sizeof(LightEventType) == 0xc, "fn_7100066744 lays two out 12 bytes apart");

void InitializeLightEvent(LightEventType* event, bool signaled, EventClearMode clear_mode);
void FinalizeLightEvent(LightEventType* event);
void SignalLightEvent(LightEventType* event);
void ClearLightEvent(LightEventType* event);
void WaitLightEvent(LightEventType* event);
bool TimedWaitLightEvent(LightEventType* event, TimeSpan timeout);

// An event. Its callers (the lifted fn_710002a6b0, fn_710002b0b0, fn_710002d298) pass only the
// pointer, so the size is not known yet and the type stays opaque until a source embeds one. The
// port uses the first 16 bytes, 8-aligned, which the console's event (a mutex, a condition and a
// flag) exceeds.
struct EventType;

void InitializeEvent(EventType* event, bool signaled, EventClearMode clear_mode);
void FinalizeEvent(EventType* event);
void SignalEvent(EventType* event);
void ClearEvent(EventType* event);
void WaitEvent(EventType* event);
bool TryWaitEvent(EventType* event);

// A system event: on the console an event a service signals (a kernel handle pair); on PC the
// runtime's own stubs create and signal them. Opaque like EventType, with the same first 16 bytes.
// ClearSystemEvent is called by the lifted fn_710002b830, fn_710002b910 and fn_710002b9e0,
// TimedWaitSystemEvent by fn_710002b130 and fn_7101215104.
struct SystemEventType;

void ClearSystemEvent(SystemEventType* event);
void DestroySystemEvent(SystemEventType* event);
void WaitSystemEvent(SystemEventType* event);
bool TryWaitSystemEvent(SystemEventType* event);
bool TimedWaitSystemEvent(SystemEventType* event, TimeSpan timeout);
// Not an import: how the port's own stubs (nn::oe's notification event, nn::hid, movie) make a
// system event the game then waits on.
void InitializeSystemEventForPort(SystemEventType* event, bool signaled, EventClearMode clear_mode);
void SignalSystemEventForPort(SystemEventType* event);

// A bounded queue of pointer-sized words over a buffer the caller provides. The movie player
// (the lifted fn_7100224904) sends to one. Opaque like EventType: the port uses the
// first 16 bytes and keeps the messages in the caller's buffer, as the console does.
struct MessageQueueType;

void InitializeMessageQueue(MessageQueueType* queue, u64* buffer, u64 count);
void FinalizeMessageQueue(MessageQueueType* queue);
void SendMessageQueue(MessageQueueType* queue, u64 message);
bool TrySendMessageQueue(MessageQueueType* queue, u64 message);
// Puts the message at the front: the next receive returns it.
void JamMessageQueue(MessageQueueType* queue, u64 message);
void ReceiveMessageQueue(u64* out, MessageQueueType* queue);
bool TryReceiveMessageQueue(u64* out, MessageQueueType* queue);
bool TimedReceiveMessageQueue(u64* out, MessageQueueType* queue, TimeSpan timeout);

}  // namespace nn::os
