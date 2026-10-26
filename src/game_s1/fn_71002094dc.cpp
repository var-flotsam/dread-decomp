// Constructor of a widget-like object (0x108+ bytes): sets the base vtable, an empty string id,
// the default state and flags, then the derived vtable, builds the member at +0x108 from the
// argument, calls virtual slot 0x50 and then clears one flag and sets two fields.
#include "base/UnkInitRecord_0x5c.h"
#include "types.h"

inline void* operator new(unsigned long, void* p) { return p; }

extern void* g_unk_7101a1f000[];
extern void* g_unk_7101a1f800[];
void fn_71000003d4(void* self, const char* s, bool flag);
void fn_710021685c(void* self);
void fn_710021693c(void* self, f32 v);

struct UnkWidgetArg;
struct UnkWidgetMember {
    u8 data[8];
    UnkWidgetMember() {}
    UnkWidgetMember(const UnkWidgetMember&) {}
};
UnkWidgetMember fn_71001512a0(UnkWidgetArg* a);

struct UnkWidget {
    void** vtable_0x0;
    void* field_0x8;
    u8 id_0x10[8];
    u8 sub_0x18[0x5c];
    s32 field_0x74;
    void* field_0x78;
    u8 sub_0x80[0x50];
    s32 field_0xd0, field_0xd4, field_0xd8, field_0xdc;
    f32 field_0xe0;
    s32 field_0xe4, field_0xe8, field_0xec, field_0xf0;
    u8 flag_0xf4, flag_0xf5, flag_0xf6, flag_0xf7, flag_0xf8, flag_0xf9, flag_0xfa, flag_0xfb,
        flag_0xfc, flag_0xfd;
    u8 pad_0xfe[2];
    u32 field_0x100;
    u8 pad_0x104[4];
    UnkWidgetMember member_0x108;
    s32 field_0x110, field_0x114, field_0x118, field_0x11c, field_0x120, field_0x124, field_0x128,
        field_0x12c, field_0x130, field_0x134;
    u8 flag_0x138;
};

typedef void (*UnkVf50)(UnkWidget*, int);

void fn_71002094dc(UnkWidget* self, UnkWidgetArg* a) {
    self->field_0x8 = nullptr;
    self->vtable_0x0 = g_unk_7101a1f000 + 2;
    fn_71000003d4(self->id_0x10, "", true);
    fn_7100025624(self->sub_0x18);
    self->field_0x74 = -1;
    self->field_0x78 = nullptr;
    fn_710021685c(self->sub_0x80);
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
    self->vtable_0x0 = g_unk_7101a1f800 + 2;
    new (&self->member_0x108) UnkWidgetMember(fn_71001512a0(a));
    self->field_0x110 = 0;
    self->field_0x114 = 0;
    self->field_0x118 = 0;
    self->field_0x11c = 0;
    self->field_0x120 = 0;
    self->field_0x124 = 0;
    self->field_0x128 = 0;
    self->field_0x12c = 0;
    self->field_0x130 = 0;
    self->field_0x134 = 0;
    self->flag_0x138 = 0;
    fn_710021693c(self->sub_0x80, 0.0f);
    ((UnkVf50)self->vtable_0x0[10])(self, 1);
    self->flag_0xf7 = 0;
    self->flag_0xf5 = 1;
    self->field_0xd4 = 1;
}
