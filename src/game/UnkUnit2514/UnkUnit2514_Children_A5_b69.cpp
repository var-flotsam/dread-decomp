// Unit 2514 (no reflection hint): an owner with an inline list of child objects at +0xa8 (a u32
// count followed directly by the pointers, so they sit at +0xac) and the calls it forwards to each
// child. Names and layouts are placeholders.
#include "types.h"

struct UnkUnit2514Child_A5_b69 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(f32 v);
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(void* arg);
    virtual bool vf_0x50(void* arg);
    virtual bool vf_0x58();
    virtual void vf_0x60(bool flag);
};

struct __attribute__((packed, aligned(4))) UnkUnit2514List_A5_b69 {
    u32 count;
    UnkUnit2514Child_A5_b69* items[8];

    UnkUnit2514Child_A5_b69** begin() { return items; }
    UnkUnit2514Child_A5_b69** end() {
        u64 bytes = (u64)count * sizeof(items[0]);
        asm("" : "+r"(bytes));  // the original keeps the byte size, not the count
        return reinterpret_cast<UnkUnit2514Child_A5_b69**>(reinterpret_cast<char*>(items) + bytes);
    }
};

struct UnkUnit2514Owner_A5_b69 {
    u8 pad_0x0[0xa8];
    UnkUnit2514List_A5_b69 children;
};

void fn_7100d62234(UnkUnit2514Owner_A5_b69* self, f32 v) {
    for (u64 i = 0; i < self->children.count;)
        self->children.items[i++]->vf_0x30(v);
}

void fn_7100d6238c(UnkUnit2514Owner_A5_b69* self, void* arg) {
    for (UnkUnit2514Child_A5_b69* child : self->children)
        if (child)
            child->vf_0x48(arg);
}

bool fn_7100d62478(UnkUnit2514Owner_A5_b69* self) {
    if (self->children.count == 0)
        return false;
    for (UnkUnit2514Child_A5_b69* child : self->children)
        if (child && !child->vf_0x58())
            return false;
    return true;
}

void fn_7100d624e4(UnkUnit2514Owner_A5_b69* self, bool flag) {
    for (UnkUnit2514Child_A5_b69* child : self->children)
        if (child)
            child->vf_0x60(flag);
}

bool fn_7100d623f8(UnkUnit2514Owner_A5_b69* self, void* arg) {
    if (self->children.count == 0)
        return false;
    for (UnkUnit2514Child_A5_b69* child : self->children)
        if (child && !child->vf_0x50(arg))
            return false;
    return true;
}
