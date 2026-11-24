// Unit 244: projects a world position to viewport pixels through the camera's view and projection:
// an orthographic box scaled with the eye's distance along the view direction, or a perspective
// from the camera's field of view and the viewport's aspect. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "game/UnkTU311.h"
#include "game/UnkTU54.h"
#include "types.h"

struct UnkMat4f {
    f32 m[4][4];
};

struct UnkFlipState_b0151;

UnkMatrix44 MakeOrthoProjectionMatrixZeroToOne(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
UnkMat4f MakePerspectiveFovMatrix(f32 fov, f32 aspect, f32 near_z, f32 far_z);
void fn_710015bf00(bool keep, UnkFlipState_b0151* state);
void fn_710015bef0(UnkMat4f* m, f32 value);

// The camera's shared settings: its view matrix and whether it is orthographic.
struct UnkCameraSettings_l1bb1cc {
    u8 pad_0x0[0x20];
    UnkMat4f view;  // 0x20
    u8 pad_0x60[0x50];
    bool ortho;  // 0xb0
};
static_assert(__builtin_offsetof(UnkCameraSettings_l1bb1cc, ortho) == 0xb0, "UnkCameraSettings layout");

struct UnkCamera_l1bb1cc {
    u8 pad_0x0[0x80];
    f32 fov;  // 0x80
    u8 pad_0x84[0x4];
    f32 near;  // 0x88
    f32 far;   // 0x8c
    u8 pad_0x90[0x4];
    f32 field_0x94;
    f32 field_0x98;
    u8 pad_0x9c[0xc];
    UnkVec3f eye;  // 0xa8
    u8 pad_0xb4[0x24];
    UnkVec3f target;  // 0xd8
    u8 pad_0xe4[0x8c];
    UnkCameraSettings_l1bb1cc* settings;  // 0x170
};
static_assert(__builtin_offsetof(UnkCamera_l1bb1cc, settings) == 0x170, "UnkCamera layout");

struct UnkViewport_l1bb1cc {
    s32 x;
    s32 y;
    u32 width;
    u32 height;
};

namespace {

// Element by element.
inline void CopyMatrix(UnkMat4f* dst, const f32* src) {
    dst->m[0][0] = src[0];
    dst->m[0][1] = src[1];
    dst->m[0][2] = src[2];
    dst->m[0][3] = src[3];
    dst->m[1][0] = src[4];
    dst->m[1][1] = src[5];
    dst->m[1][2] = src[6];
    dst->m[1][3] = src[7];
    dst->m[2][0] = src[8];
    dst->m[2][1] = src[9];
    dst->m[2][2] = src[10];
    dst->m[2][3] = src[11];
    dst->m[3][0] = src[12];
    dst->m[3][1] = src[13];
    dst->m[3][2] = src[14];
    dst->m[3][3] = src[15];
}

}  // namespace

void fn_71001bb1cc(UnkCamera_l1bb1cc* self, const UnkViewport_l1bb1cc* viewport, const UnkVec3f* pos, s32* out_x,
                   s32* out_y) {
    UnkMat4f projection;
    UnkMat4f view;
    if (self->settings->ortho) {
        f32 ex = self->eye.x;
        f32 ey = self->eye.y;
        f32 ez = self->eye.z;
        f32 dx = self->target.x - ex;
        f32 dy = self->target.y - ey;
        f32 dz = self->target.z - ez;
        f32 length = sqrtf(dx * dx + dy * dy + dz * dz);
        f32 inv = 1.0f / (length + 0.0001f);
        f32 nx = dx * inv;
        f32 ny = dy * inv;
        f32 nz = dz * inv;
        f32 distance = -(ey * ny) - ex * nx - ez * nz;
        f32 scale = distance * 0.0007f;
        scale = !(scale < 1.0f) ? scale : 1.0f;
        f32 width = viewport->width;
        f32 height = viewport->height;
        CopyMatrix(&projection, MakeOrthoProjectionMatrixZeroToOne(width * -0.5f * scale, width * 0.5f * scale, height * -0.5f * scale,
                                              height * 0.5f * scale, self->near, self->far)
                                    .m);
        CopyMatrix(&view, &self->settings->view.m[0][0]);
    } else {
        f32 aspect = (f32)viewport->width / (f32)viewport->height;
        f32 fov = fn_7100217ee0(self->fov, aspect);
        projection = MakePerspectiveFovMatrix((f32)(fov * 3.141592653589793 / 180.0), aspect, 1.0f, 10000000.0f);
        fn_710015bf00(false, reinterpret_cast<UnkFlipState_b0151*>(&projection));
        CopyMatrix(&view, &self->settings->view.m[0][0]);
        fn_710015bef0(&view, self->field_0x94 * self->field_0x98);
    }
    UnkVec3f in_view = TransformPointByMatrixWithPerspectiveDivide(&view, pos);
    UnkVec3f clip = TransformPointByMatrixWithPerspectiveDivide(&projection, &in_view);
    *out_x = viewport->x + (s32)((clip.x + 1.0f) * viewport->width * 0.5f);
    *out_y = viewport->y + (s32)((1.0f - clip.y) * viewport->height * 0.5f);
}
