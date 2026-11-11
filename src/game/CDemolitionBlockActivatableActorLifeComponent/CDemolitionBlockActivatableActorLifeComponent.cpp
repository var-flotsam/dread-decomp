// Constructor (copy of fn_710077e428 in CProtoCentralUnitComponent.cpp): base constructor, vtable, the
// container at +0x468 (base vtable, empty array with step 10 and flags 0xf, final vtable, allocator),
// then the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

void fn_71008c4b74(void* self);
extern void* g_unk_7101a7e1c0[];

struct UnkDemolitionBlockActivatableActorLifeComponent_40dc {
    void** vtable;
    u8 pad_0x8[0x468 - 8];
    void** field_0x468;
    u64 field_0x470;
    u64 field_0x478;
    u32 field_0x480;
    bool field_0x484;
    u32 field_0x488;
    base::UnkAllocator* field_0x490;
    u32 field_0x498;
    u64 field_0x4a0;
};

void fn_71008c40dc(UnkDemolitionBlockActivatableActorLifeComponent_40dc* self) {
    fn_71008c4b74(self);
    self->vtable = g_unk_7101a7e1c0 + 2;
    self->field_0x468 = g_unk_7101a20998 + 2;
    self->field_0x470 = 0;
    self->field_0x478 = 0;
    self->field_0x480 = 10;
    self->field_0x484 = true;
    self->field_0x488 = 0xf;
    self->field_0x490 = fn_7100083990();
    self->field_0x498 = 0;
    self->field_0x468 = g_unk_7101a33fa0 + 2;
    self->field_0x4a0 = 0;
    reinterpret_cast<void (*)(UnkDemolitionBlockActivatableActorLifeComponent_40dc*)>(self->vtable[3])(self);
}
