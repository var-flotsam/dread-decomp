// Rebuilds the quantised bounds of an object: after the base update, reads its minimum and
// maximum corner through two virtual getters and stores each component times ten as an int.
#include "game/UnkBounded.h"
#include "game/UnkTU307.h"
#include "types.h"

struct UnkVec3f {
    f32 x, y, z;
    UnkVec3f() {}
    UnkVec3f(const UnkVec3f& o) : x(o.x), y(o.y), z(o.z) {}
};

// `arg` (x1) reaches the base update untouched in the original (tools/forwarded_args.py).
void fn_7100212040(UnkBounded* self, const void* arg) {
    ExtendBoundingBoxToContainPoint(self, arg);
    UnkVec3f mn = self->GetMin();
    UnkVec3f mx = self->GetMax();
    self->min_x10 = (s32)(mn.x * 10.0f);
    self->min_y10 = (s32)(mn.y * 10.0f);
    self->min_z10 = (s32)(mn.z * 10.0f);
    self->max_x10 = (s32)(mx.x * 10.0f);
    self->max_y10 = (s32)(mx.y * 10.0f);
    self->max_z10 = (s32)(mx.z * 10.0f);
}
