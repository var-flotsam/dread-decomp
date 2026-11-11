// CCharClassMushroomPlatformComponent's unit (tools/tu_map.py): when the owner (+0x30) has its
// sub-object at +0x220 and fn_710117ae38 says no for a fixed hash, sends it that hash with mode 0
// (as UnkSpitSend, src/game_s2/UnkSpitSend.h). fn_710088bbbc (same unit) does the same with
// another hash, then gives the owner's weak target (fn_71002b3b84) the id 0xa2f2cc13b58b67b4
// (fn_71009c0f1c) and copies the word at +0x46c to +0x468. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU372.h"
#include "types.h"

struct UnkWeakTarget;

struct UnkMushroomOwner_b54 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkMushroomPlatform_b54 {
    u8 pad_0x0[0x30];
    union {
        UnkMushroomOwner_b54* owner;
        UnkWeakTarget* weak_owner;
    };
    u8 pad_0x38[0x468 - 0x38];
    u32 field_0x468;
    u32 field_0x46c;
};

static inline void SendUnlessPlaying_b54(UnkMushroomPlatform_b54* self, u64 hash) {
    if (UnkMushroomOwner_b54* owner = self->owner) {
        if (UnkAnimationComponent* target = owner->field_0x220) {
            if (!fn_710117ae38(target, hash, false))
                UnkSpitSend(target, hash, 0xf4061ff25a2163d6);
        }
    }
}

void fn_710088bf64(UnkMushroomPlatform_b54* self) { SendUnlessPlaying_b54(self, 0xe2ea001660c8ac66); }

void fn_710088bbbc(UnkMushroomPlatform_b54* self) {
    SendUnlessPlaying_b54(self, 0xb12cbc38a8207d70);
    if (void* target = fn_71002b3b84(self->weak_owner))
        fn_71009c0f1c(target, 0xa2f2cc13b58b67b4);
    self->field_0x468 = self->field_0x46c;
}
