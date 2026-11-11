// CEmmyFloorSlideCameraCtrl: its constructor (fn_71007d3700). The camera controller base constructor
// fn_71010ed044, the vtable, a cleared flag at +0x1d8 and 24 bytes at +0x1dc, 3 at +0x28, a set flag at
// +0x35, the floats 0 and 0.3 at +0x1f4, then the class-entry pass of base::core::CBaseObject
// (fn_7100023854). Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "game/CCameraCtrl.h"
#include "types.h"

struct UnkNode;
struct UnkBase_7100763ed4;
extern void* g_unk_7101a72188[];

struct UnkEmmyFloorSlideCameraCtrl {
    void** vtable;
    u8 pad_0x8[0x20];
    u32 field_0x28;
    u8 pad_0x2c[9];
    bool field_0x35;
    u8 pad_0x36[0x1a2];
    bool field_0x1d8;
    u8 pad_0x1d9[3];
    u8 field_0x1dc[0x18];
    f32 field_0x1f4;
    f32 field_0x1f8;
};

void fn_71007d3700(UnkEmmyFloorSlideCameraCtrl* self) {
    fn_71010ed044(reinterpret_cast<UnkBase_7100763ed4*>(self));
    self->vtable = g_unk_7101a72188 + 2;
    self->field_0x1d8 = false;
    __builtin_memset(self->field_0x1dc, 0, sizeof(self->field_0x1dc));
    self->field_0x28 = 3;
    self->field_0x35 = true;
    self->field_0x1f4 = 0.0f;
    self->field_0x1f8 = 0.3f;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
