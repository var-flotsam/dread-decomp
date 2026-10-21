// Rebuilds the quantised bounds of an object: after the base update, reads its minimum and
// maximum corner through two virtual getters and stores each component times ten as an int.
#include "types.h"

struct UnkVec3f {
    f32 x, y, z;
    UnkVec3f() {}
    UnkVec3f(const UnkVec3f& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkBounded {
    virtual void vf0();
    virtual void vf8();
    virtual void vf10();
    virtual void vf18();
    virtual void vf20();
    virtual void vf28();
    virtual void vf30();
    virtual UnkVec3f GetMin();
    virtual UnkVec3f GetMax();
    u8 pad_0x8[0x18];
    s32 min_x10, min_y10, min_z10;
    s32 max_x10, max_y10, max_z10;
};

void fn_7100216090(UnkBounded* self);

void fn_71002120f4(UnkBounded* self) {
    fn_7100216090(self);
    UnkVec3f mn = self->GetMin();
    UnkVec3f mx = self->GetMax();
    self->min_x10 = (s32)(mn.x * 10.0f);
    self->min_y10 = (s32)(mn.y * 10.0f);
    self->min_z10 = (s32)(mn.z * 10.0f);
    self->max_x10 = (s32)(mx.x * 10.0f);
    self->max_y10 = (s32)(mx.y * 10.0f);
    self->max_z10 = (s32)(mx.z * 10.0f);
}
