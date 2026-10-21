// Destructor of an object that embeds two attacks: one with vtable 0x7101a515e8 at +0x14758
// (listener at +0x30) and a CInfesterShootAttack at +0x146f8 (listener at +0x38). Each runs its
// listener's destructor and CAttack's, in reverse order; then the base's (fn_7100369380).
#include "CAttackDtor.h"

extern void* g_unk_7101a51af0[];  // the object's vtable
extern void* g_unk_7101a515e8[];  // the second attack's vtable
extern void* g_unk_7101a516e0[];  // CInfesterShootAttack's vtable

void fn_7100369380(void* self);

struct UnkAttack_71005ca8d4 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkAttackListenerRaw field_0x38;
    u8 pad_0x58[0x8];

    void Destroy() {
        vtable = g_unk_7101a516e0 + 2;
        field_0x38.Destroy();
        fn_71003606c0(this);
    }
};

struct UnkAttack2_71005ca8d4 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkAttackListenerRaw field_0x30;

    void Destroy() {
        vtable = g_unk_7101a515e8 + 2;
        field_0x30.Destroy();
        fn_71003606c0(this);
    }
};

struct UnkOwner_71005ca8d4 {
    void** vtable;
    u8 pad_0x8[0x146f0];
    UnkAttack_71005ca8d4 field_0x146f8;
    UnkAttack2_71005ca8d4 field_0x14758;

    __attribute__((always_inline)) void Destroy() {
        vtable = g_unk_7101a51af0 + 2;
        field_0x14758.Destroy();
        field_0x146f8.Destroy();
        fn_7100369380(this);
    }
};

void fn_71005ca8d4(UnkOwner_71005ca8d4* self) {
    self->Destroy();
}
