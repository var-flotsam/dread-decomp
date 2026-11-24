// CBossAIComponent's unit (tools/tu_map.py unit 549): updates the boss's combat start. While inactive, hands the
// camera back when the settings ask for it and the boss is not busy; once active, unlocks the doors and (unless
// the mode flags forbid it) hands the game manager "<owner name>_startcombat" with the lazily resolved target's
// name, and the combat id when it is not empty. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkModeFlags.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU57.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;

struct UnkBossDoorAI_ovb0102;
struct UnkCameraOwner;
void CBossAIComponent_LockDoors(UnkBossDoorAI_ovb0102* self, bool locked);

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

struct UnkBossActor_l3b0754 {
    u8 pad_0x0[0x30];
    UnkStringData* name;
};

struct UnkBossTarget_l3b0754 : base::UnkRefCounted {
    UnkBossActor_l3b0754* actor;
};

struct UnkBossSettings_l3b0754 {
    u8 pad_0x0[0x50c];
    bool field_0x50c;
    u8 pad_0x50d[0x513 - 0x50d];
    bool field_0x513;
};

struct UnkBossAI_l3b0754 {
    void** vtable;
    u8 pad_0x8[0x30 - 0x8];
    UnkBossActor_l3b0754* owner;
    u8 pad_0x38[0x148d8 - 0x38];
    bool active;
    u8 pad_0x148d9[3];
    u32 field_0x148dc;
    bool field_0x148e0;
    u8 pad_0x148e1[0x14908 - 0x148e1];
    UnkBossSettings_l3b0754* settings;
    u8 pad_0x14910[0x14918 - 0x14910];
    u8 source[0x28];
    u32 state;
    u8 pad_0x14944[4];
    base::UnkRefHandle handle;
    UnkStringData* combat_id;

    // vf_0x2d8: whether the boss is busy.
    bool IsBusy() {
        typedef bool (*BusyFn)(UnkBossAI_l3b0754*);
        return reinterpret_cast<BusyFn>(vtable[0x2d8 / 8])(this);
    }

    // The target behind the actor reference, resolving it first in state 1.
    UnkBossTarget_l3b0754* Target() {
        if (state == 1) {
            {
                UnkRefPtr fresh = fn_710023b580(source);
                fn_7100234f78(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        return static_cast<UnkBossTarget_l3b0754*>(handle.ptr);
    }
};
static_assert(__builtin_offsetof(UnkBossAI_l3b0754, combat_id) == 0x14950, "layout");

void fn_71003b0300(UnkBossAI_l3b0754* self);

void fn_71003b0754(UnkBossAI_l3b0754* self) {
    bool active = self->active;
    self->field_0x148dc = 0;
    if (!active) {
        if (self->settings->field_0x513 && !self->IsBusy())
            fn_71003ad174(reinterpret_cast<UnkCameraOwner*>(self), 0);
        return;
    }
    self->field_0x148e0 = false;
    if (!self->settings->field_0x50c)
        fn_71003b0300(self);
    CBossAIComponent_LockDoors(reinterpret_cast<UnkBossDoorAI_ovb0102*>(self), false);
    if (g_unk_71726bc570->field_0xb8)
        return;
    UnkBossTarget_l3b0754* target = self->Target();
    if (target && target->actor && fn_7100dfbe3c(g_unk_7101d47310)) {
        CRntString tag(StringOf(self->owner->name));
        CRntString_AppendCString(&tag, "_startcombat");
        if (self->combat_id != &g_unk_7101d47108->empty)
            fn_7100dfc344(g_unk_7101d47310, TextOf(self->combat_id), reinterpret_cast<const char* const&>(tag.data),
                          StringOf(self->Target()->actor->name), true);
        else
            fn_7100dfaf88(g_unk_7101d47310, &tag, StringOf(self->Target()->actor->name), true);
    }
}
