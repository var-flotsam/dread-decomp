// A step in the CActor unit (unit 341 of tools/tu_map.py): calls the virtual at 0x60 of each object
// in the actor's map at +0xa0 (its values, as src/game/CActor/fn_710023341c.cpp looks them up), then
// the actor's own virtual at 0x78. Names and layouts are placeholders.
#include "types.h"

struct UnkActorPart_71002329e8;
struct UnkActorPartVtbl_71002329e8 {
    u8 pad_0x0[0x60];
    void (*vf_0x60)(UnkActorPart_71002329e8* self);
};
struct UnkActorPart_71002329e8 {
    UnkActorPartVtbl_71002329e8* vtable;
};

struct UnkActor_71002329e8;
struct UnkActorVtbl_71002329e8 {
    u8 pad_0x0[0x78];
    void (*vf_0x78)(UnkActor_71002329e8* self);
};
struct UnkActor_71002329e8 {
    UnkActorVtbl_71002329e8* vtable;
    u8 pad_0x8[0x98];
    UnkActorPart_71002329e8** parts;  // 0xa0
    void* ids;                        // 0xa8
    u32 part_count;                   // 0xb0
};

void fn_71002329e8(UnkActor_71002329e8* self) {
    for (UnkActorPart_71002329e8 **it = self->parts, **end = it + self->part_count; it != end; ++it)
        (*it)->vtable->vf_0x60(*it);
    self->vtable->vf_0x78(self);
}
