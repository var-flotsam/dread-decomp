// Constructor (copy of fn_710077e428 in CProtoCentralUnitComponent.cpp): base constructor, vtable, the
// container at +0x570 (base vtable, empty array with step 10 and flags 0xf, final vtable, allocator),
// then the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CTrainUsableComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

extern void* g_unk_7101a93aa0[];

struct UnkTrainUsableComponentCutScene_b308 {
    void** vtable;
    u8 pad_0x8[0x570 - 8];
    void** field_0x570;
    u64 field_0x578;
    u64 field_0x580;
    u32 field_0x588;
    bool field_0x58c;
    u32 field_0x590;
    base::UnkAllocator* field_0x598;
    u32 field_0x5a0;
    u64 field_0x5a8;
};

void fn_7100a9b308(UnkTrainUsableComponentCutScene_b308* self) {
    fn_7100a9948c(self);
    self->vtable = g_unk_7101a93aa0 + 2;
    self->field_0x570 = g_unk_7101a20998 + 2;
    self->field_0x578 = 0;
    self->field_0x580 = 0;
    self->field_0x588 = 10;
    self->field_0x58c = true;
    self->field_0x590 = 0xf;
    self->field_0x598 = fn_7100083990();
    self->field_0x5a0 = 0;
    self->field_0x570 = g_unk_7101a33fa0 + 2;
    self->field_0x5a8 = 0;
    reinterpret_cast<void (*)(UnkTrainUsableComponentCutScene_b308*)>(self->vtable[3])(self);
}
