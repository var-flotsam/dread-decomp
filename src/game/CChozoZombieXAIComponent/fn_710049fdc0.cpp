// The CChozoZombieXAIComponent unit's copies of the Scourge AI component's destructors
// (src/game/CScourgeAIComponent/fn_710069f1a0.cpp): an object that embeds an attack at +0x146c8 whose
// listener sits at +0x38: the attack's destructor (its listener, then CAttack's), then the base's
// (fn_7100369380). fn_710049fdc0 destroys, fn_710049fe64 also frees.
#include "../../game_5821e4/CAttackDtor.h"

extern void* g_unk_7101a3d6b0[];  // the object's vtable
extern void* g_unk_7101a3d4d0[];  // CChozoZombieXPoisonClawsAttack's vtable

void fn_7100369380(void* self);

struct UnkAttack_710049fdc0 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkAttackListenerRaw field_0x38;

    void Destroy() {
        vtable = g_unk_7101a3d4d0 + 2;
        field_0x38.Destroy();
        fn_71003606c0(this);
    }
};

struct UnkOwner_710049fdc0 {
    void** vtable;
    u8 pad_0x8[0x146c0];
    UnkAttack_710049fdc0 field_0x146c8;

    void Destroy() {
        vtable = g_unk_7101a3d6b0 + 2;
        field_0x146c8.Destroy();
        fn_7100369380(this);
    }
};

void fn_710049fdc0(UnkOwner_710049fdc0* self) {
    self->Destroy();
}

void fn_710049fe64(UnkOwner_710049fdc0* self) {
    self->Destroy();
    operator delete(self);
}
