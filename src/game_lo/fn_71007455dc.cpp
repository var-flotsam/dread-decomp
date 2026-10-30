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

// The other way round: stores `pos` as the offset from the parent's position (or as it is, as above).
// NON_MATCHING(minor): register allocation only.
void fn_710074566c(UnkStruct_71007455dc* self, const f32* pos) {
    if (self->parent && self->mode != 1) {
        UnkVec3Result p = fn_7100745e24(self->parent);
        f32 px = p.x, py = p.y, pz = p.z;
        f32 x = pos[0], y = pos[1], z = pos[2];
        self->offset.x = x - px;
        self->offset.y = y - py;
        self->offset.z = z - pz;
        return;
    }
    self->offset.x = pos[0];
    self->offset.y = pos[1];
    self->offset.z = pos[2];
}
