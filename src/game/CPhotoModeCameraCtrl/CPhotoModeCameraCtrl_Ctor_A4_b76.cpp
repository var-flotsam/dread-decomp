// CPhotoModeCameraCtrl (the name from reflection), fn_71009ca94c: the plain constructor, the same stores as the
// in-place one in CPhotoModeCameraCtrl_Ctor_A4_b72.cpp without its closing virtual call. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/CCameraCtrl.h"
#include "game/CPhotoModeCameraCtrl.h"
#include "types.h"

struct UnkBase_7100763ed4;
extern void* g_unk_7101a89018[];
extern const char g_unk_71015dca2c[] __attribute__((visibility("hidden")));  // the class name string

struct UnkPhotoModeCameraCtrl_A4_b76 {
    void** vtable;
    u8 pad_0x8[0x20];
    u32 field_0x28;
    u8 pad_0x2c[0x1d8 - 0x2c];
    base::UnkVector<u8> list;  // 0x1d8
    f32 field_0x200;
    f32 field_0x204;
    u32 field_0x208;
    u8 pad_0x20c[0x4];
    u8 member[0x34];  // 0x210
    u8 field_0x244;
    u8 pad_0x245[0xbb0 - 0x245];
    u64 field_0xbb0;
    u64 field_0xbb8;
    u32 field_0xbc0;
    u8 pad_0xbc4[0x4];
    u64 field_0xbc8;
    u64 field_0xbd0;
    u64 field_0xbd8;
    u16 field_0xbe0;
    u8 pad_0xbe2[0x2];
    f32 field_0xbe4;
};
static_assert(__builtin_offsetof(UnkPhotoModeCameraCtrl_A4_b76, field_0xbe4) == 0xbe4, "layout");

void fn_71009ca94c(UnkPhotoModeCameraCtrl_A4_b76* self) {
    fn_71010ed044(reinterpret_cast<UnkBase_7100763ed4*>(self));
    self->vtable = &g_unk_7101a89018[2];
    new (&self->list) base::UnkVector<u8>(0xf);
    self->field_0x200 = 1500.0f;
    self->field_0x204 = 1800.0f;
    self->field_0x208 = 0;
    fn_71009ca9fc(self->member, g_unk_71015dca2c, 0x03f00f03);
    self->field_0xbb0 = 0;
    self->field_0xbb8 = 0;
    self->field_0xbc0 = 0;
    self->field_0xbc8 = 0;
    self->field_0xbd0 = 0;
    self->field_0xbd8 = 0;
    self->field_0xbe0 = 1;
    self->field_0xbe4 = 45.0f;
    self->field_0x28 = 6;
    self->field_0x244 = 1;
}
