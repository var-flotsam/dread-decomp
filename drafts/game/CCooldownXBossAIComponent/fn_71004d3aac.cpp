// CCooldownXBossAIComponent's unit (tools/tu_map.py): after the base step, on one of two event ids. The
// first sends the boss's character class one request and ends the phase (fn_71004d0cf4, fn_71004d3f44); the
// second picks a lava drop index (a tuned one or a random one below the attack's count), sends the phase 2
// action's request, sets the boss's two phase values and records where the target stands. Names and layouts
// are placeholders.
#include <stdlib.h>
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/CCooldownXBossFireBallDef.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkShakernautId.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkSelf;
void fn_71003ad45c(UnkSelf* self);
struct UnkCooldownXBossTimeline_b60a2;
void fn_71004d3f44(UnkCooldownXBossTimeline_b60a2* self);

extern CStrId g_unk_71726d82d0;
extern CStrId g_unk_71726d82c8;
extern UnkTunable g_unk_71726d37b8;
extern UnkTunable g_unk_71726d8260;

// What this function reads of the tunables.
struct UnkCooldownXIndexTunable_l4d3aac {
    u8 pad_0x0[0x24];
    s32 index;  // 0x24
};

// Two values per phase: the first goes to the boss's vf_0x168, the second to its vf_0x170.
struct UnkCooldownXBossTunable_l4d3aac {
    u8 pad_0x0[0x30];
    f32 phase0_first;   // 0x30
    f32 phase0_second;  // 0x34
    f32 phase1_first;   // 0x38
    f32 phase1_second;  // 0x3c
    f32 phase2_first;   // 0x40
    f32 phase2_second;  // 0x44
    f32 phase3_first;   // 0x48
    f32 phase3_second;  // 0x4c
};

struct UnkCooldownXLavaDrops_l4d3aac : base::reflection::UnkReflObject {
    u8 pad_0x8[0x168];
    s32 count;  // 0x170
};

struct UnkCooldownXLife_l4d3aac {
    virtual void vf_0x0();
    void SetFirst(f32 value, bool apply) {
        reinterpret_cast<void (*)(UnkCooldownXLife_l4d3aac*, f32, bool)>(
            (*reinterpret_cast<void***>(this))[0x168 / 8])(this, value, apply);
    }
    void SetSecond(f32 value) {
        reinterpret_cast<void (*)(UnkCooldownXLife_l4d3aac*, f32)>((*reinterpret_cast<void***>(this))[0x170 / 8])(
            this, value);
    }
};

struct UnkCooldownXOwner_l4d3aac {
    u8 pad_0x0[0x1b8];
    UnkCooldownXLife_l4d3aac* field_0x1b8;
    u8 pad_0x1c0[0x60];
    UnkAnimationComponent* field_0x220;
};

struct UnkCooldownXEvent_l4d3aac {
    u8 pad_0x0[0x10];
    CStrId id;  // 0x10
};

struct UnkCooldownXAI_l4d3aac {
    u8 pad_0x0[0x30];
    UnkCooldownXOwner_l4d3aac* owner;  // 0x30
    u8 pad_0x38[0x15150 - 0x38];
    UnkCooldownXLavaDrops_l4d3aac* attack;  // 0x15150
    u8 pad_0x15158[0x15650 - 0x15158];
    u32 field_0x15650;
    bool field_0x15654;
    u8 pad_0x15655[0x15680 - 0x15655];
    s32 phase;  // 0x15680
    u8 pad_0x15684[0x15750 - 0x15684];
    s32 drop_index;  // 0x15750
    u8 pad_0x15754[0x15820 - 0x15754];
    base::math::CVector3D target_pos;  // 0x15820
    u32 field_0x1582c;
    u32 field_0x15830;
    u8 pad_0x15834[2];
    bool field_0x15836;
};
static_assert(__builtin_offsetof(UnkCooldownXAI_l4d3aac, field_0x15836) == 0x15836, "layout");

void fn_71004d0cf4(UnkCooldownXAI_l4d3aac* self, bool a, bool b);

