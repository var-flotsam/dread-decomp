// CCharClassChozoCommanderSentenceSphereMovementComponent's unit (tools/tu_map.py unit 669): runs the
// movement base's handler (fn_7100ac4ec0) and, when it succeeds and its first argument is the
// owner's (+0x30) target (fn_7100d7c990), calls fn_710045c5e8 with 1. Returns the base's result.
// Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "types.h"

// Signature as src/game/CEmmyWaveMovementComponent/fn_7100503068.cpp declares it.
struct UnkObj;
bool fn_7100ac4ec0(UnkObj* self, void* a1, void* a2, void* a3, void* a4, void* a5);

struct UnkSentenceSphereMovement_b46a1 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

void fn_710045c5e8(UnkSentenceSphereMovement_b46a1* self, u32 value);

bool fn_710045c8dc(UnkSentenceSphereMovement_b46a1* self, void* a1, void* a2, void* a3, void* a4, void* a5) {
    bool result = fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), a1, a2, a3, a4, a5);
    if (result) {
        if (fn_7100d7c990(self->owner) == a1)
            fn_710045c5e8(self, 1);
    }
    return result;
}
