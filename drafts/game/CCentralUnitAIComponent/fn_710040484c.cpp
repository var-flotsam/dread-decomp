// CCentralUnitAIComponent's unit (tools/tu_map.py unit 614): the damage handler. When the base handler
// (fn_7101105e44) lets a hit through, the damage adds up (a flagged hit takes it to the maximum,
// fn_7100408770); reaching the maximum explodes the unit once: the music changes, the blackboard gets
// "bHasExploded" and the owner's +0x220 object two requests and an animation. Then the point the hit
// pushes towards is set in front of the owner. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/CBossAIComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2702.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CStrId;

// Declared with this type in src/game/CCharClassCentralUnitAIComponent/fn_7100408770.cpp.
struct UnkSelf;
f32 fn_7100408770(UnkSelf* self);

struct UnkCentralVec_l40484c {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkCentralUnitHit_l40484c {
    u8 pad_0x0[0x20];
    bool handled;  // 0x20
    u8 pad_0x21[0x57];
    UnkCentralVec_l40484c dir;  // 0x78
    u8 pad_0x84[0x30];
    u32 flags;  // 0xb4: bit 0 a near push, bit 15 a hit that destroys at once
    u8 pad_0xb8[0x40];
    f32 damage;  // 0xf8
};
static_assert(__builtin_offsetof(UnkCentralUnitHit_l40484c, damage) == 0xf8, "UnkCentralUnitHit_l40484c layout");

struct UnkCentralUnitDamageOwner_l40484c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44: the transform's translation column
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1b8];
    void* field_0x220;
};
static_assert(__builtin_offsetof(UnkCentralUnitDamageOwner_l40484c, field_0x220) == 0x220, "layout");

struct UnkCentralUnitDamageAI_l40484c {
    u8 pad_0x0[0x30];
    UnkCentralUnitDamageOwner_l40484c* owner;
    u8 pad_0x38[0x1260 - 0x38];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x146c8 - 0x1350];
    UnkSelf* core;  // 0x146c8: what the maximum damage is read from
    u8 pad_0x146d0[0x28];
    bool exploded;  // 0x146f8
    u8 pad_0x146f9[0x27];
    UnkCentralVec_l40484c push;  // 0x14720: where the hit pushes towards
    f32 damage;  // 0x1472c
};
static_assert(__builtin_offsetof(UnkCentralUnitDamageAI_l40484c, damage) == 0x1472c, "layout");

static inline UnkMgr_0xf18* MusicManager_l40484c() {
    if (!g_unk_71726bc560) {
        UnkMgr_0xf18* created = static_cast<UnkMgr_0xf18*>(fn_71000839a0(0xf18, 0x3f));
        fn_7100fd3804(created);
        g_unk_71726bc560 = created;
    }
    return g_unk_71726bc560;
}

// Stores the flag at `flag` under `name` in the blackboard section `section` names (unless the
// blackboard is locked), through the caller's typed value.
inline void SetSectionBool_l40484c(CBlackboard* blackboard, const CStrId& section, CStrId name, bool* flag,
                                   UnkTypedValue* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            value->type = fn_710000d228();
            value->value = flag;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&name), value);
        }
    }
}

u32 fn_710040484c(UnkCentralUnitDamageAI_l40484c* self, UnkCentralUnitHit_l40484c* hit) {
    u32 result = fn_7101105e44(reinterpret_cast<UnkSluggerAI*>(self), reinterpret_cast<UnkSluggerEvent*>(hit));
    if (result)
        return result;
    if (!self->exploded) {
        f32 damage = hit->damage + self->damage;
        self->damage = damage;
        if (hit->flags & 0x8000) {
            damage = fn_7100408770(self->core);
            self->damage = damage;
        }
        if (damage >= fn_7100408770(self->core)) {
            UnkTypedValue value;
            self->exploded = true;
            fn_7100fdb9e0(MusicManager_l40484c(), 3, 0, -1.0f, 0.0f);
            SetSectionBool_l40484c(&self->blackboard, g_unk_71726b7cb8, CStrId("bHasExploded", true), &self->exploded,
                                   &value);
            if (void* target = self->owner->field_0x220) {
                // Every request local at the block's scope: none of their stack slots is shared, as in
                // the original.
                base::reflection::UnkReflTypeIdArg mode, mask, mode2, mask2;
                mode.id = 0;
                mask.id = UnkAll_SpitSend().value;
                fn_7101176690(target, 0xcf13234adbf96d1d, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                              &g_unk_7172769740, &mask);
                void* target2 = self->owner->field_0x220;
                mode2.id = 0;
                mask2.id = UnkAll_SpitSend().value;
                fn_7101176690(target2, 0x01b8e151f1cf0ee5, 0xcf7107181622cb23, &mode2, &g_unk_710164b958,
                              &g_unk_7172769740, &mask2);
                fn_710117662c(static_cast<UnkActorTarget*>(self->owner->field_0x220), 0xd, 0, false, false, -1.0f);
            }
        }
        fn_71008c1538(reinterpret_cast<UnkRequest*>(hit), 0.0f);
        hit->handled = true;
    }
    f32 distance = (hit->flags & 1) ? 20.0f : 50.0f;
    UnkCentralUnitDamageOwner_l40484c* owner = self->owner;
    UnkCentralVec_l40484c push = {owner->x + distance * hit->dir.x, owner->y + distance * hit->dir.y,
                                  owner->z + distance * hit->dir.z};
    self->push = push;
    return result;
}
