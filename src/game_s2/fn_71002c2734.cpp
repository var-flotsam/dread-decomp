// Whether the first object is behind the second: true when the first has no position object,
// otherwise whether the vector from the second to the first's position has a dot product with the
// second's direction (the part at 0x38; elements at 0x8, 0x18, 0x28) that is negative or unordered.
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

UnkMatrixRow* fn_710002614c(void* part);

bool fn_71002c2734(void* self, UnkActor* first, UnkActor* second) {
    UnkActor* target = fn_7100d7c990(first);
    if (target) {
        f32 tx = target->field_0x44;
        f32 ty = target->field_0x54;
        f32 tz = target->field_0x64;
        f32 sx = second->field_0x44;
        f32 sy = second->field_0x54;
        f32 sz = second->field_0x64;
        UnkMatrixRow* m = fn_710002614c(second->field_0x38);
        f32 m8 = m->field_0x8;
        f32 m18 = m->field_0x18;
        f32 m28 = m->field_0x28;
        f32 dx = tx - sx;
        f32 dy = ty - sy;
        f32 dz = tz - sz;
        f32 dot = dx * m8 + dy * m18 + dz * m28;
        if (dot >= 0.0f)
            return false;
    }
    return true;
}
