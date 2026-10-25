// Whether the first object is behind the second: true when the first has no position object,
// otherwise whether the vector from the second to the first's position has a dot product with the
// second's direction (the third column of its transform's matrix) that is negative or unordered.
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "types.h"

bool fn_71002c2734(void* self, UnkActor* first, UnkActor* second) {
    UnkActor* target = fn_7100d7c990(first);
    if (target) {
        f32 tx = target->field_0x44;
        f32 ty = target->field_0x54;
        f32 tz = target->field_0x64;
        f32 sx = second->field_0x44;
        f32 sy = second->field_0x54;
        f32 sz = second->field_0x64;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(second->field_0x38));
        f32 m8 = m.m[2];
        f32 m18 = m.m[6];
        f32 m28 = m.m[10];
        f32 dx = tx - sx;
        f32 dy = ty - sy;
        f32 dz = tz - sz;
        f32 dot = dx * m8 + dy * m18 + dz * m28;
        if (dot >= 0.0f)
            return false;
    }
    return true;
}
