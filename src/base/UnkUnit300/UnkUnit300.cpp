// Unit 300 of tools/tu_map.py (0x7100211a84-0x710021234c): applying a pair of three-word records
// (+0x20 and +0x2c of the source) through the target's vf_0xb0, the second only when the first
// was accepted. Names and layouts are placeholders.
#include "types.h"

struct UnkRecord300 {
    u32 a;
    u32 b;
    u32 c;
};

struct UnkTarget300 {
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
    virtual bool vf_0xb0(u32 a, u32 b, u32 c);
};

struct UnkSource300 {
    u8 pad_0x0[0x20];
    UnkRecord300 first;   // 0x20
    UnkRecord300 second;  // 0x2c
};

bool fn_7100211b08(UnkTarget300* self, const UnkSource300* source) {
    if (!self->vf_0xb0(source->first.a, source->first.b, source->first.c))
        return false;
    return self->vf_0xb0(source->second.a, source->second.b, source->second.c);
}
