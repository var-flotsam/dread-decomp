// CRailCameraCtrl unit (tools/tu_map.py unit 1497): the constructor of fn_71009d63bc
// (CRailCameraCtrl_Ctor_A5_b73.cpp) followed by the base object's class-entry pass (fn_7100023854).
// Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "game/CCameraBoundaryCtrl.h"
#include "types.h"

struct UnkStruct_7100763ed4;
struct UnkNode;

extern void* g_unk_7101a89a80[];  // vtable

struct UnkRailCameraCtrl_ovb0049 {
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
static_assert(__builtin_offsetof(UnkRailCameraCtrl_ovb0049, field_0x2f0) == 0x2f0, "layout");

void fn_71009d799c(UnkRailCameraCtrl_ovb0049* self) {
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
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
