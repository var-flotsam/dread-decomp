// CEnemyLifeComponent's unit (tools/tu_map.py unit 2714): sends the owner's animation the hit's own
// animation when it names one, else the "hit from behind" one when the attacker stands behind the owner
// (against the way it faces), else the plain hit one. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/UnkTU1730.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkLifeActor_l15bef0 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    void* animation;  // 0x220
};
static_assert(__builtin_offsetof(UnkLifeActor_l15bef0, animation) == 0x220, "UnkLifeActor_l15bef0 layout");

struct UnkHitSource_l15bef0 {
    u8 pad_0x0[0x10];
    UnkLifeActor_l15bef0* actor;  // 0x10
};

struct UnkHit_l15bef0 {
    u8 pad_0x0[0x8];
    UnkHitSource_l15bef0* source;  // 0x8
    u8 pad_0x10[0x16];
    u8 mode;  // 0x26
    u8 pad_0x27[0x79];
    CStrId animation;  // 0xa0
};

struct UnkLifeTunables_l15bef0 {
    u8 pad_0x0[0x60];
    CStrId hit_animation;         // 0x60
    CStrId hit_behind_animation;  // 0x68
};

struct UnkEnemyLife_l15bef0 {
    u8 pad_0x0[0x30];
    UnkLifeActor_l15bef0* actor;  // 0x30
    u8 pad_0x38[0x260 - 0x38];
    UnkLifeTunables_l15bef0* tunables;  // 0x260
};

static inline bool IsEmpty(const CStrId& id) {
    return &g_unk_7101d47108->empty == id.data;
}

static inline void Send(void* animation, u64 hash, u32 mode_id, base::reflection::UnkReflTypeIdArg* mode,
                        base::reflection::UnkReflTypeIdArg* mask) {
    mode->id = mode_id;
    mask->id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(animation, hash, 0xf4061ff25a2163d6ull, mode, &g_unk_710164b958, &g_unk_7172769740, mask);
}

void fn_710115bef0(UnkEnemyLife_l15bef0* self, UnkHit_l15bef0* hit) {
    UnkLifeActor_l15bef0* actor = self->actor;
    void* animation = actor->animation;
    if (!animation)
        goto done;
    // The goto above skips these declarations, so they get no lifetime markers and keep slots of their own.
    u32 mode_id;
    base::reflection::UnkReflTypeIdArg own_mode;
    base::reflection::UnkReflTypeIdArg own_mask;
    base::reflection::UnkReflTypeIdArg behind_mode;
    base::reflection::UnkReflTypeIdArg behind_mask;
    base::reflection::UnkReflTypeIdArg front_mode;
    base::reflection::UnkReflTypeIdArg front_mask;
    UnkLifeActor_l15bef0* attacker;
    mode_id = hit->mode << 3;

    if (!IsEmpty(hit->animation)) {
        Send(animation, hit->animation.data->Key(), mode_id, &own_mode, &own_mask);
        return;
    }

    attacker = hit->source ? hit->source->actor : nullptr;
    if (actor && attacker) {
        {
            f32 attacker_x = attacker->xform.m[0][3];
            f32 x = actor->xform.m[0][3];
            UnkVec3f angle = actor->xform.GetAngle();
            const CStrId& behind = self->tunables->hit_behind_animation;
            if (!IsEmpty(behind)) {
                f32 facing = angle.y > 0.0f ? 1.0f : -1.0f;
                bool facing_right = facing > 0.0f;
                bool facing_left = facing < 0.0f;
                f32 dx = attacker_x - x;
                bool from_left = dx < 0.0f;
                bool from_right = dx > 0.0f;
                if ((from_left & facing_right) | (from_right & facing_left)) {
                    Send(self->actor->animation, behind.data->Key(), mode_id, &behind_mode, &behind_mask);
                    return;
                }
            }
        }
    }

    if (!IsEmpty(self->tunables->hit_animation))
        Send(self->actor->animation, self->tunables->hit_animation.data->Key(), mode_id, &front_mode, &front_mask);
done:;
}
