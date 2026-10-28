// Destructor (fn_710063904c) and deleting destructor (fn_71006390f0) of an object that embeds an
// attack (vtable 0x7101a57f68) at +0x146c8: the attack's destructor (its listener at +0x70, with
// its own vtable and unregister function, then CAttack's), then the base's (fn_7100369380).
#include "../../game_5821e4/CAttackDtor.h"

extern void* g_unk_7101a58510[];  // the object's vtable
extern void* g_unk_7101a57f68[];  // attack (vtable 0x7101a57f68)'s vtable

void fn_7100369380(void* self);
void fn_71010cb2fc(void* owner, void* listener);
extern void* g_unk_7101a2aef8[];  // the listener's vtable

struct UnkAttack_710063904c {
    void** vtable;
    u8 pad_0x8[0x68];
    UnkAttackListenerRaw field_0x70;

    void Destroy() {
        vtable = g_unk_7101a57f68 + 2;
        field_0x70.Destroy(g_unk_7101a2aef8, fn_71010cb2fc);
        fn_71003606c0(this);
    }
};

struct UnkOwner_710063904c {
    void** vtable;
    u8 pad_0x8[0x146c0];
    UnkAttack_710063904c field_0x146c8;

    void Destroy() {
        vtable = g_unk_7101a58510 + 2;
        field_0x146c8.Destroy();
        fn_7100369380(this);
    }
};

void fn_710063904c(UnkOwner_710063904c* self) {
    self->Destroy();
}

void fn_71006390f0(UnkOwner_710063904c* self) {
    self->Destroy();
    operator delete(self);
}
