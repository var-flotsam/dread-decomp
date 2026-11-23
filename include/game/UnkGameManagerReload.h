#pragma once

#include "base/UnkDefaultAllocator.h"
#include "game/UnkReloadCallback.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU83.h"
#include "types.h"

struct UnkStrVector;

// The game manager's members the reload uses (game/UnkGameManager.h leaves them as padding).
struct UnkGameManagerReload {
    u8 pad_0x0[0x748];
    UnkReloadCallback field_0x748;  // the pending load's callback
    u8 field_0x758[0x28];  // a string list (UnkStrVector)
    u8 pad_0x780[0x48];
    nn::os::MutexType field_0x7c8;
    u8 pad_0x7e8[0x8];
    base::global::CRntString field_0x7f0;
    u8 pad_0x820[0x3d8];
    u8 field_0xbf8[1];  // a blackboard
    u8 pad_0xbf9[0xeb7];
    UnkReloadCallback field_0x1ab0;  // the saved callback
    u8 field_0x1ac0[0x28];  // the saved string list (UnkStrVector)

    // Sets the pending load's string list and callback under the lock.
    void SetPending(u8* list, const UnkReloadCallback* callback) {
        fn_710006691c(&field_0x7c8);
        fn_71000e0524(reinterpret_cast<UnkStrVector*>(field_0x758), reinterpret_cast<UnkStrVector*>(list));
        field_0x748.context = callback->context;
        field_0x748.fn = callback->fn;
        fn_7100066920(&field_0x7c8);
    }
};
