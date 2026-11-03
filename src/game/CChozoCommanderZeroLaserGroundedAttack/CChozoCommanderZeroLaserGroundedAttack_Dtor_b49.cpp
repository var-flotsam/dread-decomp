// CChozoCommanderZeroLaserGroundedAttack (tools/tu_map.py): the destructors. The vtable, the listener at
// +0xa0 (game_5821e4/CAttackDtor.h), the array at +0x78 (base::UnkVector, freed), then CAttack's
// destructor; fn_7100448964 also frees the object. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "../../game_5821e4/CAttackDtor.h"

extern void* g_unk_7101a36718[];  // the attack's vtable

struct UnkZeroLaserGroundedAttack_b49 {
    void** vtable;
    u8 pad_0x8[0x70];
    base::UnkVector<u64> field_0x78;
    UnkAttackListenerRaw listener;

    __attribute__((always_inline)) void Destroy() {
        vtable = g_unk_7101a36718 + 2;
        listener.Destroy();
        field_0x78.Free();
        fn_71003606c0(this);
    }
};
static_assert(__builtin_offsetof(UnkZeroLaserGroundedAttack_b49, listener) == 0xa0, "layout");

void fn_71004488b8(UnkZeroLaserGroundedAttack_b49* self) {
    self->Destroy();
}

void fn_7100448964(UnkZeroLaserGroundedAttack_b49* self) {
    self->Destroy();
    operator delete(self);
}
