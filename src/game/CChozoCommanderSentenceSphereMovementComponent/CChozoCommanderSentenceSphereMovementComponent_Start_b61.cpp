// CChozoCommanderSentenceSphereMovementComponent's unit (tools/tu_map.py): after fn_7100ac3880, clears +0x5d8, marks
// the owner's (+0x30) object at +0x1b8 (+0x64), resets the one at +0x2a0 (fn_71008618cc), copies +0x13c of the
// definition at +0x5d0 to +0x5dc, runs fn_710045b8e0, clears +0x5e4 and +0x5e8, sets g_unk_7101649730 on the owner's
// timeline (+0x2e8) to the all-flags mask and registers itself with the game manager under g_unk_71726ca0e8.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkSelf;
void fn_7100ac3880(UnkSelf* self);  // as src/game/CChozoCommanderEnergyShardsSphereMovementComponent/fn_7100459e6c.cpp
struct UnkStruct_71008618cc;
void fn_71008618cc(void* self);  // as src/game/CEmmyWaveMovementComponent/fn_7100502628.cpp declares it
struct UnkSentenceSphere_b61a2;
void fn_710045b8e0(UnkSentenceSphere_b61a2* self);
extern u64 g_unk_71726ca0e8 __attribute__((visibility("hidden")));
struct UnkSentenceSphereKey_b61a2;
extern UnkSentenceSphereKey_b61a2 g_unk_7101649730;  // reached through the GOT

struct UnkSentenceSphereMark_b61a2 {
    u8 pad_0x0[0x64];
    u8 field_0x64;
};

struct UnkSentenceSphereOwner_b61a2 {
    u8 pad_0x0[0x1b8];
    UnkSentenceSphereMark_b61a2* field_0x1b8;
    u8 pad_0x1c0[0xe0];
    UnkStruct_71008618cc* field_0x2a0;
    u8 pad_0x2a8[0x40];
    void* field_0x2e8;
};

struct UnkSentenceSphereDef_b61a2 {
    u8 pad_0x0[0x13c];
    u32 field_0x13c;
};

struct UnkSentenceSphere_b61a2 {
    u8 pad_0x0[0x30];
    UnkSentenceSphereOwner_b61a2* owner;
    u8 pad_0x38[0x598];
    UnkSentenceSphereDef_b61a2* field_0x5d0;
    u32 field_0x5d8;
    u32 field_0x5dc;
    u8 pad_0x5e0[0x4];
    u32 field_0x5e4;
    bool field_0x5e8;
};

void fn_710045b780(UnkSentenceSphere_b61a2* self) {
    fn_7100ac3880(reinterpret_cast<UnkSelf*>(self));
    self->field_0x5d8 = 0;
    if (UnkSentenceSphereMark_b61a2* mark = self->owner->field_0x1b8)
        reinterpret_cast<u8*>(mark)[0x64] = 1;
    if (UnkStruct_71008618cc* collision = self->owner->field_0x2a0)
        fn_71008618cc(collision);
    self->field_0x5dc = self->field_0x5d0->field_0x13c;
    fn_710045b8e0(self);
    self->field_0x5e4 = 0;
    self->field_0x5e8 = false;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649730), &mask.id, false, -1.0f);
    }
    fn_7100de9540(g_unk_7101d47310, &g_unk_71726ca0e8, reinterpret_cast<UnkWeakTarget*>(self));
}
