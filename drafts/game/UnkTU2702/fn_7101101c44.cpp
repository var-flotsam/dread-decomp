// Unit 2702 (hint CAIComponent): steps the AI's state machine unless the actor is frozen: runs the step
// (fn_7101101dec) with the parameter block for the current mode, records the step's results, and reports
// the old and new state (virtual 0x450) when it changed. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CFrozenComponent.h"
#include "types.h"

using base::math::CVector3D;

struct UnkAIStruct_g;
void fn_7101102da8(UnkAIStruct_g* self);

struct UnkAIStateParams_l101c44 {
    u8 data[0x60];
};

struct UnkAISettings_l101c44 {
    u8 pad_0x0[0x248];
    UnkAIStateParams_l101c44 idle;     // 0x248
    UnkAIStateParams_l101c44 alert;    // 0x2a8
    UnkAIStateParams_l101c44 resting;  // 0x308
};

struct UnkAIActor_l101c44 {
    u8 pad_0x0[0x2c8];
    void* frozen;  // 0x2c8
};

struct UnkAI_l101c44;
void fn_7101101dec(UnkAI_l101c44* self, const UnkAIStateParams_l101c44* params, bool flag, s32* state, f32 dt);

struct UnkAI_l101c44 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkAIActor_l101c44* actor;  // 0x30
    u8 pad_0x38[0x11ac];
    s32 state;  // 0x11e4
    u8 pad_0x11e8[6];
    bool field_0x11ee;
    u8 pad_0x11ef;
    u32 field_0x11f0;
    u8 pad_0x11f4[0x10];
    CVector3D field_0x1204;
    CVector3D field_0x1210;
    u8 pad_0x121c;
    bool field_0x121d;
    bool idle;     // 0x121e
    bool alerted;  // 0x121f
    bool field_0x1220;
    u8 pad_0x1221[0x2db];
    u32 field_0x14fc;
    CVector3D field_0x1500;
    CVector3D field_0x150c;
    u8 pad_0x1518[0xf8];
    UnkAISettings_l101c44* settings;  // 0x1610
    u8 pad_0x1618[0x13054];
    s32 field_0x1466c;

    void vf_0x450(s32 from, s32 to) {
        reinterpret_cast<void (*)(UnkAI_l101c44*, s32, s32)>(vtable[0x450 / 8])(this, from, to);
    }
    void vf_0x460() { reinterpret_cast<void (*)(UnkAI_l101c44*)>(vtable[0x460 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkAI_l101c44, field_0x1220) == 0x1220, "layout");
static_assert(__builtin_offsetof(UnkAI_l101c44, settings) == 0x1610, "layout");
static_assert(__builtin_offsetof(UnkAI_l101c44, field_0x1466c) == 0x1466c, "layout");

void fn_7101101c44(UnkAI_l101c44* self, f32 dt, bool flag) {
    s32 old = self->state;
    if (self->actor->frozen && fn_71007f9f20(self->actor->frozen))
        return;
    const UnkAIStateParams_l101c44* params;
    if (!self->alerted && !self->field_0x121d)
        params = &self->settings->idle;
    else if (!self->idle)
        params = &self->settings->alert;
    else
        params = &self->settings->resting;
    fn_7101101dec(self, params, flag, &self->state, dt);
    s32 now = self->state;
    self->idle = now == 0 || now == 3;
    self->field_0x1220 = self->field_0x11ee;
    self->field_0x14fc = self->field_0x11f0;
    if ((now == 0 || now == 3) && !self->alerted)
        self->alerted = true;
    self->field_0x1500 = self->field_0x1204;
    self->field_0x150c = self->field_0x1210;
    if ((old == 1 || old == 2) && now == 0) {
        fn_7101102da8(reinterpret_cast<UnkAIStruct_g*>(self));
    } else if ((old == 3 || old == 0) && now == 1) {
        self->vf_0x460();
        self->field_0x1466c = 0;
    }
    if (old != self->state)
        self->vf_0x450(old, self->state);
}
