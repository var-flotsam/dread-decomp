// Constructor (copy of fn_710077e428 in CProtoCentralUnitComponent.cpp): base constructor, vtable, the
// container at +0x428 (base vtable, empty array with step 10 and flags 0xf, final vtable, allocator),
// then the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU341.h"
#include "types.h"

void fn_7100a5dac0(void* self);
extern void* g_unk_7101a8f070[];

struct UnkColliderTriggerComponent_cb9c {
    void** vtable;
    u8 pad_0x8[0x428 - 8];
    void** field_0x428;
    u64 field_0x430;
    u64 field_0x438;
    u32 field_0x440;
    bool field_0x444;
    u32 field_0x448;
    base::UnkAllocator* field_0x450;
    u32 field_0x458;
    u64 field_0x460;
};

void fn_7100a4cb9c(UnkColliderTriggerComponent_cb9c* self) {
    fn_7100a5dac0(self);
    self->vtable = g_unk_7101a8f070 + 2;
    self->field_0x428 = g_unk_7101a20998 + 2;
    self->field_0x430 = 0;
    self->field_0x438 = 0;
    self->field_0x440 = 10;
    self->field_0x444 = true;
    self->field_0x448 = 0xf;
    self->field_0x450 = fn_7100083990();
    self->field_0x458 = 0;
    self->field_0x428 = g_unk_7101a20950 + 2;
    self->field_0x460 = 0;
    reinterpret_cast<void (*)(UnkColliderTriggerComponent_cb9c*)>(self->vtable[3])(self);
}
