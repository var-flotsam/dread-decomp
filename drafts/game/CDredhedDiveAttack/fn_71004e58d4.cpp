// CDredhedDiveAttack's unit, fn_71004e58d4: whether the dive can start: the AI's first distance is under
// 300, or the target is within 500 on x, ahead of where the owner faces, and the second distance is over
// 300. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkDredhedDiveAttack_l4e58d4 {
    void* vtable;
    UnkActor* owner;  // 0x8
};

static inline f32 Sign(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

bool fn_71004e58d4(UnkDredhedDiveAttack_l4e58d4* self) {
    void* ai = self->owner->field_0x240;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    f32 facing = m.m[2];
    UnkActor* target = fn_7100d7c990(self->owner);
    f32 dx = target->field_0x44 - self->owner->field_0x44;
    f32 far;
    f32 near;
    fn_71010fc778(ai, &far);
    fn_71010fc68c(static_cast<UnkGoliathAI_b58a3*>(ai), &near);
    f32 side = Sign(dx);
    f32 ahead = Sign(facing);
    return near < 300.0f ||
           (__builtin_fabsf(dx) < 500.0f && side + 0.1f >= ahead && !(side - 0.1f > ahead) && far > 300.0f);
}
