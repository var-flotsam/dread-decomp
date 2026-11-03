// behaviortree::CAgitateInNervousnessAction (tools/tu_map.py unit 995, the name from reflection): the
// per-frame step. While active (+0x2c) the timer at +0x34 runs up to the limit at +0x30 (then the action
// ends: 5), restarting when it passes the CSabotoruAIComponent's own period (+0x1128); then, unless the
// owner's character class plays the animation named at +0x38, fn_71006749bc starts it. Returns 2
// (running). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkSabotoruAI_ain {
    void** vtable;
    u8 pad_0x8[0x1120];
    f32 period;  // 0x1128
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkSabotoruAI_ain*)>(vtable[0])(this);
    }
};

struct UnkAgitateAnim_ain {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkAgitateAction_ain {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0xc];
    bool active;  // 0x2c
    f32 limit;  // 0x30
    f32 time;  // 0x34
    UnkAgitateAnim_ain* anim;  // 0x38
};

void fn_71006749bc(UnkAgitateAction_ain* self);

u32 fn_7100674b28(UnkAgitateAction_ain* self, f32 dt) {
    if (self->active) {
        self->time += dt;
        if (self->time >= self->limit)
            return 5;
        UnkSabotoruAI_ain* ai = static_cast<UnkSabotoruAI_ain*>(self->owner->field_0x240);
        UnkSabotoruAI_ain* sabotoru = fn_710009d50c(ai->vf_0x0(), fn_7100677dd4()) ? ai : nullptr;
        if (sabotoru->period < self->time)
            self->time = 0.0f;
    }
    if (UnkAnimationComponent* cls = static_cast<UnkAnimationComponent*>(self->owner->field_0x220)) {
        if (!fn_710117ae38(cls, self->anim->id, false))
            fn_71006749bc(self);
    }
    return 2;
}
