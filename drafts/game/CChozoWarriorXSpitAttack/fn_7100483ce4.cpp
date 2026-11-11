// CChozoWarriorXSpitAttack's update: after the base update (fn_71003609a8), turns the angle at +0x50
// towards the aim: the pitch of the vector between the owner and the point fn_71011735e8 finds (or, once
// the attack has not started, between two stored points at +0x74 and +0x80), at a rate that eases from 3 to
// 10 over the first 0.7 seconds (+0x4c), then hands the angle in degrees to the owner's bag under "Angle"
// and runs fn_7100483f20. Names and layouts are placeholders.
#include "game_s2/UnkSpitAttack.h"
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "types.h"

using base::global::CStrId;

struct UnkEntity;
bool fn_71011735e8(UnkEntity* entity, const void* key, UnkMatrix44* out, u32 flags);
void fn_7100483f20(UnkSpitAttack* self, f32 dt);
extern u64 g_unk_7101649880;
extern "C" f32 atan2f(f32 y, f32 x);

struct UnkSpitUpdate_b0266 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x38];
    bool started;  // 0x48
    u8 pad_0x49[0x3];
    f32 timer;  // 0x4c
    f32 angle;  // 0x50
    u8 pad_0x54[0x20];
    f32 from_x;  // 0x74
    f32 from_y;  // 0x78
    u8 pad_0x7c[0x4];
    f32 to_x;  // 0x80
    f32 to_y;  // 0x84
};
static_assert(__builtin_offsetof(UnkSpitUpdate_b0266, to_x) == 0x80, "layout");

static inline f32 Saturate(f32 t) {
    return !(t <= 0.0f) ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
}

static inline f32 Lerp(f32 a, f32 b, f32 k) {
    if (k >= 1.0f)
        return b;
    if (k < 0.0f)
        return a;
    f32 rest = (1.0f - k) * a;
    return k * b + rest;
}

void fn_7100483ce4(UnkSpitUpdate_b0266* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    f32 pitch;
    if (self->started) {
        self->timer = self->timer + dt;
        UnkActor* owner = self->owner;
        f32 x = owner->field_0x44;
        f32 y = owner->field_0x54;
        UnkMatrix44 m = {};
        m.m[15] = 1.0f;
        m.m[10] = 1.0f;
        m.m[5] = 1.0f;
        m.m[0] = 1.0f;
        bool found = fn_71011735e8(reinterpret_cast<UnkEntity*>(owner->field_0x228), &g_unk_7101649880, &m, 0);
        x = found ? m.m[3] : x;
        y = found ? m.m[7] : y;
        f32 dy = y - fn_7100d7c990(self->owner)->field_0x54;
        f32 dx = __builtin_fabsf(x - fn_7100d7c990(self->owner)->field_0x44);
        pitch = atan2f(dy, dx);
    } else {
        f32 dy = self->from_y - self->to_y;
        f32 dx = __builtin_fabsf(self->from_x - self->to_x);
        pitch = atan2f(dy, dx);
    }
    f32 target = 1.57079637f - pitch;
    f32 k = Saturate(self->timer / 0.699999988f);
    f32 step = Lerp(3.0f, 10.0f, k) * dt;
    f32 current = self->angle;
    f32 lo = current - step;
    f32 hi = current + step;
    f32 a = target > lo ? target : lo;
    f32 b = target < hi ? target : hi;
    self->angle = target > current ? b : a;
    if (void* bag = self->owner->field_0x220) {
        auto* props = reinterpret_cast<unk_prop_bag::UnkPropBag*>(static_cast<char*>(bag) + 0x80);
        CStrId id("Angle", true);
        unk_prop_bag::fn_710033187c(props, id, (f32)((f64)(self->angle * 180.0f) / 3.14159265358979323846));
    }
    fn_7100483f20(reinterpret_cast<UnkSpitAttack*>(self), dt);
}
