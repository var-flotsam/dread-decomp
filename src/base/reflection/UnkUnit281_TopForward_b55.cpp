// Unit 281 (tools/tu_map.py): calls forwarded to the object on top of a stack of pointers (+0x48,
// count at +0x88).
#include "types.h"

struct UnkUnit281Handler {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(void* a, void* b);
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68(void* a, void* b, void* c);
};

struct UnkUnit281Stack {
    u8 pad_0x0[0x48];
    UnkUnit281Handler* items[8];
    u32 count;  // +0x88
};

void fn_71002004d8(UnkUnit281Stack* self, void* a, void* b) {
    self->items[self->count - 1]->vf_0x30(a, b);
}

void fn_7100202f1c(UnkUnit281Stack* self, void* a, void* b, void* c) {
    self->items[self->count - 1]->vf_0x68(a, b, c);
}
