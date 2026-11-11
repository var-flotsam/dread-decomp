// CPhotoModeCameraCtrl (the name from reflection): the in-place constructor its reflection descriptor points to
// (fn_0x28). After the camera controller base constructor (fn_71010ed044) it sets the vtable, builds an engine
// array (flags 0xf) at +0x1d8, a pair of distances (1500, 1800), the member at +0x210 (fn_71009ca9fc, with the
// class name string and a flags word), clears the fields at +0xbb0, sets an angle of 45 and a flag, then makes the
// post-construction virtual call (slot 3). Defined under the `void()` symbol src/reflection/CPhotoModeCameraCtrl.cpp
// declares. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CCameraCtrl.h"
#include "game/CPhotoModeCameraCtrl.h"
#include "types.h"

struct UnkBase_7100763ed4;
extern void* g_unk_7101a89018[];
extern const char g_unk_71015dca2c[] __attribute__((visibility("hidden")));  // the class name string

struct UnkPhotoModeCameraCtrl_A4_b72 {
    void** vtable;
    u8 pad_0x8[0x20];
    u32 field_0x28;
    u8 pad_0x2c[0x1d8 - 0x2c];
    void* list_data;  // 0x1d8
    u64 list_count;  // count and capacity
    u32 list_step;
    u8 list_grow;
    u32 list_flags;
    base::UnkAllocator* list_allocator;
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
static_assert(__builtin_offsetof(UnkPhotoModeCameraCtrl_A4_b72, field_0xbe4) == 0xbe4, "layout");

void fn_71009cbe24_A4_b72(UnkPhotoModeCameraCtrl_A4_b72* self) asm("_Z13fn_71009cbe24v");
void fn_71009cbe24_A4_b72(UnkPhotoModeCameraCtrl_A4_b72* self) {
    fn_71010ed044(reinterpret_cast<UnkBase_7100763ed4*>(self));
    self->vtable = &g_unk_7101a89018[2];
    self->list_data = nullptr;
    self->list_count = 0;
    self->list_step = 10;
    self->list_grow = 1;
    self->list_flags = 0xf;
    self->list_allocator = fn_7100083990();
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
    reinterpret_cast<void (*)(UnkPhotoModeCameraCtrl_A4_b72*)>(self->vtable[3])(self);
}
