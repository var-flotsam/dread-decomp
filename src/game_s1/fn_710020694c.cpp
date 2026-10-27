// Constructor of a widget-like object (0x108+ bytes): sets the base vtable, an empty string id,
// the default state and flags, then the derived vtable, builds the member at +0x108 from the
// argument and finishes by calling virtual slot 0x50 (the tail call).
#include "base/UnkInitRecord_0x5c.h"
#include "game/UnkSphere.h"
#include "types.h"

inline void* operator new(u64, void* p) { return p; }

extern void* g_unk_7101a1f000[];
extern void* g_unk_7101a1f5e0[];
void fn_71000003d4(void* self, const char* s, bool flag);
void fn_710021685c(void* self);

struct UnkWidgetArg;
struct UnkWidgetMember {
    u8 data[8];
    UnkWidgetMember() {}
    UnkWidgetMember(const UnkWidgetMember&) {}
};
UnkWidgetMember fn_7100151260(UnkWidgetArg* a);

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
    u32 field_0x110;
    s32 field_0x114, field_0x118, field_0x11c, field_0x120, field_0x124;
    u8 field_0x128, field_0x129;
    u8 flag_0x12a, flag_0x12b, flag_0x12c;
};

typedef void (*UnkVf50)(UnkWidget*, int);

void fn_710020694c(UnkWidget* self, UnkWidgetArg* a) {
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
    self->vtable_0x0 = g_unk_7101a1f5e0 + 2;
    new (&self->member_0x108) UnkWidgetMember(fn_7100151260(a));
    self->field_0x110 = 0x7f7fffff;
    self->field_0x114 = 0;
    self->field_0x118 = 0;
    self->field_0x11c = 0;
    self->field_0x120 = 0;
    self->field_0x124 = 0;
    self->field_0x128 = 0;
    self->field_0x129 = 0;
    self->flag_0x12a = 1;
    self->flag_0x12b = 1;
    self->flag_0x12c = 0;
    fn_710021693c(&self->sub_0x80, 0.0f);
    self->flag_0xf7 = 0;
    self->flag_0xf5 = 1;
}
