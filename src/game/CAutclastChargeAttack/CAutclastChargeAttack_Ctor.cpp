// CAutclastChargeAttack: two identical constructors (fn_71003b4500, fn_71003b7614) on the attack
// base constructor fn_710036069c. Names and layouts are placeholders.
#include "types.h"
#include "base/UnkVector.h"

void fn_710036069c(void* self);
extern void* g_unk_7101a2ae00[];
extern void* g_unk_7101a2aef8[];

struct UnkAutclastChargeAttack {
    void** vtable;
    u8 pad_0x8[0x20];
    u64 field_0x28;
    u8 pad_0x30[0x8];
    void** field_0x38;
    u64 field_0x40;
    u64 field_0x48;
    u64 field_0x50;
    u64 field_0x58;
    u64 field_0x60;
    u32 field_0x68;
    u64 field_0x70;
    u64 field_0x78;
    u8 pad_0x80[0x8];
    u64 field_0x88;
    u64 field_0x90;
    u64 field_0x98;
    u32 field_0xa0;
    bool field_0xa4;
    u32 field_0xa8;
    base::UnkAllocator* allocator;
};

__attribute__((always_inline)) static inline void Construct(UnkAutclastChargeAttack* self) {
    fn_710036069c(self);
    self->field_0x28 = 0;
    self->vtable = g_unk_7101a2ae00 + 2;
    self->field_0x38 = g_unk_7101a2aef8 + 2;
    __builtin_memset(&self->field_0x40, 0, 24);
    self->field_0x60 = 0;
    self->field_0x68 = 0;
    self->field_0x98 = 0;
    self->field_0x70 = 0;
    self->field_0x78 = 0;
    self->field_0x88 = 0;
    self->field_0x90 = 0;
    self->field_0xa0 = 10;
    self->field_0xa4 = true;
    self->field_0xa8 = 15;
    self->allocator = fn_7100083990();
}

void fn_71003b4500(UnkAutclastChargeAttack* self) {
    Construct(self);
}

void fn_71003b7614(UnkAutclastChargeAttack* self) {
    Construct(self);
}
