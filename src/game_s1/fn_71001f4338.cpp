// Sets the shown flag of a widget-like object; when it changes, tells the listener, copies the
// style block (fields 0x28..0x6d) from the object at +0x108 into itself and calls virtual slot
// 0x30 (tail call) with the address of the copied block.
#include "types.h"

struct UnkListener;
struct UnkToggleSrc {
    u8 pad_0x0[0x28];
    u32 f_0x28;
    u8 pad_0x2c[0xc];
    u32 f_0x38, f_0x3c, f_0x40, f_0x44, f_0x48, f_0x4c, f_0x50;
    u8 pad_0x54[4];
    u64 f_0x58;
    u32 f_0x60, f_0x64, f_0x68;
    u8 f_0x6c, f_0x6d;
};
struct UnkToggleDst;
struct UnkToggleVt {
    void* slot[6];
    void (*slot_0x30)(UnkToggleDst*, void*);
};
struct UnkToggleDst {
    UnkToggleVt* vtable_0x0;
    UnkListener* listener_0x8;
    u8 pad_0x10[0x78];
    u32 f_0x88;
    u8 pad_0x8c[0xc];
    u32 f_0x98, f_0x9c, f_0xa0, f_0xa4, f_0xa8, f_0xac, f_0xb0;
    u8 pad_0xb4[4];
    u64 f_0xb8;
    u32 f_0xc0, f_0xc4, f_0xc8;
    u8 f_0xcc, f_0xcd;
    u8 pad_0xce[0x28];
    u8 flag_0xf6;
    u8 pad_0xf7;
    u8 flag_0xf8;
    u8 flag_0xf9;
    u8 pad_0xfa[0xe];
    UnkToggleSrc* src_0x108;
};
void fn_71001eef58(UnkListener* l, UnkToggleDst* t, bool b);
void fn_71001eef74(UnkListener* l, UnkToggleDst* t, bool b);

void fn_71001f4338(UnkToggleDst* self, bool on) {
    if (on != self->flag_0xf9) {
        self->flag_0xf9 = on;
        if (self->listener_0x8) {
            fn_71001eef58(self->listener_0x8, self, on);
            fn_71001eef74(self->listener_0x8, self, self->flag_0xf9 && self->flag_0xf6);
        }
        UnkToggleSrc* s = self->src_0x108;
        self->f_0x88 = s->f_0x28;
        self->f_0x98 = s->f_0x38;
        self->f_0x9c = s->f_0x3c;
        self->f_0xa0 = s->f_0x40;
        self->f_0xa4 = s->f_0x44;
        self->f_0xa8 = s->f_0x48;
        self->f_0xac = s->f_0x4c;
        self->f_0xb0 = s->f_0x50;
        self->f_0xb8 = s->f_0x58;
        self->f_0xc0 = s->f_0x60;
        self->f_0xc4 = s->f_0x64;
        self->f_0xc8 = s->f_0x68;
        self->f_0xcc = s->f_0x6c;
        self->f_0xcd = s->f_0x6d;
        self->vtable_0x0->slot_0x30(self, &self->f_0x98);
    }
}
