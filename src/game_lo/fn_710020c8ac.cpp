// Sets an enabled flag (0xf9) on an object. When it changes: tells the owner (if any) about the
// new state, refreshes a block of values from the object's description, notifies itself through
// a virtual hook, and mirrors the flag into the description.
#include "engine/scene/UnkToggle.h"

void fn_710020c8ac(UnkToggle* self, bool value) {
    if (value == self->flag_0xf9)
        return;
    self->flag_0xf9 = value;
    if (self->listener_0x8) {
        fn_71001eef58(self->listener_0x8, self, value);
        fn_71001eef74(self->listener_0x8, self, self->flag_0xf9 && self->flag_0xf6);
    }
    UnkToggleSrc* desc = self->src_0x108;
    self->f_0x88 = desc->f_0x28;
    self->f_0x98 = desc->f_0x38;
    self->f_0x9c = desc->f_0x3c;
    self->f_0xa0 = desc->f_0x40;
    self->f_0xa4 = desc->f_0x44;
    self->f_0xa8 = desc->f_0x48;
    self->f_0xac = desc->f_0x4c;
    self->f_0xb0 = desc->f_0x50;
    self->f_0xb8 = desc->f_0x58;
    self->f_0xc0 = desc->f_0x60;
    self->f_0xc4 = desc->f_0x64;
    self->f_0xc8 = desc->f_0x68;
    self->f_0xcc = desc->f_0x6c;
    self->f_0xcd = desc->f_0x6d;
    self->vtable_0x0->slot_0x30(self, &self->f_0x98);
    UnkToggleSrc* d = self->src_0x108;
    d->flag_0xe0 = self->flag_0xf9;
}
