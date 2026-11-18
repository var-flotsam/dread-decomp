// Unit 816 (tools/tu_map.py): an Emmy behaviour's update. While enabled (+0x28) and after the shared
// step (fn_7100573218), it records the owner's position and either holds the value at its maximum
// (mode 0) or runs a four-phase cycle (mode 1) on a timer, easing the value down in phase 1.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkTU817.h"
#include "types.h"

// As src/game/CEmmyAIComponent/CEmmyAIComponent_CanAct_b63a3.cpp defines it.
struct UnkEmmyAI_b63a3;
bool fn_7100551d98(UnkEmmyAI_b63a3* self);
// As src/game/UnkTU818/UnkTU818_PathLoad_ovb0258.cpp declares it.
struct UnkAim_b0258y;
// An easing curve over [0, 1].
f32 fn_7100076880(f32 t);

struct UnkEmmyOwner_l572734 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
};

struct UnkEmmyPulse_l572734 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    u8 pad_0x8[0x18];
    UnkEmmyOwner_l572734* owner;  // 0x20
    bool enabled;                 // 0x28
    u8 pad_0x29[3];
    u32 mode;          // 0x2c
    f32 max_value;     // 0x30
    u8 pad_0x34[4];
    f32 fade_time;     // 0x38
    f32 low_time;      // 0x3c
    f32 rise_time;     // 0x40
    f32 high_time;     // 0x44
    bool has_owner;    // 0x48
    u8 pad_0x49[3];
    base::math::CVector3D position;  // 0x4c
    f32 value;         // 0x58
    u32 phase;         // 0x5c
    f32 timer;         // 0x60
};
static_assert(__builtin_offsetof(UnkEmmyPulse_l572734, timer) == 0x60, "layout");

void fn_7100572734(UnkEmmyPulse_l572734* self, f32 dt) {
    if (!self->enabled)
        return;
    fn_7100573218(reinterpret_cast<UnkAim_b0258y*>(self));
    if (!self->has_owner)
        return;
    UnkActor* actor = self->owner->actor;
    self->position = base::math::CVector3D{actor->field_0x44, actor->field_0x54, actor->field_0x64};
    bool active = fn_7100551d98(reinterpret_cast<UnkEmmyAI_b63a3*>(self->owner));
    if (self->mode == 0) {
        self->value = self->max_value;
        self->vf_0x30();
        return;
    }
    if (self->mode != 1)
        return;
    switch (self->phase) {
    case 0:
        if (active && self->timer <= 0.0f) {
            self->phase = 1;
            self->timer = self->fade_time;
        }
        break;
    case 1:
        if (!active) {
            self->phase = 2;
        } else if (self->timer <= 0.0f) {
            self->phase = 2;
            self->value = self->max_value;
            self->timer = self->low_time;
        } else {
            f32 value = fn_7100076880(1.0f - self->timer / self->fade_time) * self->max_value;
            self->value = !(value < 0.0f) ? value : 0.0f;
        }
        break;
    case 2:
        if (self->timer <= 0.0f) {
            self->phase = 3;
            self->timer = self->rise_time;
        }
        break;
    case 3:
        if (self->timer <= 0.0f) {
            self->timer = self->high_time;
            self->value = 0.0f;
            self->phase = 0;
        }
        break;
    }
    self->vf_0x30();
    f32 timer;
    if (active) {
        timer = self->timer - dt;
    } else if (self->phase == 3) {
        timer = self->timer - (dt + dt);
    } else {
        self->timer = 0.0f;
        return;
    }
    self->timer = !(timer < 0.0f) ? timer : 0.0f;
}
