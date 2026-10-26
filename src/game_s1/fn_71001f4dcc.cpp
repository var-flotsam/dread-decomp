// Constructor of a widget-like object (0x108+ bytes): sets the base vtable, an empty string id,
// the default state and flags, then the derived vtable, builds the member at +0x108 from the
// argument and finishes by calling virtual slot 0x50 (the tail call).
#include "base/UnkInitRecord_0x5c.h"
#include "base/UnkVector.h"
#include "game/UnkSphere.h"
#include "types.h"

extern void* g_unk_7101a1f000[];
extern void* g_unk_7101a1f218[];
void fn_71000003d4(void* self, const char* s, bool flag);
void fn_710021685c(void* self);

struct UnkWidgetArg;
struct UnkWidgetMember {
    u8 data[8];
    UnkWidgetMember() {}
    UnkWidgetMember(const UnkWidgetMember&) {}
};
UnkWidgetMember fn_7100151360(UnkWidgetArg* a);

struct UnkWidgetBuffer {
    s64 field_0x0, field_0x8;
    s32 field_0x10;
    u8 flag_0x14;
    u8 pad_0x15[3];
    s32 field_0x18;
    u8 pad_0x1c[4];
};

struct UnkWidget {
    void** vtable_0x0;
    void* field_0x8;
    u8 id_0x10[8];
    u8 sub_0x18[0x5c];
    s32 field_0x74;
    void* field_0x78;
    UnkSphere sub_0x80;
    s32 field_0xd0, field_0xd4, field_0xd8, field_0xdc;
    f32 field_0xe0;
    s32 field_0xe4, field_0xe8, field_0xec, field_0xf0;
    u8 flag_0xf4, flag_0xf5, flag_0xf6, flag_0xf7, flag_0xf8, flag_0xf9, flag_0xfa, flag_0xfb,
        flag_0xfc, flag_0xfd;
    u8 pad_0xfe[2];
    u32 field_0x100;
    u8 pad_0x104[4];
    UnkWidgetMember member_0x108;
    UnkWidgetBuffer buffer_0x110;
    base::UnkAllocator* allocator_0x130;
    f32 field_0x138, field_0x13c, field_0x140, field_0x144;
    u8 pad_0x148[0x14];
    f32 field_0x15c, field_0x160;
    s32 field_0x164, field_0x168, field_0x16c, field_0x170, field_0x174;
    f32 field_0x178;
    s32 field_0x17c;
    u8 flag_0x180, flag_0x181, flag_0x182, flag_0x183;
    u8 flag_0x184;
    u8 pad_0x185[3];
    s32 field_0x188;
};

typedef void (*UnkVf50)(UnkWidget*, int);

void fn_71001f4dcc(UnkWidget* self, UnkWidgetArg* a) {
    self->field_0x8 = nullptr;
    self->vtable_0x0 = g_unk_7101a1f000 + 2;
    fn_71000003d4(self->id_0x10, "", true);
    fn_7100025624(self->sub_0x18);
    self->field_0x74 = -1;
    self->field_0x78 = nullptr;
    fn_710021685c(&self->sub_0x80);
    self->field_0xd0 = 0;
    self->field_0xd4 = 0;
    self->field_0xd8 = -1;
    self->field_0xdc = -1;
    self->field_0xe0 = -1.0f;
    self->field_0xe4 = 0;
    self->field_0xe8 = 0;
    self->field_0xec = 0;
    self->field_0xf0 = 0;
    self->flag_0xf4 = 0;
    self->flag_0xf5 = 1;
    self->flag_0xf6 = 1;
    self->flag_0xf7 = 1;
    self->flag_0xf8 = 1;
    self->flag_0xf9 = 0;
    self->flag_0xfa = 1;
    self->flag_0xfb = 1;
    self->flag_0xfc = 1;
    self->flag_0xfd = 1;
    self->field_0x100 = 0x600d600d;
    self->vtable_0x0 = g_unk_7101a1f218 + 2;
    new (&self->member_0x108) UnkWidgetMember(fn_7100151360(a));
    self->buffer_0x110.field_0x0 = 0;
    self->buffer_0x110.field_0x8 = 0;
    self->buffer_0x110.field_0x10 = 10;
    self->buffer_0x110.flag_0x14 = 1;
    self->buffer_0x110.field_0x18 = 15;
    self->allocator_0x130 = fn_7100083990();
    self->field_0x138 = 1.0f;
    self->field_0x13c = 1.0f;
    self->field_0x140 = 1.0f;
    self->field_0x144 = 1.0f;
    self->field_0x15c = 3.4028235e38f;
    self->field_0x160 = 1.0f;
    self->field_0x164 = 0;
    self->field_0x168 = 0;
    self->field_0x16c = 0;
    self->field_0x170 = 0;
    self->field_0x174 = 0;
    self->field_0x178 = 100.0f;
    self->field_0x17c = 0;
    self->flag_0x180 = 0;
    self->flag_0x181 = 0;
    self->flag_0x182 = 1;
    self->flag_0x183 = 1;
    self->flag_0x184 = 0;
    self->field_0x188 = 0;
    fn_710021693c(&self->sub_0x80, 0.0f);
}
