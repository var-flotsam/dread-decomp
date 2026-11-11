// CSpecialEnergyComponent unit (tools/tu_map.py): fn_7100a1f768, the update after the component base's
// (fn_71010c4234). While recovery is on (+0x49), the delay (+0x4c) first runs up to the tunable's
// fTimeToStartEnergyRecover (+0x24); then the energy (fn_7100a1fa98) moves from +0x44 by +0x40 over
// the recovery time, the faster one (+0x50) while fn_7100a1f84c's test holds (inlined here), else
// +0x54. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CComponent.h"
#include "game/CSpecialEnergyComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1501.h"
#include "types.h"

extern base::reflection::UnkTunable g_unk_7172745770;  // CTunableSpecialEnergyComponent

struct UnkLifeComponent;

struct UnkEnergyHolder_b48 {
    u8 pad_0x0[0x118];
    f32 energy;
};

struct UnkSpecialEnergyActor_b48 {
    u8 pad_0x0[0x1f0];
    UnkEnergyHolder_b48* holder;
    u8 pad_0x1f8[0x10];
    void* field_0x208;
    u8 pad_0x210[0x10];
    UnkAnimationComponent* animation;
};

struct UnkSpecialEnergyComponent_b48 {
    u8 pad_0x0[0x30];
    UnkSpecialEnergyActor_b48* actor;
    u8 pad_0x38[0x8];
    f32 field_0x40;
    f32 field_0x44;
    u8 pad_0x48;
    bool recovering;
    u8 pad_0x4a[2];
    f32 delay;
    f32 fast_time;
    f32 slow_time;

    // fn_7100a1f84c.
    bool Fast() {
        if (actor->field_0x208 && fn_71009e1390(actor->field_0x208)) {
            if (UnkAnimationComponent* animation = actor->animation)
                return fn_710117b348(animation, fn_7100c3d378());
        }
        if (UnkEnergyHolder_b48* holder = actor->holder)
            return holder->energy > 1.1920929e-07f;
        return false;
    }
};

void fn_7100a1f768(UnkSpecialEnergyComponent_b48* self, f32 dt) {
    fn_71010c4234(reinterpret_cast<char*>(self));
    if (!self->recovering)
        return;
    if (self->delay < *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172745770) + 0x24)) {
        self->delay += dt;
        return;
    }
    f32 time = self->Fast() ? self->fast_time : self->slow_time;
    f32 rate = self->field_0x40 / time;
    fn_7100a1fa98(reinterpret_cast<UnkLifeComponent*>(self), false, true, self->field_0x44 + rate * dt);
}
