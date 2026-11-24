// CChozoWarriorXWallClimbAttack's unit (tools/tu_map.py unit 719): whether the attack may start. Besides the
// base check the owner's life must be positive and, for a Chozo Warrior X AI, its actor must not be above the
// climb height, it must face the target (or, on a wall, no cutscene may run), the wall must be far enough
// away, and when the wall is nearer than the reach the AI's current attack must be the glaive spin. Names and
// layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntString.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/CChozoWarriorAttack.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::global::CRntString;
using base::math::CVector3D;
using base::reflection::CClass;

extern "C" f32 sqrtf(f32);

struct UnkWarriorAI {
    u8 pad_0x0[8];
};
struct UnkGoliathAI_b58a3;
struct UnkScorpiusAI_b69a4;

bool CChozoWarriorAIComponent_IsTargetAboveMinDistanceToFloor(UnkWarriorAI* ai);

// The name an attack reports (fn_7100360890): a counted entry whose key is at +0x30.
struct UnkAttackName_l48cde0 {
    u8 pad_0x0[0x30];
    s64 key;  // 0x30
    u8 pad_0x38[0xc];
    s32 reads;  // 0x44
};

struct UnkWallClimbLife_l48cde0 {
    u8 pad_0x0[0x54];
    f32 life;
};

struct UnkWallClimbAI_l48cde0 {
    virtual const CClass* vf_0x0();
    u8 pad_0x8[0x28];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x149a4 - 0x38];
    f32 height;  // 0x149a4
};

struct UnkWallClimbOwner_l48cde0 {
    u8 pad_0x0[0x38];
    u8 transform[0xc];  // 0x38
    f32 x;              // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x168];
    UnkWallClimbLife_l48cde0* life;  // 0x1d0
    u8 pad_0x1d8[0x68];
    UnkWallClimbAI_l48cde0* ai;  // 0x240
};

struct UnkWallClimbAttack_l48cde0 {
    u8 pad_0x0[0x8];
    UnkWallClimbOwner_l48cde0* owner;  // 0x8
};

static inline CVector3D Pos(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

static inline CVector3D Pos(const UnkWallClimbOwner_l48cde0* owner) {
    return {owner->x, owner->y, owner->z};
}

static inline const UnkMatrix44& Matrix(UnkWallClimbOwner_l48cde0* owner) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->transform));
}

// Counts a read of the name and gives its key.
static inline s64 Key(UnkAttackName_l48cde0* const* name) {
    ++(*name)->reads;
    return (*name)->key;
}

bool fn_710048cde0(UnkWallClimbAttack_l48cde0* self, void* arg, u32 mode) {
    if (!fn_7100493b00(static_cast<void*>(self), arg, mode))
        return false;
    UnkWallClimbOwner_l48cde0* owner = self->owner;
    if (owner->life && !(owner->life->life > 0.0f))
        return false;
    UnkWallClimbAI_l48cde0* ai = owner->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710048db44()))
        return true;
    f32 y = ai->actor->field_0x54;
    f32 limit = ai->height + 50.0f;
    if (limit < y)
        return false;
    UnkWarriorAI* warrior = reinterpret_cast<UnkWarriorAI*>(ai);
    if (CChozoWarriorAIComponent_IsTargetAboveMinDistanceToFloor(warrior)) {
        bool in_cutscene = IsPlayerGhostAuraStateIn3To6(g_unk_7101d47310);
        if (y > limit || in_cutscene)
            return false;
    } else {
        UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
        CVector3D d = Pos(target) - Pos(self->owner);
        f32 length = Length(d);
        if (!target)
            return false;
        f32 inv = 1.0f / (length + 0.0001f);
        d.z *= inv;
        d.y *= inv;
        d.x *= inv;
        const UnkMatrix44& m = Matrix(self->owner);
        f32 mx = m.m[2];
        f32 my = m.m[6];
        f32 mz = m.m[10];
        if (!(d.x * mx + d.y * my + d.z * mz >= 0.0f))
            return false;
    }
    u32 result = 0;
    f32 wall = 0.0f;
    fn_71010fc778(ai, &wall);
    if (wall > 200.0f) {
        f32 reach = 0.0f;
        fn_71010fc68c(reinterpret_cast<UnkGoliathAI_b58a3*>(ai), &reach);
        if (!(reach < wall) || CChozoWarriorAIComponent_IsTargetAboveMinDistanceToFloor(warrior)) {
            result = 1;
        } else {
            UnkScorpiusAI_b69a4* attacks = reinterpret_cast<UnkScorpiusAI_b69a4*>(ai);
            if (fn_7101105970(attacks)) {
                auto* name = reinterpret_cast<UnkAttackName_l48cde0* const*>(
                    fn_7100360890(static_cast<CAttack*>(fn_7101105970(attacks))));
                s64 id, spin;
                {
                    CRntString key("CHOZO_WARRIOR_X_GLAIVE_SPIN_ATTACK");
                    ++(*name)->reads;
                    id = Key(name);
                    spin = key.key;
                }
                result = id == spin;
            }
        }
    }
    return result != 0;
}
