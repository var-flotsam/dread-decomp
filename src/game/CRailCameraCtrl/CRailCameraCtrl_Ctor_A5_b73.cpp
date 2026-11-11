// CRailCameraCtrl unit (tools/tu_map.py, unit 1497): the rail camera controller's constructor. It runs the shared
// camera-controller base constructor (fn_7100763ed4), sets the vtable (g_unk_7101a89a80), clears its own
// fields (-1 at +0x2a8) and sets three base fields. Names and layouts are placeholders.
#include "game/CCameraBoundaryCtrl.h"
#include "types.h"

struct UnkStruct_7100763ed4;

extern void* g_unk_7101a89a80[];  // vtable

struct UnkRailCameraCtrl_A5_b73 {
    void* vtable;
    u8 unk_0x8[0x20];
    u32 field_0x28;
    u8 unk_0x2c[0xa4 - 0x2c];
    u32 field_0xa4;
    u8 unk_0xa8[0x265 - 0xa8];
    bool field_0x265;
    u8 unk_0x266[0x2a0 - 0x266];
    u64 field_0x2a0;
    s32 field_0x2a8;
    u64 field_0x2b0;
    u32 field_0x2b8;
    u64 field_0x2c0;
    u64 field_0x2c8;
    u64 field_0x2d0;
    u64 field_0x2d8;
    u64 field_0x2e0;
    u64 field_0x2e8;
    u64 field_0x2f0;
};
static_assert(__builtin_offsetof(UnkRailCameraCtrl_A5_b73, field_0x2f0) == 0x2f0, "layout");

void fn_71009d63bc(UnkRailCameraCtrl_A5_b73* self) {
    fn_7100763ed4(reinterpret_cast<UnkStruct_7100763ed4*>(self));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a89a80) + 0x10;
    self->field_0x2a0 = 0;
    self->field_0x2a8 = -1;
    self->field_0x2b0 = 0;
    self->field_0x2b8 = 0;
    self->field_0xa4 = 0;
    self->field_0x2c0 = 0;
    self->field_0x2c8 = 0;
    self->field_0x2d0 = 0;
    self->field_0x2d8 = 0;
    self->field_0x2e0 = 0;
    self->field_0x2e8 = 0;
    self->field_0x2f0 = 0;
    self->field_0x28 = 2;
    self->field_0x265 = true;
}
