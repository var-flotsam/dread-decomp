// Unit 816 (tools/tu_map.py, 0x71005725f8-0x7100573104): fn_71005726c0 switches an object on or off; switching
// on arms it (+0x5c = 1, +0x60 = +0x38) and calls vtable slot 3 with 0, switching off clears +0x64; both end in
// vtable slot 7. Names and layouts are placeholders.
#include "types.h"

struct UnkToggle_b62a3 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3(f32);
    virtual void vf_4();
    virtual void vf_5();
    virtual void vf_6();
    virtual void vf_7();
    u8 pad_0x8[0x30];
    u32 field_0x38;
    u8 pad_0x3c[0xc];
    bool on;
    u8 pad_0x49[0x13];
    u32 field_0x5c;
    u32 field_0x60;
    bool field_0x64;
};

void fn_71005726c0(UnkToggle_b62a3* self, bool on) {
    if (self->on == on)
        return;
    self->on = on;
    if (on) {
        self->field_0x5c = 1;
        self->field_0x60 = self->field_0x38;
        self->vf_3(0.0f);
    } else {
        self->field_0x64 = false;
    }
    self->vf_7();
}
