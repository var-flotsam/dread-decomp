// Sets the shown flag of a widget-like object; when it changes, tells the listener, copies the
// style block (fields 0x28..0x6d) from the object at +0x108 into itself and calls virtual slot
// 0x30 (tail call) with the address of the copied block.
#include "engine/scene/UnkToggle.h"

void fn_71001f887c(UnkToggle* self, bool on) {
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
