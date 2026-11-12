// CChozoRobotSoldierBeamMovementComponent's unit (tools/tu_map.py): sets "onlaunch_charged" or
// "onlaunch_notcharged" (by the flag at +0x5d8) on the owner's (+0x30) timeline (+0x2e8) to the all-flags mask;
// when charged, stores the definition's (+0x5d0) +0x134 / +0xd4 at +0x7c; runs fn_7100ac3880; shows the owner's
// (+0x280) parts "tail" (not charged) and "tail_charged" (charged); registers itself with the game manager under
// g_unk_71726cd560. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/CFXComponent.h"
#include "game/CProjectileMovement.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkSelf;
struct UnkWorld;
extern u64 g_unk_71726cd560 __attribute__((visibility("hidden")));

struct UnkRobotBeamOwner_b62a2 {
    u8 pad_0x0[0x280];
    UnkWorld* field_0x280;
    u8 pad_0x288[0x2e8 - 0x288];
    void* field_0x2e8;
};

struct UnkRobotBeamDef_b62a2 {
    u8 pad_0x0[0xd4];
    f32 field_0xd4;
    u8 pad_0xd8[0x134 - 0xd8];
    f32 field_0x134;
};

struct UnkRobotBeam_b62a2 {
    u8 pad_0x0[0x30];
    UnkRobotBeamOwner_b62a2* owner;
    u8 pad_0x38[0x7c - 0x38];
    f32 field_0x7c;
    u8 pad_0x80[0x5d0 - 0x80];
    UnkRobotBeamDef_b62a2* field_0x5d0;
    u8 charged;
};

__attribute__((always_inline)) static inline void Set_b62a2(UnkTimelineComponent* timeline, const char* name) {
    base::reflection::UnkReflTypeIdArg mask;
    base::global::CName id(name);
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &id.hash, &mask.id, false, -1.0f);
}

void fn_71004791d0(UnkRobotBeam_b62a2* self) {
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        if (self->charged)
            Set_b62a2(timeline, "onlaunch_charged");
        else
            Set_b62a2(timeline, "onlaunch_notcharged");
    }
    if (self->charged)
        self->field_0x7c = self->field_0x5d0->field_0x134 / self->field_0x5d0->field_0xd4;
    fn_7100ac3880(reinterpret_cast<UnkSelf*>(self));
    fn_710114fe8c(self->owner->field_0x280, base::global::CName("tail").hash, !self->charged, 0);
    fn_710114fe8c(self->owner->field_0x280, base::global::CName("tail_charged").hash, self->charged, 0);
    fn_7100de9540(g_unk_7101d47310, &g_unk_71726cd560, reinterpret_cast<UnkWeakTarget*>(self));
}
