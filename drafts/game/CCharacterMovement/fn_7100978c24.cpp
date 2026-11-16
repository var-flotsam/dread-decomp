// CCharacterMovement (unit 1445): advances a movement's transform: the start transform times the
// relative matrix, rescaled to `scale` (or normalized when it is 1), moved by the offset plus
// velocity * t + acceleration * t^2 / 2. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

// The flags a new matrix clears (set elsewhere).
__attribute__((visibility("hidden"))) extern u32 g_unk_717273a7d0;

struct UnkMoveState_l978c24 {
    f32 field_0x0;  // the elapsed time
    u8 pad_0x4[0x68];
    UnkVec3f field_0x6c;  // the offset added to the position
    UnkMatrix44 field_0x78;  // the relative matrix
    UnkXform field_0xb8;  // the start transform
};
static_assert(__builtin_offsetof(UnkMoveState_l978c24, field_0xb8) == 0xb8, "UnkMoveState_l978c24 layout");

// Whether two values are equal within the tolerance.
static inline bool Near(f32 a, f32 b) {
    return a - 1e-5f <= b && a + 1e-5f >= b;
}

void fn_7100978c24(UnkMoveState_l978c24* self, const UnkVec3f* velocity, const UnkVec3f* accel, f32 scale) {
    UnkMatrix44 m = fn_71000127c8(self->field_0xb8.GetMatrix(), self->field_0x78);
    if (scale == 1.0f) {
        fn_71000774fc(reinterpret_cast<UnkXform*>(&m));
    } else if (scale > 1e-7f) {
        UnkVec3f s = fn_7100076cec(reinterpret_cast<UnkXform*>(&m));
        if ((!Near(s.x, scale) || !Near(s.y, scale) || !Near(s.z, scale)) && s.x > 1e-5f && s.y > 1e-5f &&
            s.z > 1e-5f) {
            m = fn_71000127c8(m, fn_7100079138(scale / s.x, scale / s.y, scale / s.z));
        }
    }

    UnkVec3f start = self->field_0xb8.GetTranslation();
    f32 t = self->field_0x0;
    m.m[3] = start.x + self->field_0x6c.x + t * velocity->x + t * t * (accel->x * 0.5f);
    m.m[7] = start.y + self->field_0x6c.y + t * velocity->y + t * t * (accel->y * 0.5f);
    m.m[11] = start.z + self->field_0x6c.z + t * velocity->z + t * t * (accel->z * 0.5f);

    UnkCopyMatrix(reinterpret_cast<UnkMatrix44*>(self->field_0xb8.m), m);
    self->field_0xb8.flags = (self->field_0xb8.flags | 8) & ~g_unk_717273a7d0;
}
