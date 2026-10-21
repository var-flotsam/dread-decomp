// Destructor (fn_71006d40e4) and deleting destructor (fn_71006d4188) of an object that embeds a
// CTakumakuDashAttack at +0x146d0: the attack's destructor (its listener at +0x40, then CAttack's),
// then the base's (fn_7100369380).
#include "CAttackDtor.h"

extern void* g_unk_7101a67e00[];  // the object's vtable
extern void* g_unk_7101a67a90[];  // CTakumakuDashAttack's vtable

void fn_7100369380(void* self);

struct UnkAttack_71006d40e4 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkAttackListenerRaw field_0x40;

    void Destroy() {
        vtable = g_unk_7101a67a90 + 2;
        field_0x40.Destroy();
        fn_71003606c0(this);
    }
};

struct UnkOwner_71006d40e4 {
    void** vtable;
    u8 pad_0x8[0x146c8];
    UnkAttack_71006d40e4 field_0x146d0;

    void Destroy() {
        vtable = g_unk_7101a67e00 + 2;
        field_0x146d0.Destroy();
        fn_7100369380(this);
    }
};

void fn_71006d40e4(UnkOwner_71006d40e4* self) {
    self->Destroy();
}

void fn_71006d4188(UnkOwner_71006d40e4* self) {
    self->Destroy();
    operator delete(self);
}
