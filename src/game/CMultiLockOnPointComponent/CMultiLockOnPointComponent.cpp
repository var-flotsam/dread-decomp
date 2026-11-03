// Constructor (copy of fn_710077e428 in CProtoCentralUnitComponent.cpp): base constructor, vtable, the
// container at +0x40 (base vtable, empty array with step 10 and flags 0xf, final vtable, allocator),
// with the flag at +0x3d cleared first,
// then the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

void fn_7100877960(void* self);
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];
extern void* g_unk_7101a7bbd0[];

struct UnkMultiLockOnPointComponent_4884 {
    void** vtable;
    u8 pad_0x8[0x35];
    bool field_0x3d;
    u8 pad_0x3e[0x2];
    void** field_0x40;
    u64 field_0x48;
    u64 field_0x50;
    u32 field_0x58;
    bool field_0x5c;
    u32 field_0x60;
    base::UnkAllocator* field_0x68;
    u32 field_0x70;
    u64 field_0x78;
};

void fn_7100884884(UnkMultiLockOnPointComponent_4884* self) {
    fn_7100877960(self);
    self->vtable = g_unk_7101a7bbd0 + 2;
    self->field_0x3d = false;
    self->field_0x40 = g_unk_7101a20998 + 2;
    self->field_0x48 = 0;
    self->field_0x50 = 0;
    self->field_0x58 = 10;
    self->field_0x5c = true;
    self->field_0x60 = 0xf;
    self->field_0x68 = fn_7100083990();
    self->field_0x70 = 0;
    self->field_0x40 = g_unk_7101a33fa0 + 2;
    self->field_0x78 = 0;
    reinterpret_cast<void (*)(UnkMultiLockOnPointComponent_4884*)>(self->vtable[3])(self);
}
