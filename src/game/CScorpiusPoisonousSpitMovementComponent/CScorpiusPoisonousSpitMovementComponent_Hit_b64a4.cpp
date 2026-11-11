// CScorpiusPoisonousSpitMovementComponent unit (tools/tu_map.py, the name from reflection): rejects
// hits once done (+0x5d8) and on the object held at +0x230 (+0x10); otherwise forwards to
// fn_7100ac4ec0 and then notes the hit (fn_71006993e0). Names and layouts are placeholders.
#include "game/CProjectileMovement.h"
#include "types.h"

struct UnkObj;

struct UnkSpitHeld_b64a4 {
    u8 pad_0x0[0x10];
    void* other;
};

struct UnkSpitMovement_b64a4 {
    u8 pad_0x0[0x230];
    UnkSpitHeld_b64a4* held;
    u8 pad_0x238[0x3a0];
    bool done;  // 0x5d8
};

void fn_71006993e0(UnkSpitMovement_b64a4* self, void* other, void* a4);

bool fn_71006997e0(UnkSpitMovement_b64a4* self, void* other, void* a2, void* a3, void* a4, void* a5) {
    if (self->done)
        return false;
    UnkSpitHeld_b64a4* held = self->held;
    if ((held ? held->other : nullptr) == other)
        return false;
    bool result = fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), other, a2, a3, a4, a5);
    fn_71006993e0(self, other, a4);
    return result;
}
