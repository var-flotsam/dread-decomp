#pragma once

#include "base/UnkErasedVector.h"
#include "game/UnkTU341.h"
#include "types.h"

// The callback set behind a lazily resolved handle: callbacks at +0x8, the state at +0x30 (1: not
// resolved yet, 2: resolved) and the handle at +0x38. The base copy constructor is
// fn_7100da4dcc (the same as src/game/CLogicCamera/CLogicCamera_Value_ovb0070.cpp's).
// The callback set behind the camera's lazily resolved handle: callbacks at +0x8, the state at
// +0x30 (1: not resolved yet, 2: resolved) and the handle at +0x38.
struct UnkCameraCallbackBase_ovb0070 {
    void** vtable;
    UnkOwnedErasedVector callbacks;
    u32 state;

    UnkCameraCallbackBase_ovb0070(const UnkCameraCallbackBase_ovb0070& other) asm("fn_7100da4dcc");
    ~UnkCameraCallbackBase_ovb0070() { vtable = &g_unk_7101a20998[2]; }
};
