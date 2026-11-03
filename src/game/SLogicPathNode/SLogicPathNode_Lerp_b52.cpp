// SLogicPathNode unit (unit 2657 of tools/tu_map.py): a point along a list of 0x20-byte nodes whose
// fourth virtual (+0x18) gives the node's position. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

struct UnkLogicPathPoint_b52 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual const CVector3D* vf_3();
    u8 pad_0x8[0x18];
};

struct UnkLogicPathPoints_b52 {
    u8 pad_0x0[0x8];
    UnkLogicPathPoint_b52* data;
    u32 count;
};

// Returned in memory (through x8): a vector with a user-written copy constructor.
struct UnkLogicPathPos_b52 {
    f32 x, y, z;
    UnkLogicPathPos_b52(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkLogicPathPos_b52(const UnkLogicPathPos_b52& o) : x(o.x), y(o.y), z(o.z) {}
};

// The position of node `index` moved `t` of the way towards node `index + 1` (zero when `index` is
// out of range, the node itself when it is the last).
UnkLogicPathPos_b52 fn_71010596ac(UnkLogicPathPoints_b52* self, s32 index, f32 t) {
    UnkLogicPathPos_b52 result(0, 0, 0);
    if (index >= 0 && (u32)index < self->count) {
        const CVector3D* a = self->data[(u32)index].vf_3();
        result.x = a->x;
        result.y = a->y;
        result.z = a->z;
        if ((u32)(index + 1) < self->count) {
            const CVector3D* b = self->data[(u32)(index + 1)].vf_3();
            f32 x = (b->x - result.x) * t + result.x;
            f32 y = (b->y - result.y) * t + result.y;
            f32 z = (b->z - result.z) * t + result.z;
            result.x = x;
            result.y = y;
            result.z = z;
        }
    }
    return result;
}
