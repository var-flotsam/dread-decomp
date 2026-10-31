// Constructor of a component in the CProtoCentralUnitComponent unit: the base constructor
// fn_710076e964, the vtable (0x7101a6f108) and, at +0xa00, a container object built as
// src/game/CCentralUnitAIComponent/fn_71004076e4.cpp builds its own (base vtable 0x7101a20998,
// an empty array with step 10 and flags 0xf, then the final vtable 0x7101a33fa0 and the default
// allocator), then the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkProtoCentralUnitBase;
void fn_710076e964(UnkProtoCentralUnitBase* self);
extern void* g_unk_7101a6f108[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];

struct UnkProtoCentralUnit {
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

void fn_710077e428(UnkProtoCentralUnit* self) {
    fn_710076e964(reinterpret_cast<UnkProtoCentralUnitBase*>(self));
    self->vtable = g_unk_7101a6f108 + 2;
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
    reinterpret_cast<void (*)(UnkProtoCentralUnit*)>(self->vtable[3])(self);
}
