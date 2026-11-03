// CLockOnMissileGun unit (tools/tu_map.py): true when fn_710084eb44 holds, the value of the virtual
// at +0x1e0 is at least 3, the flag at +0x199c is clear and the count at +0x19b8 is zero. Names
// are placeholders.
#include "types.h"

struct UnkLockOnMissileGun_b49a4 {
    void** vtable;
    u8 pad_0x8[0x199c - 0x8];
    bool field_0x199c;
    u8 pad_0x199d[0x19b8 - 0x199d];
    s32 field_0x19b8;

    f32 vf_0x1e0() { return reinterpret_cast<f32 (*)(UnkLockOnMissileGun_b49a4*)>(vtable[0x1e0 / 8])(this); }
};

bool fn_710084eb44(UnkLockOnMissileGun_b49a4* self);

bool fn_710084d400(UnkLockOnMissileGun_b49a4* self) {
    if (fn_710084eb44(self) && !(self->vf_0x1e0() < 3.0f) && !self->field_0x199c && self->field_0x19b8 == 0)
        return true;
    return false;
}
