#pragma once

// nn::os: threads, synchronisation, time and memory. What the boot path needs, roughly in order:
// the system tick (frame timers, src/game_s1/fn_7100080c40.cpp); mutexes (the default allocator,
// include/base/UnkDefaultAllocator.h, locks one around every allocation); the current thread
// (src/base/UnkDefaultAllocatorGlobal.cpp keys a sub-allocator on it); then threads, events,
// light events, message queues and multi-waits for the loader and render threads, and the memory
// block and heap calls behind the 1.82 GB .bss sub-allocation (docs/PORT_PLAN.md).
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
// Only what src/ uses today is declared; each of the others is added with the function that
// needs it, in the game's layout where the game embeds the object.

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

// A thread control block. The game embeds these in its own objects, so the real size matters
// once CreateThread is ported (M1); until then it is opaque and only the pointer is used.
struct ThreadType;

ThreadType* GetCurrentThread();

}  // namespace nn::os
