// Destructor (fn_710069f1a0) and deleting destructor (fn_710069f244) of an object that embeds a
// CScourgeTongueSlashAttack at +0x146c8: the attack's destructor (its listener at +0x40, then CAttack's),
// then the base's (fn_7100369380).
#include "../../game_5821e4/CAttackDtor.h"

extern void* g_unk_7101a61f08[];  // the object's vtable
extern void* g_unk_7101a61c20[];  // CScourgeTongueSlashAttack's vtable

void fn_7100369380(void* self);

struct UnkAttack_710069f1a0 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkAttackListenerRaw field_0x40;

    void Destroy() {
        vtable = g_unk_7101a61c20 + 2;
        field_0x40.Destroy();
        fn_71003606c0(this);
    }
};

struct UnkOwner_710069f1a0 {
    void** vtable;
    u8 pad_0x8[0x146c0];
    UnkAttack_710069f1a0 field_0x146c8;

    void Destroy() {
        vtable = g_unk_7101a61f08 + 2;
        field_0x146c8.Destroy();
        fn_7100369380(this);
    }
};

void fn_710069f1a0(UnkOwner_710069f1a0* self) {
    self->Destroy();
}

void fn_710069f244(UnkOwner_710069f1a0* self) {
    self->Destroy();
    operator delete(self);
}
