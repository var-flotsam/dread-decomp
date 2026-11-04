// CCharClassAIComponent unit (tools/tu_map.py, the name from reflection): true when one of two masks
// (+0x454 when vf_0xc0 says so, else +0x450) is non-zero and every flag of the third argument's word at
// +0xb4 is in it. Names and layouts are placeholders.
#include "types.h"

struct UnkMaskInfo_b66a2 {
    u8 pad_0x0[0xb4];
    u32 flags;
};

struct UnkCharClassAI_b66a2 {
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
    virtual bool vf_0xc0();

    u8 pad_0x8[0x448];
    u32 mask;
    u32 alt_mask;
};

bool fn_7100414010(UnkCharClassAI_b66a2* self, void* actor, const UnkMaskInfo_b66a2* info) {
    u32 mask = self->vf_0xc0() ? self->alt_mask : self->mask;
    if (!mask) {
        return false;
    }
    return (info->flags & ~mask) == 0;
}
