// Destructor (fn_71005a4260) and deleting destructor (fn_71005a4304) of an object that embeds a
// CGroundShockerAttack at +0x146d0: the attack's destructor (its listener at +0x40, then
// CAttack's), then the base's (fn_71005a24b0).
#include "CAttackDtor.h"

extern void* g_unk_7101a4ee08[];  // the object's vtable
extern void* g_unk_7101a4e298[];  // CGroundShockerAttack's vtable

void fn_71005a24b0(void* self);

struct UnkGroundShockerAttack_71005a4260 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkAttackListenerRaw field_0x40;

    void Destroy() {
        vtable = g_unk_7101a4e298 + 2;
        field_0x40.Destroy();
        fn_71003606c0(this);
    }
};

struct UnkOwner_71005a4260 {
    void** vtable;
    u8 pad_0x8[0x146c8];
    UnkGroundShockerAttack_71005a4260 field_0x146d0;

    void Destroy() {
        vtable = g_unk_7101a4ee08 + 2;
        field_0x146d0.Destroy();
        fn_71005a24b0(this);
    }
};

void fn_71005a4260(UnkOwner_71005a4260* self) {
    self->Destroy();
}

void fn_71005a4304(UnkOwner_71005a4260* self) {
    self->Destroy();
    operator delete(self);
}
