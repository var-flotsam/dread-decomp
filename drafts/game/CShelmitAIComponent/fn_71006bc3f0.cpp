// CShelmitAIComponent's unit (tools/tu_map.py unit 1054): a message handler override. While the attack runs
// (mode 1 at +0x14780) a flagged message either stops the owner (fn_7100232c68) when its attack component is
// done, or, once the abort track has played out (sub-mode 1), sends the owner's track again and puts the owner
// back at its saved position and angles (+0x14748); otherwise the owner stops when it is too far from its
// target. Other modes go to the base handler (fn_7101111dc8). Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CAnimationComponent.h"
#include "game/CAttackComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU341.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkNailongAI_b46a2;
struct UnkNailongMsg_b46a2;

struct UnkShelmitTunable_l6bc3f0 {
    u8 pad_0x0[0x34];
    f32 distance;  // 0x34
};

struct UnkShelmitAttack_l6bc3f0 {
    void** vtable;

    void Stop() { reinterpret_cast<void (*)(UnkShelmitAttack_l6bc3f0*)>(vtable[0x178 / 8])(this); }
};

struct UnkShelmitOwner_l6bc3f0 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x278 - 0x228];
    UnkShelmitAttack_l6bc3f0* attack;  // 0x278
    u8 pad_0x280[0x3b0 - 0x280];
    bool moved;  // 0x3b0
    u8 pad_0x3b1[0x3c0 - 0x3b1];
    bool turned;  // 0x3c0

    void OnTransformChanged(UnkXform* x, u32* what) {
        reinterpret_cast<void (*)(UnkShelmitOwner_l6bc3f0*, UnkXform*, u32*)>(vtable[0x118 / 8])(this, x, what);
    }
};
static_assert(__builtin_offsetof(UnkShelmitOwner_l6bc3f0, turned) == 0x3c0, "layout");

struct UnkShelmitMsg_l6bc3f0 {
    u8 pad_0x0[0x20];
    bool field_0x20;
};

struct UnkShelmitAI_l6bc3f0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkShelmitOwner_l6bc3f0* owner;  // 0x30
    u8 pad_0x38[0x14748 - 0x38];
    UnkVec3f saved_position;  // 0x14748
    u8 pad_0x14754[0xc];
    UnkVec3f saved_angle;  // 0x14760
    u8 pad_0x1476c[0x14780 - 0x1476c];
    s32 mode;      // 0x14780
    s32 sub_mode;  // 0x14784

    // The track the attack plays (vtable slot 0x6b0).
    CStrId AttackTrack() {
        return reinterpret_cast<CStrId (*)(UnkShelmitAI_l6bc3f0*)>(vtable[0x6b0 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkShelmitAI_l6bc3f0, sub_mode) == 0x14784, "layout");

static const u64 kAbortTrack = 0xf4061ff25a2163d6;

static inline f32 AtLeastZero(f32 value) {
    return value > 0.0f ? value : 0.0f;
}

static inline bool Near(f32 value, f32 target) {
    return target - 0.001f <= value && value <= target + 0.001f;
}

u32 fn_71006bc3f0(UnkShelmitAI_l6bc3f0* self, UnkShelmitMsg_l6bc3f0* msg) {
    UnkActor* other = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    UnkAnimationComponent* animation = self->owner->animation;
    UnkShelmitAttack_l6bc3f0* attack = self->owner->attack;
    if (msg->field_0x20 && self->mode == 1 && attack && self->sub_mode != 1 && !fn_7101136308(attack)) {
        fn_7100232c68(self->owner);
        return 0;
    }
    if (!other || !animation || !attack)
        return 0;
    if (self->mode != 1)
        return fn_7101111dc8(reinterpret_cast<UnkNailongAI_b46a2*>(self), reinterpret_cast<UnkNailongMsg_b46a2*>(msg));
    if (!msg->field_0x20 || !fn_7101136308(attack))
        return 0;

    if (self->sub_mode == 1) {
        bool aborted = fn_710117b348(animation, fn_7100bef064());
        if (!aborted) {
            f32 frames = fn_7101177ea0(animation, kAbortTrack);
            aborted = fn_710117aaac(animation, fn_7100bef064(), AtLeastZero(frames - 1.0f));
        }
        if (aborted) {
            attack->Stop();
            {
                CStrId track = self->AttackTrack();
                u64 hash = track.data->Key();
                base::reflection::UnkReflTypeIdArg mode;
                mode.id = 2;
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7101176690(animation, hash, kAbortTrack, &mode, &g_unk_710164b958, &g_unk_7172769740,
                              &mask);
            }
            UnkShelmitOwner_l6bc3f0* owner = self->owner;
            UnkVec3f position = owner->xform.GetTranslation();
            if (Near(position.x, self->saved_position.x) && Near(position.y, self->saved_position.y) &&
                Near(position.z, self->saved_position.z)) {
                UnkVec3f angle = owner->xform.GetAngle();
                if (Near(angle.x, self->saved_angle.x) && Near(angle.y, self->saved_angle.y) &&
                    Near(angle.z, self->saved_angle.z))
                    return 0;
            }
            UnkShelmitOwner_l6bc3f0* moved = self->owner;
            moved->xform.SetTranslation(self->saved_position);
            moved->moved = true;
            UnkShelmitOwner_l6bc3f0* turned = self->owner;
            turned->xform.SetAngle(self->saved_angle);
            u32 what = 2;
            turned->OnTransformChanged(&turned->xform, &what);
            turned->turned = true;
            return 0;
        }
    }

    UnkShelmitOwner_l6bc3f0* owner = self->owner;
    f32 dx = owner->xform.m[0][3] - other->field_0x44;
    f32 dy = owner->xform.m[1][3] - other->field_0x54;
    f32 dz = owner->xform.m[2][3] - other->field_0x64;
    f32 limit = reinterpret_cast<const UnkShelmitTunable_l6bc3f0*>(&g_unk_71727bc558)->distance;
    if (dx * dx + dy * dy + dz * dz > limit * limit)
        fn_7100232c68(owner);
    return 0;
}
