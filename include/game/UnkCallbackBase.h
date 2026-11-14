#pragma once

// What every callback-set class shares: the refcounted owner a set holds, and the base of a set (a
// vtable, an owned type-erased array of callbacks and a count; 0x38 bytes). The set classes derive
// from it: UnkCallbackSet_7101a20950 (game/UnkCallbackSet_7101a20950.h) and the classes that the
// files using them still define for themselves (UnkCallbackSet, vtable 0x7101a33fa0, in
// src/game/CCharClassWaterPoolComponent/fn_710090dbb4.cpp; UnkCallbackSet_7101cb2880 in
// src/game_s7/fn_7101132350.cpp). Names are placeholders.

#include "base/UnkErasedVector.h"

struct UnkRefCounted {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

struct UnkCallbackBase {
    virtual void vf0();
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    UnkCallbackBase() : field_0x30(0) {}
};
