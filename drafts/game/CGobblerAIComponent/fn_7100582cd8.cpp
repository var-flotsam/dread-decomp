// CGobblerAIComponent (unit 832): an attack request's set-up after the base step. An attack track against
// a target not in a melee track marks the request; during the custom-impulse track the request gets an
// impulse blended by the vertical distance between the two actors. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkGobblerActor_l582cd8 {
    u8 pad_0x0[0x44];
    f32 y;  // 0x44
    u8 pad_0x48[0x220 - 0x48];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkGobblerImpulse_l582cd8 {
    f32 x;
    f32 y;
};

struct UnkGobblerParams_l582cd8 {
    u8 pad_0x0[0x600];
    UnkGobblerImpulse_l582cd8 impulse_near;  // 0x600
    UnkGobblerImpulse_l582cd8 impulse_far;  // 0x608
    f32 distance_min;  // 0x610
    f32 distance_max;  // 0x614
};

struct UnkGobblerAI_l582cd8 {
    u8 pad_0x0[0x30];
    UnkGobblerActor_l582cd8* owner;  // 0x30
    u8 pad_0x38[0x14700 - 0x38];
    UnkGobblerParams_l582cd8* params;  // 0x14700
};

struct UnkGobblerRequest_l582cd8 {
    u8 pad_0x0[0x18];
    bool field_0x18;
    u8 pad_0x19[0x54 - 0x19];
    UnkGobblerImpulse_l582cd8 impulse;  // 0x54
    u8 pad_0x5c[0xc8 - 0x5c];
    u32 field_0xc8;  // 0xc8
};

static inline UnkGobblerImpulse_l582cd8 Lerp(const UnkGobblerImpulse_l582cd8& a, const UnkGobblerImpulse_l582cd8& b,
                                             f32 t) {
    if (t >= 1.0f)
        return a;
    if (t < 0.0f)
        return b;
    UnkGobblerImpulse_l582cd8 r;
    r.x = a.x * t + b.x * (1.0f - t);
    r.y = a.y * t + b.y * (1.0f - t);
    return r;
}

void fn_7100582cd8(UnkGobblerAI_l582cd8* self, UnkGobblerActor_l582cd8* target, UnkGobblerRequest_l582cd8* request) {
    fn_7101105c90(reinterpret_cast<UnkKraidAI2*>(self), target, reinterpret_cast<UnkKraidRequest*>(request));
    UnkAnimationComponent* animation = self->owner ? self->owner->animation : nullptr;
    UnkAnimationComponent* target_animation = target ? target->animation : nullptr;
    if (!animation)
        return;
    if (fn_710117b348(animation, fn_7100ceda9c()) && !fn_710117b348(target_animation, fn_7100d1ab58()))
        request->field_0x18 = true;
    if (!self->params)
        return;
    if (!fn_710117b348(animation, fn_7100c1fe28()))
        return;
    request->field_0xc8 = 1;
    f32 distance = __builtin_fabsf(target->y - self->owner->y);
    const UnkGobblerParams_l582cd8* params = self->params;
    f32 t = (distance - params->distance_min) / (params->distance_max - params->distance_min);
    if (t <= 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;
    request->impulse = Lerp(params->impulse_near, params->impulse_far, t);
}
