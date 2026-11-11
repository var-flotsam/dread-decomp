// Constructor (copy of fn_710077e428 in CProtoCentralUnitComponent.cpp): base constructor, vtable, the
// container at +0x3c0 (base vtable, empty array with step 10 and flags 0xf, final vtable, allocator),
// then the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "types.h"

extern void* g_unk_7101a8c108[];

struct UnkAutsharpSpawnPointComponent_482c {
    void** vtable;
    u8 pad_0x8[0x3c0 - 8];
    void** field_0x3c0;
    u64 field_0x3c8;
    u64 field_0x3d0;
    u32 field_0x3d8;
    bool field_0x3dc;
    u32 field_0x3e0;
    base::UnkAllocator* field_0x3e8;
    u32 field_0x3f0;
    u64 field_0x3f8;
};

void fn_7100a1482c(UnkAutsharpSpawnPointComponent_482c* self) {
    fn_71011258e4(self);
    self->vtable = g_unk_7101a8c108 + 2;
    self->field_0x3c0 = g_unk_7101a20998 + 2;
    self->field_0x3c8 = 0;
    self->field_0x3d0 = 0;
    self->field_0x3d8 = 10;
    self->field_0x3dc = true;
    self->field_0x3e0 = 0xf;
    self->field_0x3e8 = fn_7100083990();
    self->field_0x3f0 = 0;
    self->field_0x3c0 = g_unk_7101a20950 + 2;
    self->field_0x3f8 = 0;
    reinterpret_cast<void (*)(UnkAutsharpSpawnPointComponent_482c*)>(self->vtable[3])(self);
}
