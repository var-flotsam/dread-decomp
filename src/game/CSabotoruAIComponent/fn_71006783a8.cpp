// Destructor (fn_71006783a8) and deleting destructor (fn_710067844c) of an object that embeds a
// CSabotoruTurnInDoorAttack at +0x14780: the attack's destructor (its listener at +0x38, then CAttack's),
// then the base's (fn_7100369380).
#include "../../game_5821e4/CAttackDtor.h"

extern void* g_unk_7101a5e9d8[];  // the object's vtable
extern void* g_unk_7101a5e490[];  // CSabotoruTurnInDoorAttack's vtable

void fn_7100369380(void* self);

struct UnkAttack_71006783a8 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkAttackListenerRaw field_0x38;

    void Destroy() {
        vtable = g_unk_7101a5e490 + 2;
        field_0x38.Destroy();
        fn_71003606c0(this);
    }
};

struct UnkOwner_71006783a8 {
    void** vtable;
    u8 pad_0x8[0x14778];
    UnkAttack_71006783a8 field_0x14780;

    void Destroy() {
        vtable = g_unk_7101a5e9d8 + 2;
        field_0x14780.Destroy();
        fn_7100369380(this);
    }
};

void fn_71006783a8(UnkOwner_71006783a8* self) {
    self->Destroy();
}

void fn_710067844c(UnkOwner_71006783a8* self) {
    self->Destroy();
    operator delete(self);
}
