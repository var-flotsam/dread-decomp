// CChozoCommanderZeroLaserGroundedAttack (tools/tu_map.py): two identical constructors on the attack base
// (fn_710042baa8): the vtable, a cleared run at +0x5c-0x70, the array at +0x78 (base::UnkVector, flags 0xf)
// and the listener at +0xa0 (vtable g_unk_7101a22d48) with its fields cleared. The destructors are in
// CChozoCommanderZeroLaserGroundedAttack_Dtor_b49.cpp. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CChozoCommanderGroundAttack.h"
#include "types.h"

extern void* g_unk_7101a36718[];  // the attack's vtable
extern void* g_unk_7101a22d48[];  // the listener's vtable

struct UnkZeroLaserListener_b67a2 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkZeroLaserGroundedAttack_b67a2 {
    void** vtable;
    u8 pad_0x8[0x54];
    u8 field_0x5c[0x10];
    u32 field_0x6c;
    u8 pad_0x70[8];
    base::UnkVector<u64> field_0x78;
    UnkZeroLaserListener_b67a2 listener;
};

// Typed as CChozoCommanderHypersparkAttack_Ctors_b46.cpp declares it (one signature per symbol).
struct UnkHypersparkAttack_b46a1;

static inline __attribute__((always_inline)) void Construct(UnkZeroLaserGroundedAttack_b67a2* self) {
    fn_710042baa8(reinterpret_cast<UnkHypersparkAttack_b46a1*>(self));
    self->vtable = &g_unk_7101a36718[2];
    self->field_0x6c = 0;
    __builtin_memset(self->field_0x5c, 0, sizeof(self->field_0x5c));
    new (&self->field_0x78) base::UnkVector<u64>(0xf);
    self->listener.vtable = &g_unk_7101a22d48[2];
    self->listener.field_0x8 = 0;
    self->listener.field_0x10 = 0;
    self->listener.field_0x18 = 0;
}

void fn_71004466d0(UnkZeroLaserGroundedAttack_b67a2* self) { Construct(self); }
void fn_7100448a48(UnkZeroLaserGroundedAttack_b67a2* self) { Construct(self); }
