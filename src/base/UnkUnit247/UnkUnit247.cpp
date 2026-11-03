// Unit 247 of tools/tu_map.py (0x71001bbb3c-0x71001bc2dc): an object's rotation as a quaternion:
// its own (+0xb8) in mode 1, the identity otherwise; always answers true. Names and layouts are
// placeholders.
#include "base/math/CVector4D.h"
#include "types.h"

struct UnkRotated247 {
    u8 pad_0x0[0xb8];
    base::math::CVector4D rotation;
};

bool fn_71001bc1e8(const UnkRotated247* self, base::math::CVector4D* out, u32 mode) {
    base::math::CVector4D q;
    q.x = 0.0f;
    q.y = 0.0f;
    q.z = 0.0f;
    q.w = 1.0f;
    if (mode == 1)
        fn_710007b518(&q, &self->rotation);
    out->x = q.x;
    out->y = q.y;
    out->z = q.z;
    out->w = q.w;
    return true;
}
