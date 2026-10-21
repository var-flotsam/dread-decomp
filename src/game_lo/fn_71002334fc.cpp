// Sets a flag and, when it changed, passes the new value on to every child in the object's
// list (virtual slot 0xe8).
#include "types.h"

struct UnkChild_71002334fc {
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
    virtual void vf_0xe8(bool value);
};

struct UnkStruct_71002334fc {
    u8 unk_0x0[0xa0];
    UnkChild_71002334fc** children;
    u8 unk_0xa8[0x8];
    u32 child_count;
    u8 unk_0xb4[0x1b3 - 0xb4];
    bool field_0x1b3;
};

void fn_71002334fc(UnkStruct_71002334fc* self, bool value) {
    if (self->field_0x1b3 == value)
        return;
    self->field_0x1b3 = value;
    UnkChild_71002334fc** it = self->children;
    UnkChild_71002334fc** end = it + self->child_count;
    for (; it != end; ++it)
        (*it)->vf_0xe8(self->field_0x1b3);
}
