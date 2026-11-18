// CMovementComponent unit (tools/tu_map.py unit 2688): the angles that turn a rotation (given as
// angles) to stand on the movement's up vector (+0x58): the basis is rebuilt around up from the
// rotation's second column (its first when that is nearly parallel to up), unless the owner
// answers fn_71007ea32c. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "game/UnkTU54.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

void* fn_71007ea32c(void* actor);
void fn_71000769f0(UnkMatrix44* out, const void* a, const void* b, const void* c, const void* d);
__attribute__((visibility("hidden"))) extern const UnkVec3f g_unk_710164ceb8;

struct UnkMovement_l0d2cf4 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x20];
    UnkVec3f up;  // 0x58
};

// A column of the rotation part of a row-major matrix.
inline UnkVec3f Column(const UnkMatrix44& m, int i) {
    return UnkVec3f(m.m[i], m.m[4 + i], m.m[8 + i]);
}

UnkVec3f fn_71010d2cf4(UnkMovement_l0d2cf4* self, const UnkVec3f* angles) {
    UnkMatrix44 rotation = fn_7100078fc8(reinterpret_cast<const UnkMatrix44&>(*angles));
    UnkVec3f up = self->up;
    UnkVec3f forward = Column(rotation, 1);
    UnkVec3f side = Cross(forward, up);
    if (!fn_71007ea32c(self->owner)) {
        f32 d = fabsf(Dot(Column(rotation, 1), self->up));
        f32 low = d - 0.001f;
        f32 high = d + 0.001f;
        if (high >= 1.0f && low <= 1.0f) {
            forward = Cross(up, Column(rotation, 0));
            side = Cross(forward, up);
        } else {
            side = Cross(Column(rotation, 1), up);
            forward = Cross(up, side);
        }
    }
    UnkMatrix44 basis;
    fn_71000769f0(&basis, &side, &forward, &up, &g_unk_710164ceb8);
    return fn_7100079b18(reinterpret_cast<UnkXform*>(&basis), angles->z);
}
