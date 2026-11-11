// CChozoCommanderDropCameraCtrl (unit 1203 of tools/tu_map.py, reflection hint): the update. Before the
// controller's progress (+0x2c) reaches 0.85 it is the camera-boundary controller's (fn_71007643c0);
// after, the same steps as CAnimatedCameraCtrl's (fn_71010ea3e4,
// src/game/CAnimatedCameraCtrl/CAnimatedCameraCtrl_Lifecycle_ovb0101.cpp) without the target check:
// the placement copied on a reset, the boundaries applied (fn_7100764a50) when enabled, and the
// position kept at +0x270. Names and layouts are placeholders.
#include "game/CCameraBoundaryCtrl.h"
#include "types.h"

struct UnkCamVec3_b0101 {
    f32 x, y, z;
    UnkCamVec3_b0101(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkCamVec3_b0101(const UnkCamVec3_b0101& o) : x(o.x), y(o.y), z(o.z) {}
    UnkCamVec3_b0101& operator=(const UnkCamVec3_b0101& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// A camera placement: a position and a direction.
struct UnkCamPlacement_ovb0109 {
    UnkCamVec3_b0101 pos;
    UnkCamVec3_b0101 dir;
    UnkCamPlacement_ovb0109& operator=(const UnkCamPlacement_ovb0109& o) {
        pos = o.pos;
        dir = o.dir;
        return *this;
    }
};

struct UnkDropCameraCtrl_ovb0109 {
    u8 pad_0x0[0x2c];
    f32 progress;  // 0x2c
    u8 pad_0x30[0x8];
    UnkCamPlacement_ovb0109 field_0x38;
    u8 pad_0x50[0x18];
    UnkCamPlacement_ovb0109 field_0x68;
    u8 pad_0x80[0x1b8];
    UnkCamVec3_b0101 field_0x238;
    u8 pad_0x244[0x20];
    bool field_0x264;
    u8 pad_0x265[0xb];
    UnkCamVec3_b0101 field_0x270;
};
static_assert(__builtin_offsetof(UnkDropCameraCtrl_ovb0109, field_0x270) == 0x270, "layout");

struct UnkAnimatedCameraCtrl_b0101;
// The camera-boundary controller's update (CCameraBoundaryCtrl unit).
void fn_71007643c0(UnkDropCameraCtrl_ovb0109* self, bool reset);

void fn_71007986ac(UnkDropCameraCtrl_ovb0109* self, bool reset) {
    if (!(self->progress >= 0.85f)) {
        fn_71007643c0(self, reset);
        return;
    }
    UnkCamVec3_b0101 from = self->field_0x238;
    f32 y = self->field_0x68.pos.y;
    f32 x = self->field_0x68.pos.x;
    f32 z = self->field_0x68.pos.z;
    if (reset)
        self->field_0x38 = self->field_0x68;
    if (self->field_0x264) {
        UnkCamVec3_b0101 to(x, y, z);
        fn_7100764a50(reinterpret_cast<UnkAnimatedCameraCtrl_b0101*>(self), &from, &to, reset);
    }
    if (reset)
        self->field_0x38 = self->field_0x68;
    self->field_0x270 = self->field_0x68.pos;
}
