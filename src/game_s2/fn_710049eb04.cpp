// Whether the object's target is far enough to leave alone: false when the target lies behind
// the object (the vector to it has a negative dot product with the object's direction, the part at
// 0x38, elements at 0x8, 0x18, 0x28); otherwise whether the squared distance between the two is not below the
// square of both radii (at 0x37c) plus 70.
#include "game/UnkActor.h"
#include "types.h"

struct UnkMatrixRow {
    u8 pad_0x0[8];
    f32 field_0x8;
    u8 pad_0xc[0xc];
    f32 field_0x18;
    u8 pad_0x1c[0xc];
    f32 field_0x28;
};

struct UnkFollower {
    u8 pad_0x0[8];
    UnkActor* field_0x8;
};

UnkMatrixRow* fn_710002614c(void* part);

bool fn_710049eb04(UnkFollower* self) {
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    UnkActor* me = self->field_0x8;
    f32 tx = target->field_0x44;
    f32 ty = target->field_0x54;
    f32 tz = target->field_0x64;
    f32 mx = me->field_0x44;
    f32 my = me->field_0x54;
    f32 mz = me->field_0x64;
    f32 dx0 = tx - mx;
    f32 dy0 = ty - my;
    f32 dz0 = tz - mz;
    UnkMatrixRow* m = fn_710002614c(me->field_0x38);
    f32 m8 = m->field_0x8;
    f32 m18 = m->field_0x18;
    f32 m28 = m->field_0x28;
    f32 dot = dx0 * m8 + dy0 * m18 + dz0 * m28;
    if (dot < 0.0f)
        return false;
    UnkActor* a = self->field_0x8;
    f32 ax = a->field_0x44;
    f32 ay = a->field_0x54;
    f32 az = a->field_0x64;
    UnkActor* p = fn_7100d7c990(a);
    f32 dx = ax - p->field_0x44;
    f32 dy = ay - p->field_0x54;
    f32 pz = p->field_0x64;
    f32 d2 = dx * dx + dy * dy + (az - pz) * (az - pz);
    f32 r1 = self->field_0x8->field_0x37c;
    f32 r2 = fn_7100d7c990(self->field_0x8)->field_0x37c;
    f32 limit = r1 + r2 + 70.0f;
    return !(d2 < limit * limit);
}
