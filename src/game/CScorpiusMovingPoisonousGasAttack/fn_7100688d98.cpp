// CScorpiusMovingPoisonousGasAttack's destructor (fn_7100688d98) and deleting destructor
// (fn_7100688e5c): its listener (+0x78), then CScorpiusPoisonousGasAttack's part (vtable store,
// the ref-counted pointer at +0x38), then CAttack's. CScorpiusAttack's destructor stores no
// vtable.
#include "../../game_5821e4/CAttackDtor.h"

extern void* g_unk_7101a60790[];  // CScorpiusMovingPoisonousGasAttack's vtable
extern void* g_unk_7101a60890[];  // CScorpiusPoisonousGasAttack's vtable

struct UnkAttack_7100688d98 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkAttackOwner* field_0x38;
    u8 pad_0x40[0x38];
    UnkAttackListenerRaw field_0x78;

    void Destroy() {
        vtable = g_unk_7101a60790 + 2;
        field_0x78.Destroy();
        vtable = g_unk_7101a60890 + 2;
        if (UnkAttackOwner* p = field_0x38) {
            if (--p->ref_count == 0)
                p->Delete();
        }
        fn_71003606c0(this);
    }
};

void fn_7100688d98(UnkAttack_7100688d98* self) {
    self->Destroy();
}

void fn_7100688e5c(UnkAttack_7100688d98* self) {
    self->Destroy();
    operator delete(self);
}
