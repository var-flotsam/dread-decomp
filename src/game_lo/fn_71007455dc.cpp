// A position: the stored offset (0x2c) on its own when the mode at 0xd8 is 1 or there is no
// parent (0x10); otherwise the parent's position (fn_7100745e24) plus that offset.
#include "types.h"

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkParent_71007455dc;
UnkVec3Result fn_7100745e24(UnkParent_71007455dc* parent);

struct UnkStruct_71007455dc {
    u8 unk_0x0[0x10];
    UnkParent_71007455dc* parent;
    u8 unk_0x18[0x14];
    UnkVec3Result offset;  // 0x2c
    u8 unk_0x38[0xa0];
    s32 mode;  // 0xd8
};

UnkVec3Result fn_71007455dc(UnkStruct_71007455dc* self) {
    if (self->mode != 1 && self->parent) {
        UnkVec3Result p = fn_7100745e24(self->parent);
        return UnkVec3Result(p.x + self->offset.x, p.y + self->offset.y, p.z + self->offset.z);
    }
    return self->offset;
}
