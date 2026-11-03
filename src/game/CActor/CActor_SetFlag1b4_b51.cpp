// A setter in the CActor unit (unit 341 of tools/tu_map.py): stores the flag at +0x1b4, notifies
// through one of two virtuals (0x138 when set, 0x140 when cleared) unless the flag at +0x1b0 is set
// without the pointer at +0x1a8, then passes the flag to the virtual at 0x110.
// Names and layouts are placeholders.
#include "types.h"

class UnkActor_7100233910 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110(bool value);
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();

    u8 pad_0x8[0x1a0];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 pad_0x1b1[0x3];
    bool field_0x1b4;
};

void fn_7100233910(UnkActor_7100233910* self, bool value) {
    self->field_0x1b4 = value;
    if (!self->field_0x1b0 || self->field_0x1a8) {
        if (value)
            self->vf_0x138();
        else
            self->vf_0x140();
    }
    self->vf_0x110(self->field_0x1b4);
}
