// CGoliathAttack's destructor: its listener (+0x48), its string id (+0x40), then CAttack's.
#include "../../game_5821e4/CAttackDtor.h"

extern void* g_unk_7101a4c108[];  // CGoliathAttack's vtable

struct UnkAttack_7100587280 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkAttackStrIdRaw field_0x40;
    UnkAttackListenerRaw field_0x48;
};

void fn_7100587280(UnkAttack_7100587280* self) {
    self->vtable = g_unk_7101a4c108 + 2;
    self->field_0x48.Destroy();
    self->field_0x40.Release();
    fn_71003606c0(self);
}
