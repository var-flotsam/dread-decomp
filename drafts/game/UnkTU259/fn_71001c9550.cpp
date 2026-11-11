#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

struct UnkQuadTarget;
struct UnkBoundTarget;
struct UnkHandle;

struct UnkCamState_b0154q {
    UnkQuadTarget* quad;  // 0x0
    u8 pad_0x8[0x3a0];
    UnkBoundTarget* target;  // 0x3a8
    u8 pad_0x3b0[0x160];
    u32 field_0x510;
    u32 field_0x514;
    u32 field_0x518;
    u32 field_0x51c;
    f32 field_0x520;
    f32 field_0x524;
};

struct UnkGfxQ2_b0154 {
    u8 pad_0x0[0x317aa20];
    UnkCamState_b0154q cam;  // 0x317aa20
};

u32 fn_7100132fc0();
u32 fn_7100132fd0();
u32 fn_7100132f80();
u32 fn_7100132f90();
f32 fn_7100133000();
void fn_710015c250(UnkBoundTarget* target);
void fn_710015c1e0(UnkHandle** out, s32 width, s32 height, s32 count, const s32& format);
void* fn_710015c450(UnkBoundTarget* target);
void fn_71001606c0(void* texture, u32* a, u32* b);
void fn_710019c124(UnkQuadTarget* self, u32 index, const CVector2D& value);
void fn_710019c2ac(UnkQuadTarget* self);

#define UNK_MIN(a, b) ((a) < (b) ? (a) : (b))

void fn_71001c9550(UnkGfxQ2_b0154* self, bool full) {
    fn_7100132fc0();
    fn_7100132fd0();
    f32 width = (f32)fn_7100132f80();
    f32 scale_x = fn_7100133000();
    f32 height = (f32)fn_7100132f90();
    f32 scale_y = fn_7100133000();
    u32 view_w = UNK_MIN(fn_7100132fc0(), fn_7100132f80());
    scale_x *= width;
    scale_y *= height;
    UnkCamState_b0154q* cam = &self->cam;
    u32 view_h = UNK_MIN(fn_7100132fd0(), fn_7100132f90());
    u32 target_w = (u32)scale_x;
    u32 target_h = (u32)scale_y;
    u32 x = (fn_7100132fc0() - view_w) >> 1;
    u32 y = full ? 0 : (fn_7100132fd0() - view_h) >> 1;
    cam->field_0x518 = view_w;
    cam->field_0x510 = x;
    cam->field_0x514 = y;
    cam->field_0x51c = view_h;
    cam->field_0x520 = 0.0f;
    cam->field_0x524 = 1.0f;
    if (cam->target) {
        fn_710015c250(cam->target);
        cam->target = nullptr;
    }
    fn_710015c1e0((UnkHandle**)&cam->target, target_w, target_h, 3, 2);
    u32 zero_a = 0;
    u32 zero_b = 0;
    fn_71001606c0(fn_710015c450(cam->target), &zero_a, &zero_b);
    fn_710019c124(cam->quad, 0, CVector2D{0.0f, 0.0f});
    fn_710019c124(cam->quad, 1, CVector2D{1.0f, 0.0f});
    fn_710019c124(cam->quad, 2, CVector2D{0.0f, 1.0f});
    fn_710019c124(cam->quad, 3, CVector2D{1.0f, 1.0f});
    fn_710019c2ac(cam->quad);
}
