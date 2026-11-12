// In the CKraidAcidBlobsMovementComponent unit: fn_7100604be8 rejects a null `other`, the one held
// at +0x230 (+0x10) and one whose part at +0x1b8 says so (fn_7101161088); otherwise it forwards to
// fn_7100ac4ec0 when `other` has a part at +0x240 or +0x200, else to fn_7100ac4c60. Names and layouts
// are placeholders.
#include "game/CLifeComponent.h"
#include "game/CProjectileMovement.h"
#include "types.h"

struct UnkObj;
struct UnkBossComponent;

struct UnkKraidAcidBlobsHeld {
    u8 pad_0x0[0x10];
    void* other;
};

struct UnkKraidAcidBlobsOther {
    u8 pad_0x0[0x1b8];
    void* field_0x1b8;
    u8 pad_0x1c0[0x40];
    void* field_0x200;
    u8 pad_0x208[0x38];
    void* field_0x240;
};

struct UnkKraidAcidBlobsMovement {
    u8 pad_0x0[0x230];
    UnkKraidAcidBlobsHeld* field_0x230;
};

bool fn_7100604be8(UnkKraidAcidBlobsMovement* self, UnkKraidAcidBlobsOther* other, void* a2, void* a3, void* a4,
                   void* a5) {
    if (!other)
        return false;
    UnkKraidAcidBlobsHeld* held = self->field_0x230;
    if ((held ? held->other : nullptr) == other)
        return false;
    if (other->field_0x1b8 && fn_7101161088(other->field_0x1b8))
        return false;
    if (other->field_0x240 || other->field_0x200)
        return fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), other, a2, a3, a4, a5);
    return fn_7100ac4c60(reinterpret_cast<UnkBossComponent*>(self), reinterpret_cast<u64>(a2),
                         reinterpret_cast<u64>(a3), reinterpret_cast<u64>(a4), reinterpret_cast<u64>(a5));
}
