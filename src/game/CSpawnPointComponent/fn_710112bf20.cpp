// From CSpawnPointComponent's unit: the id (+0x30) of the actor the holder at +0x98 points at, or the
// empty id. In state 1 it first builds a counted reference with fn_710023b580 on the member at +0x68,
// hands it to the holder (fn_7100234f78) and moves to state 2 (as fn_71003b0160,
// src/game/CBossAIComponent/fn_71003b0160.cpp). Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"

using base::global::CStrId;

struct UnkSpawnTarget {
    u8 pad_0x0[0x30];
    CStrId id;
};

// The weak handle the holder refers to (base::UnkRefCounted's layout); its target is at +0x10.
struct UnkSpawnTargetHolder : base::UnkRefCounted {
    UnkSpawnTarget* field_0x10;
};

struct UnkSpawnPointLink {
    u8 pad_0x0[0x68];
    u8 field_0x68[0x28];
    u32 field_0x90;
    u8 pad_0x94[4];
    base::UnkRefHandle field_0x98;
};

const CStrId* fn_710112bf20(UnkSpawnPointLink* self) {
    if (self->field_0x90 == 1) {
        {
            UnkRefPtr reference = fn_710023b580(self->field_0x68);
            // The reference's pointer is the handle's.
            fn_7100234f78(&self->field_0x98, reinterpret_cast<const base::UnkRefHandle*>(&reference));
        }
        self->field_0x90 = 2;
    }
    UnkSpawnTargetHolder* holder = static_cast<UnkSpawnTargetHolder*>(self->field_0x98.ptr);
    if (holder && holder->field_0x10)
        return &holder->field_0x10->id;
    return &g_unk_7101d470e0;
}
