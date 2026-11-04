// CFulmiteBellyMineAIComponent's unit (tools/tu_map.py): when the object at +0x146d0 has its flag
// +0x54c set and the owner's (+0x30) sub-object at +0x220 exists, sends it `hash` with mode 0 under the
// kind 0xf4061ff25a2163d6 (as UnkSpitSend, src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkBellyMineFlags_A3_b64 {
    u8 pad_0x0[0x54c];
    bool field_0x54c;
};

struct UnkBellyMineOwner_A3_b64 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkBellyMineAI_A3_b64 {
    u8 pad_0x0[0x30];
    UnkBellyMineOwner_A3_b64* owner;
    u8 pad_0x38[0x146d0 - 0x38];
    UnkBellyMineFlags_A3_b64* field_0x146d0;
};

void fn_710057ec5c(UnkBellyMineAI_A3_b64* self, u64 hash) {
    if (!self->field_0x146d0->field_0x54c)
        return;
    if (void* target = self->owner->field_0x220)
        UnkSpitSend(target, hash, 0xf4061ff25a2163d6);
}
