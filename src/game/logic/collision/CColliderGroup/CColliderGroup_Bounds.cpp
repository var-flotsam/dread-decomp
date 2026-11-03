// game::logic::collision::CColliderGroup (unit 2504 of tools/tu_map.py): refreshes the group through its slot
// 0x88, then returns one of two blocks (+0x6c when the flag at +0x68 is set, else +0xac). Names and layouts are
// placeholders.
#include "types.h"

struct UnkColliderGroupBlock {
    u8 data[0x40];
};

struct UnkColliderGroupBounds {
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

    u8 pad_0x8[0x60];
    bool flag_0x68;
    u8 pad_0x69[0x3];
    UnkColliderGroupBlock block_0x6c;
    UnkColliderGroupBlock block_0xac;
};

UnkColliderGroupBlock* fn_7100d45c98(UnkColliderGroupBounds* self) {
    self->vf_0x88();
    return self->flag_0x68 ? &self->block_0x6c : &self->block_0xac;
}
