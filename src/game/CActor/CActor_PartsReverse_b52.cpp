// Copies of fn_71002325f8 (src/game/CActor/CActor_PartsReverse_b51.cpp) in the CActor unit: each
// calls one virtual of every object in the actor's map at +0xa0, last to first. Names and layouts
// are placeholders.
#include "types.h"

class UnkActorPart_b52 {
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
};

struct UnkActorParts_b52 {
    u8 pad_0x0[0xa0];
    UnkActorPart_b52** parts;  // 0xa0
    void* ids;                 // 0xa8
    u32 part_count;            // 0xb0
};

void fn_71002326d0(void* object) {
    UnkActorParts_b52* self = static_cast<UnkActorParts_b52*>(object);
    for (s32 i = self->part_count - 1; i >= 0; --i)
        self->parts[static_cast<u32>(i)]->vf_0x88();
}

void fn_71002328dc(UnkActorParts_b52* self) {
    for (s32 i = self->part_count - 1; i >= 0; --i)
        self->parts[static_cast<u32>(i)]->vf_0x48();
}
