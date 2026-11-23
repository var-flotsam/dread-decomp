#pragma once

#include "base/UnkDefaultAllocator.h"

// Holds a mutex for its scope (through the engine's lock and unlock helpers).
struct UnkMutexLock {
    nn::os::MutexType* mutex;
    explicit UnkMutexLock(nn::os::MutexType* m) : mutex(m) { fn_710006691c(m); }
    ~UnkMutexLock() { fn_7100066920(mutex); }
};
