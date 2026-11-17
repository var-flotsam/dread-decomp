// Unit 1176 (hint 'CCentralUnitComponent'): starts the central unit's combat. In state 2 of the state machine
// at +0x30, when the lazily resolved actor reference (source +0x978, state +0x9a0, handle +0x9a8) has a target
// and the game manager allows it (fn_7100dfbe3c), hands the manager "<owner name>_startcombat" with the
// target's name (and the id at +0x9b0 when it is not empty), then requests state 3 ("none" 8). Names and
// layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU57.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;

// The id's text, counting the read.
static inline const char* const& TextOf(UnkStringData* const& id) {
    ++id->reads;
    return id->text;
}

// The id's whole string, counting the read.
static inline const CRntString& StringOf(UnkStringData* const& id) {
    ++id->reads;
    return id->String();
}

struct UnkCentralUnitActor_l77561c {
    u8 pad_0x0[0x30];
    UnkStringData* name;
};

struct UnkCentralUnitTarget_l77561c : base::UnkRefCounted {
    UnkCentralUnitActor_l77561c* actor;
};

struct UnkCentralUnitProto_l77561c {
    u8 pad_0x0[0x18];
    UnkCentralUnitActor_l77561c* owner;
    u8 pad_0x20[0x30 - 0x20];
    UnkSmMachine_A5_b73 machine;
    u8 pad_0xe0[0x978 - 0xe0];
    u8 source[0x28];
    u32 state;
    u8 pad_0x9a4[4];
    base::UnkRefHandle handle;
    UnkStringData* combat_id;

    // The target behind the actor reference, resolving it first in state 1.
    UnkCentralUnitTarget_l77561c* Target() {
        if (state == 1) {
            {
                UnkRefPtr fresh = fn_710023b580(source);
                fn_7100234f78(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        return static_cast<UnkCentralUnitTarget_l77561c*>(handle.ptr);
    }
};
static_assert(__builtin_offsetof(UnkCentralUnitProto_l77561c, combat_id) == 0x9b0, "layout");

void fn_710077561c(UnkCentralUnitProto* unit) {
    auto* self = reinterpret_cast<UnkCentralUnitProto_l77561c*>(unit);
    if (!self->machine.current || self->machine.current->id != 2)
        return;
    UnkCentralUnitTarget_l77561c* target = self->Target();
    if (target && target->actor && fn_7100dfbe3c(g_unk_7101d47310)) {
        CRntString tag(StringOf(self->owner->name));
        fn_710007f564(&tag, "_startcombat");
        if (self->combat_id != &g_unk_7101d47108->empty)
            fn_7100dfc344(g_unk_7101d47310, TextOf(self->combat_id), reinterpret_cast<const char* const&>(tag.data),
                          StringOf(self->Target()->actor->name), true);
        else
            fn_7100dfaf88(g_unk_7101d47310, &tag, StringOf(self->Target()->actor->name), true);
    }
    RequestState_A5_b73<15, 8>(&self->machine, 3);
}
