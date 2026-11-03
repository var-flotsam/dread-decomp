// Constructor (copy of fn_710077e428 in CProtoCentralUnitComponent.cpp): base constructor, vtable, the
// container at +0xa00 (base vtable, empty array with step 10 and flags 0xf, final vtable, allocator),
// then the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkProtoCentralUnitBase;
void fn_710076e964(UnkProtoCentralUnitBase* self);
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];
extern void* g_unk_7101a6e918[];

struct UnkCaveCentralUnitComponent_86d0 {
    void** vtable;
    u8 pad_0x8[0x9f8];
    void** field_0xa00;
    u64 field_0xa08;
    u64 field_0xa10;
    u32 field_0xa18;
    bool field_0xa1c;
    u32 field_0xa20;
    base::UnkAllocator* field_0xa28;
    u32 field_0xa30;
    u64 field_0xa38;
};

void fn_71007686d0(UnkCaveCentralUnitComponent_86d0* self) {
    fn_710076e964(reinterpret_cast<UnkProtoCentralUnitBase*>(self));
    self->vtable = g_unk_7101a6e918 + 2;
    self->field_0xa00 = g_unk_7101a20998 + 2;
    self->field_0xa08 = 0;
    self->field_0xa10 = 0;
    self->field_0xa18 = 10;
    self->field_0xa1c = true;
    self->field_0xa20 = 0xf;
    self->field_0xa28 = fn_7100083990();
    self->field_0xa30 = 0;
    self->field_0xa00 = g_unk_7101a33fa0 + 2;
    self->field_0xa38 = 0;
    reinterpret_cast<void (*)(UnkCaveCentralUnitComponent_86d0*)>(self->vtable[3])(self);
}
