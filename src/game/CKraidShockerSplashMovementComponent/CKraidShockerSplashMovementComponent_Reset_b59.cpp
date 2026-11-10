// CKraidShockerSplashMovementComponent's unit (tools/tu_map.py): resets the owner's (+0x30) collision
// component (+0x2a0, fn_7100861148), hands a given object to fn_7100ac82c0, sets the id
// g_unk_710164a2a8 on the owner's timeline (+0x2e8) to the all-flags mask (as
// src/game/CKraidAIComponent/fn_7100600c98.cpp) and clears +0x4b8 and +0x4c0. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkStruct_7100861148;
void fn_7100861148(UnkStruct_7100861148* self);

struct UnkSplashKey_b59a4;
extern UnkSplashKey_b59a4 g_unk_710164a2a8;  // reached through the GOT

struct UnkSplashOwner_b59a4 {
    u8 pad_0x0[0x2a0];
    UnkStruct_7100861148* field_0x2a0;
    u8 pad_0x2a8[0x2e8 - 0x2a8];
    UnkTimelineComponent* field_0x2e8;
};

struct UnkSplash_b59a4 {
    u8 pad_0x0[0x30];
    UnkSplashOwner_b59a4* owner;
    u8 pad_0x38[0x4b8 - 0x38];
    u64 field_0x4b8;
    u32 field_0x4c0;
};

s32 fn_7100ac82c0(UnkSplash_b59a4* self, void* other);

void fn_710060768c(UnkSplash_b59a4* self, void* other) {
    fn_7100861148(self->owner->field_0x2a0);
    if (other)
        fn_7100ac82c0(self, other);
    if (UnkTimelineComponent* timeline = self->owner->field_0x2e8) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a2a8), &mask.id, false, -1.0f);
    }
    self->field_0x4b8 = 0;
    self->field_0x4c0 = 0;
}
