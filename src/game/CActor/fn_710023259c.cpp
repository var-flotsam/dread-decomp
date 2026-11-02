// A step in the CActor unit (unit 341 of tools/tu_map.py): calls the virtual at 0x90 of each object
// in the actor's map at +0xa0 (its values, as src/game/CActor/fn_71002329e8.cpp walks them), with
// the global flag g_unk_717269c4d8 set for the duration. Names and layouts are placeholders.
#include "types.h"

extern bool g_unk_717269c4d8;  // reached through the GOT

class UnkActorPart_710023259c {
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
};

struct UnkActor_710023259c {
    u8 pad_0x0[0xa0];
    UnkActorPart_710023259c** parts;  // 0xa0
    void* ids;                        // 0xa8
    u32 part_count;                   // 0xb0
};

void fn_710023259c(UnkActor_710023259c* self) {
    g_unk_717269c4d8 = true;
    for (UnkActorPart_710023259c **it = self->parts, **end = it + self->part_count; it != end; ++it)
        (*it)->vf_0x90();
    g_unk_717269c4d8 = false;
}