// Sends a request with mode 1 under the two ids; the caller owns the two arguments.
static inline void Send_l4d3aac(void* cls, u64 hash, base::reflection::UnkReflTypeIdArg& mode,
                                base::reflection::UnkReflTypeIdArg& mask) {
    mode.id = 1;
    mask.id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(cls, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

static inline u32 RandBelow(u32 n) { return n == 0 ? 0 : (rand() & 0x7fff) % n; }

static inline const UnkCooldownXBossTunable_l4d3aac& BossTunable() {
    return reinterpret_cast<const UnkCooldownXBossTunable_l4d3aac&>(g_unk_71726d8260);
}

void fn_71004d3aac(UnkCooldownXAI_l4d3aac* self, const UnkCooldownXEvent_l4d3aac* event) {
    base::reflection::UnkReflTypeIdArg end_mode, end_mask;
    UnkTrackQuery query;
    UnkShakernautId_b53 id;
    base::reflection::UnkReflTypeIdArg drop_mode, drop_mask;
    fn_71003ad45c(reinterpret_cast<UnkSelf*>(self));
    self->field_0x15650 = 0;
    UnkAnimationComponent* cls = self->owner->field_0x220;
    UnkCooldownXLife_l4d3aac* life = self->owner->field_0x1b8;
    if (!cls || !life)
        return;
    if (event->id == g_unk_71726d82d0) {
        Send_l4d3aac(cls, 0x1103a527ce41749b, end_mode, end_mask);
        fn_71004d0cf4(self, false, true);
        fn_71004d3f44(reinterpret_cast<UnkCooldownXBossTimeline_b60a2*>(self));
        return;
    }
    if (!(event->id == g_unk_71726d82c8))
        return;

    UnkCooldownXLavaDrops_l4d3aac* attack = self->attack;
    UnkCooldownXLavaDrops_l4d3aac* lava = fn_710009d50c(attack->vf_0x0(), fn_71004add20()) ? attack : nullptr;
    s32 count = lava->count;
    s32 limit = reinterpret_cast<const UnkCooldownXIndexTunable_l4d3aac&>(g_unk_71726d37b8).index;
    if (limit >= 0 && limit < count)
        self->drop_index = reinterpret_cast<const UnkCooldownXIndexTunable_l4d3aac&>(g_unk_71726d6438).index;
    else
        self->drop_index = RandBelow(count);

    u64 hash;
    if (self->field_0x15836) {
        hash = 0x6807ded87add32d0;
    } else {
        query.cls = fn_7100bf9f88();
        query.result = nullptr;
        query.value = -1.0f;
        fn_710117b08c(cls, &query);
        hash = query.result ? query.result->sources.data->Key() : 0xee0204a29a395d8e;
    }
    id.value = 0x80;
    fn_7101178564(reinterpret_cast<UnkShakernautCtrl_b53*>(cls), &id, true, false);
    Send_l4d3aac(cls, hash, drop_mode, drop_mask);

    const UnkCooldownXBossTunable_l4d3aac& tunable = BossTunable();
    switch (self->phase) {
    case 0: life->SetSecond(tunable.phase0_second); break;
    case 1: life->SetSecond(tunable.phase1_second); break;
    case 2: life->SetSecond(tunable.phase2_second); break;
    case 3: life->SetSecond(tunable.phase3_second); break;
    default: life->SetSecond(0.0f); break;
    }
    switch (self->phase) {
    case 0: life->SetFirst(tunable.phase0_first, true); break;
    case 1: life->SetFirst(tunable.phase1_first, true); break;
    case 2: life->SetFirst(tunable.phase2_first, true); break;
    case 3: life->SetFirst(tunable.phase3_first, true); break;
    default: life->SetFirst(0.0f, true); break;
    }

    self->field_0x1582c = 0;
    self->field_0x15830 = 0;
    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    base::math::CVector3D pos = {target->field_0x44, target->field_0x54, target->field_0x64};
    self->target_pos = pos;
    self->field_0x15654 = true;
    self->field_0x15836 = false;
}
