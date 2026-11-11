// CHydrogigaGrabCameraCtrl (unit 1324): the constructor. The base constructor (fn_71010ed044), the
// vtable, two empty string ids with cleared words after them (+0x1d8 and +0x1f8; the shape of
// CNailuggerAcidBallMovementComponent_b40.cpp), then the base fields it overrides. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CCameraCtrl.h"
#include "types.h"

struct UnkBase_7100763ed4;
extern void* g_unk_7101a7a408[];

struct UnkGrabCameraCtrlCtor {
    void** vtable;
    u8 pad_0x8[0x20];
    u32 field_0x28;
    u8 pad_0x2c[0x9];
    bool field_0x35;
    u8 pad_0x36[0x66];
    u32 field_0x9c;
    u8 pad_0xa0[0x4];
    u32 field_0xa4;
    u8 pad_0xa8[0x18];
    f32 field_0xc0;
    f32 field_0xc4;
    u8 pad_0xc8[0x110];
    base::global::UnkStringData* camera;  // 0x1d8
    u64 field_0x1e0;
    u32 field_0x1e8;
    u8 pad_0x1ec[0xc];
    base::global::UnkStringData* field_0x1f8;
    u64 field_0x200;
};
static_assert(__builtin_offsetof(UnkGrabCameraCtrlCtor, camera) == 0x1d8, "layout");

static inline __attribute__((always_inline)) void InitEmpty_hg(base::global::UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_710086e7c8(UnkGrabCameraCtrlCtor* self) {
    fn_71010ed044(reinterpret_cast<UnkBase_7100763ed4*>(self));
    self->vtable = g_unk_7101a7a408 + 2;
    InitEmpty_hg(&self->camera);
    self->field_0x1e0 = 0;
    self->field_0x1e8 = 0;
    InitEmpty_hg(&self->field_0x1f8);
    self->field_0x200 = 0;
    self->field_0x28 = 3;
    self->field_0x35 = true;
    self->field_0xa4 = 0;
    self->field_0x9c = 0;
    self->field_0xc0 = 1000.0f;
    self->field_0xc4 = 1000.0f;
}
