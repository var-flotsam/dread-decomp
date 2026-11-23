#pragma once

#include "types.h"

struct UnkShotWeakTarget;

// The layout of UnkShotWeakRef in src/game/CShotManager.cpp: a weak reference, empty when its
// generation is -1. Its fields are written untyped (the original reloads the array after each new
// element) and it is not trivially copyable (the original copies element by element).
struct UnkShotWeakRef {
    UnkShotWeakTarget* target;
    s64 generation;

    UnkShotWeakRef() {
        UnkShotWeakTarget* none = nullptr;
        s64 invalid = -1;
        target = none;
        generation = invalid;
    }
    ~UnkShotWeakRef() {}
};
