#pragma once

#include "game/CAnimationComponent.h"
#include "game/UnkVtableHolder_l1757f4.h"
#include "types.h"

// The listener lists: active, pending and queued, and the lock count of the passes over them.
struct UnkAnimListenerSet_l1757f4 : UnkVtableHolder_l1757f4 {
    base::global::CRntVector<void*> active;   // 0x8
    base::global::CRntVector<void*> pending;  // 0x30
    base::global::CRntVector<void*> queued;   // 0x58
    s32 lock;                   // 0x80

    UnkAnimListenerSet_l1757f4()
        : UnkVtableHolder_l1757f4(g_unk_7101cb4880 + 2), active(0xf), pending(0xf), queued(0xf), lock(0) {}
    ~UnkAnimListenerSet_l1757f4() {
        vtable = g_unk_7101cb4880 + 2;
        queued.Free();
        pending.Free();
        active.Free();
    }
};
