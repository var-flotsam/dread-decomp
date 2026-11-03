// Steps in the CActor unit (unit 341 of tools/tu_map.py): call one virtual on each enabled part
// (its flag at +0x20) of the actor's part map at +0xa0 (as src/game/CActor/fn_710023259c.cpp walks
// it). Names and layouts are placeholders.
#include "types.h"

class UnkActorPart_7100233970 {
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
    virtual void vf_0x110();

    u8 pad_0x8[0x18];
    bool enabled;  // 0x20
};

struct UnkActor_7100233970 {
    u8 pad_0x0[0xa0];
    UnkActorPart_7100233970** parts;  // 0xa0
    void* ids;                        // 0xa8
    u32 part_count;                   // 0xb0
    u8 pad_0xb4[0x101];
    bool frozen;  // 0x1b5
};

typedef void (UnkActorPart_7100233970::*UnkActorPartFn)();

template <UnkActorPartFn Fn>
__attribute__((always_inline)) static inline void ForEachEnabledPart(UnkActor_7100233970* self) {
    for (UnkActorPart_7100233970 **it = self->parts, **end = it + self->part_count; it != end; ++it) {
        UnkActorPart_7100233970* part = *it;
        if (part->enabled)
            (part->*Fn)();
    }
}

void fn_7100232eac(UnkActor_7100233970* self) {
    if (self->frozen)
        return;
    for (UnkActorPart_7100233970 **it = self->parts, **end = it + self->part_count; it != end; ++it) {
        UnkActorPart_7100233970* part = *it;
        if (part && part->enabled)
            part->vf_0xa0();
    }
}

void fn_7100233970(UnkActor_7100233970* self) {
    ForEachEnabledPart<&UnkActorPart_7100233970::vf_0x108>(self);
}

void fn_71002339d4(UnkActor_7100233970* self) {
    ForEachEnabledPart<&UnkActorPart_7100233970::vf_0x110>(self);
}
