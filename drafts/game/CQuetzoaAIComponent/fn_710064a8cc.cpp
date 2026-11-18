// CQuetzoaAIComponent's unit (tools/tu_map.py, unit 973): after the base update, moves the front-back and
// up-down factors (+0x14ad0) toward the owner's velocity, scaled and in the owner's facing, and reports
// them to the owner's value bag as "FrontBackFactor" and "UpDownFactor". Names and layouts are placeholders.
#include <math.h>
#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkBossAI_l3ae15c;
void fn_71003ae15c(UnkBossAI_l3ae15c* self, f32 dt);

struct UnkQuetzoaBagOwner_l64a8cc {
    u8 pad_0x0[0x80];
    unk_prop_bag::UnkPropBag bag;  // 0x80
};

struct UnkQuetzoaMoveState_l64a8cc {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkQuetzoaMoveEntry_l64a8cc {
    u8 pad_0x0[0x10];
    UnkQuetzoaMoveState_l64a8cc* state;  // 0x10
};

struct UnkQuetzoaMoveSlot_l64a8cc {
    UnkQuetzoaMoveEntry_l64a8cc* entry;  // 0x0
    u8 pad_0x8[0x8];
    void* field_0x10;
};

struct UnkQuetzoaMover_l64a8cc {
    u8 pad_0x0[0xb0];
    UnkQuetzoaMoveSlot_l64a8cc* slot;  // 0xb0
};

struct UnkQuetzoaMovement_l64a8cc {
    u8 pad_0x0[0x40];
    f32 velocity_x;  // 0x40
    f32 velocity_y;  // 0x44
    f32 velocity_z;  // 0x48
    u8 pad_0x4c[0x7c - 0x4c];
    f32 scale;  // 0x7c
};

struct UnkQuetzoaOwner_l64a8cc {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1f0 - 0x78];
    UnkQuetzoaMovement_l64a8cc* movement;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkQuetzoaBagOwner_l64a8cc* bag_owner;  // 0x220
    u8 pad_0x228[0x298 - 0x228];
    UnkQuetzoaMover_l64a8cc* mover;  // 0x298
};

// The front-back factor and how fast it moves.
struct UnkQuetzoaFrontBack_l64a8cc {
    f32 value;
    f32 speed;
};

struct UnkQuetzoaAI_l64a8cc {
    u8 pad_0x0[0x30];
    UnkQuetzoaOwner_l64a8cc* owner;  // 0x30
    u8 pad_0x38[0x14ad0 - 0x38];
    UnkQuetzoaFrontBack_l64a8cc front_back;  // 0x14ad0
    f32 up_down;                              // 0x14ad8
};

inline s32 Sign(f32 v) {
    return (v > 0.0f) - (v < 0.0f);
}

// `from` moved toward `to` by at most `step`.
inline f32 Approach(f32 from, f32 to, f32 step) {
    if (to > from) {
        f32 high = from + step;
        return to < high ? to : high;
    }
    f32 low = from - step;
    return to > low ? to : low;
}

void fn_710064a8cc(UnkQuetzoaAI_l64a8cc* self, f32 dt) {
    fn_71003ae15c(reinterpret_cast<UnkBossAI_l3ae15c*>(self), dt);
    UnkQuetzoaOwner_l64a8cc* owner = self->owner;
    UnkQuetzoaBagOwner_l64a8cc* bag_owner = owner->bag_owner;
    UnkQuetzoaMover_l64a8cc* mover = owner->mover;
    UnkQuetzoaMovement_l64a8cc* movement = owner->movement;
    if (!bag_owner || !mover || !movement)
        return;

    f32 x = movement->velocity_x;
    f32 y = movement->velocity_y;
    f32 z = movement->velocity_z;
    f32 inv = 1.0f / (sqrtf(x * x + y * y + z * z) + 0.0001f);
    f32 dir_x = x * inv;
    f32 dir_y = y * inv;
    f32 facing = fn_710002614c(&self->owner->transform).m[2];
    f32 front_back = movement->scale * (dir_x * Sign(facing));
    f32 up_down = dir_y * movement->scale;
    UnkQuetzoaMoveSlot_l64a8cc* slot = mover->slot;
    if (slot->entry && slot->entry->state && slot->entry->state->kind == 2 && slot->field_0x10)
        front_back = front_back > 0.15f ? front_back : 0.15f;

    f32 up_down_step = dt * 4.0f;
    f32 settle = dt * 0.5f;
    UnkQuetzoaFrontBack_l64a8cc& fb = self->front_back;
    f32 speed = fabsf(fb.speed);
    speed = speed <= 1.0f ? 1.0f : (!(speed >= 4.0f) ? speed : 4.0f);
    f32 step = speed * dt;
    f32 next = Approach(fb.value, front_back, step);
    f32 last_up_down = self->up_down;
    f32 moved = (next - fb.value) / step;
    fb.value = next;
    fb.speed = speed;
    f32 next_up_down = Approach(last_up_down, up_down, up_down_step);
    fb.speed = moved + settle * Sign(moved);
    self->up_down = next_up_down;

    unk_prop_bag::fn_710033187c(&bag_owner->bag, CStrId("FrontBackFactor", true), fb.value);
    unk_prop_bag::fn_710033187c(&bag_owner->bag, CStrId("UpDownFactor", true), self->up_down);
}
