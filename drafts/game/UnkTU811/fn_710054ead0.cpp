// UnkTU811 (CEmmyAIComponent): whether the E.M.M.I. leaves a point alone. It does when the point is out of
// reach (too high or too far), not ahead of the owner, behind the target, or while other states hold;
// otherwise it sends the owner's animation one of three requests and answers with the send's result.
// Names and layouts are placeholders.
#include <math.h>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "game/CAnimationComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU39.h"
#include "types.h"

bool fn_71005137dc(UnkActor* owner);

struct UnkVec3_l54ead0 {
    f32 x;
    f32 y;
    f32 z;
};

static inline UnkVec3_l54ead0 operator-(const UnkVec3_l54ead0& a, const UnkVec3_l54ead0& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline UnkVec3_l54ead0 operator/(const UnkVec3_l54ead0& v, f32 d) {
    return {v.x / d, v.y / d, v.z / d};
}

static inline f32 Dot(const UnkVec3_l54ead0& a, const UnkVec3_l54ead0& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline f32 Length(const UnkVec3_l54ead0& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

static inline UnkVec3_l54ead0 Pos(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

struct UnkEmmyAI_l54ead0 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x29418 - 0x38];
    u8 field_0x29418[0x10];  // a timer (fn_710002fb70)
    u8 pad_0x29428[0x29f26 - 0x29428];
    bool field_0x29f26;
    u8 pad_0x29f27[0x2a25c - 0x29f27];
    UnkVec3_l54ead0 point;  // 0x2a25c
};
static_assert(__builtin_offsetof(UnkEmmyAI_l54ead0, point) == 0x2a25c, "layout");

static inline bool Send(void* animation, u64 hash, base::reflection::UnkReflTypeIdArg* mode,
                        base::reflection::UnkReflTypeIdArg* mask) {
    mode->id = 0;
    mask->id = UnkAll_SpitSend().value;
    return fn_7101176690(animation, hash, 0xf4061ff25a2163d6ull, mode, &g_unk_710164b958, &g_unk_7172769740, mask);
}

bool fn_710054ead0(UnkEmmyAI_l54ead0* self) {
    void* movement = self->owner->field_0x218;
    if (!movement || fn_71010cab0c(movement))
        return false;

    UnkActor* owner = self->owner;
    if (!(fabsf(self->point.y - owner->field_0x54) < 300.0f))
        return true;
    UnkVec3_l54ead0 delta = self->point - Pos(owner);
    f32 dist = Length(delta);
    if (!(dist < 1000.0f))
        return true;

    UnkVec3_l54ead0 dir = delta / dist;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    UnkVec3_l54ead0 forward = {m.m[2], m.m[6], m.m[10]};
    if (!(Dot(dir, forward) > 0.3f))
        return true;

    if (UnkActor* target = fn_7100d7c990(self->owner)) {
        UnkVec3_l54ead0 to_target = Pos(target) - Pos(self->owner);
        f32 len = Length(to_target);
        if (len < dist && Dot(forward, to_target / len) > 0.1f)
            return true;
    }

    if (fn_710002fb70(self->field_0x29418) || self->field_0x29f26 || fn_71005137dc(self->owner))
        return true;
    if (void* animation = self->owner->field_0x220) {
        if (fn_7101178e6c(static_cast<UnkTakumakuComponent*>(animation), 10))
            return true;
    }

    if (self->owner->field_0x218 && fn_7100979ef0(self->owner->field_0x218))
        goto moving;
    if (fn_71010cc38c(self->owner->field_0x218) == 2)
        goto kind2;
    goto other;
    // The gotos skip these declarations, so they get no lifetime markers and keep slots of their own
    // (as fn_71005474f4 in this unit).
    base::reflection::UnkReflTypeIdArg mode_moving;
    base::reflection::UnkReflTypeIdArg mask_moving;
    base::reflection::UnkReflTypeIdArg mode_kind2;
    base::reflection::UnkReflTypeIdArg mask_kind2;
    base::reflection::UnkReflTypeIdArg mode_other;
    base::reflection::UnkReflTypeIdArg mask_other;
moving:
    return Send(self->owner->field_0x220, 0x76244453272b959aull, &mode_moving, &mask_moving);
kind2:
    return Send(self->owner->field_0x220, 0x785e4fb1e8de46cbull, &mode_kind2, &mask_kind2);
other:
    return Send(self->owner->field_0x220, 0x30d9709f9a2a7864ull, &mode_other, &mask_other);
}
