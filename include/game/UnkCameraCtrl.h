#pragma once

// The camera controller current in the camera holder fn_7100defcec gives (game/UnkGameManager.h):
// a reflected object (its class at vtable slot 0) that callers test against CBossCameraCtrl
// (src/game/CChozoCommanderKiGrabAttack/fn_7100430830.cpp) and CAnimatedCameraCtrl (src/game/CSubAreaManager.cpp), both
// CCameraCtrl classes in reflection. Only the fields those callers read are laid out; names are
// placeholders.

#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkCameraCtrl {
    void** vtable;
    u8 pad_0x8[0x18];
    base::global::CStrId field_0x20;  // its id

    // The object's class (vtable slot 0).
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkCameraCtrl*)>(vtable[0])(this);
    }
};

// The holder's current camera controller (the first word of what its +0x60 points to), or null
// when +0x60 is. Not decompiled yet.
UnkCameraCtrl* fn_71010e2080(void* holder);

// The block at +0x38 of the object the camera holder's reference at +0x60 points to, or a constant
// default one (src/game/CCameraComponent/fn_71010e537c.cpp).
const void* fn_71010e537c(void* holder);
