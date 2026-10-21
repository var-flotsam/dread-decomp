// Destructor of an attack (vtable 0x7101a55fa8): its string id (+0x78), its listener (+0x30),
// then CAttack's.
#include "CAttackDtor.h"

extern void* g_unk_7101a55fa8[];  // the attack's vtable

struct UnkAttack_710060b114 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkAttackListenerRaw field_0x30;
    u8 pad_0x50[0x28];
    UnkAttackStrIdRaw field_0x78;
};

void fn_710060b114(UnkAttack_710060b114* self) {
    self->vtable = g_unk_7101a55fa8 + 2;
    self->field_0x78.Release();
    self->field_0x30.Destroy();
    fn_71003606c0(self);
}
