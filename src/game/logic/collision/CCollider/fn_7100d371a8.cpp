// game::logic::collision::CCollider unit: stores a flag byte and calls virtual slot 0xa8 on each
// child in the inline array at +0x168 (count at +0x164). Names and layouts are placeholders.
#include "types.h"

struct UnkColliderChild {
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
};

struct UnkColliderA {
    u8 pad_0x0[0x6b];
    u8 flag;  // 0x6b
    u8 pad_0x6c[0xf8];
    u32 count;  // 0x164
    UnkColliderChild* children[1];  // 0x168
};

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_7100d371a8(UnkColliderA* self, u32 value) {
    self->flag = value & 1;
    UnkColliderChild** data = self->children;
    for (UnkColliderChild** it = data; it != data + self->count; ++it)
        (*it)->vf_0xa8();
}
