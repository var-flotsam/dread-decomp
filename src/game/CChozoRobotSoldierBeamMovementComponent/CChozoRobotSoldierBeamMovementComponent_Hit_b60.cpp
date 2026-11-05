// CChozoRobotSoldierBeamMovementComponent's unit (tools/tu_map.py): the first time the base test
// (fn_7100ac4c60, arguments passed on) holds, raises +0x5da, clears +0x4b8 and +0x4c0 and, when the owner
// (+0x30) has a timeline (+0x2e8), sets one of two fixed ids (by the flag at +0x5d8) to the all-flags
// mask. Always true. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkBossComponent;
bool fn_7100ac4c60(UnkBossComponent* self, u64 a, u64 b, u64 c, u64 d);

struct UnkSoldierBeam_b60a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x4b8 - 0x38];
    u64 field_0x4b8;
    s32 field_0x4c0;
    u8 pad_0x4c4[0x5d8 - 0x4c4];
    bool field_0x5d8;
    u8 pad_0x5d9;
    bool field_0x5da;
};

bool fn_71004795cc(UnkSoldierBeam_b60a2* self, u64 a, u64 b, u64 c, u64 d) {
    if (self->field_0x5da || !fn_7100ac4c60(reinterpret_cast<UnkBossComponent*>(self), a, b, c, d))
        return true;
    self->field_0x5da = true;
    self->field_0x4b8 = 0;
    self->field_0x4c0 = 0;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        u64 off = 0x2b5ea1fd8a588e4aull;
        u64 on = 0xd3ff1fc021c49baaull;
        const u64* key = self->field_0x5d8 ? &on : &off;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, key, &mask.id, false, -1.0f);
    }
    return true;
}

// The same without the base test.
bool fn_7100479480(UnkSoldierBeam_b60a2* self) {
    if (self->field_0x5da)
        return true;
    self->field_0x5da = true;
    self->field_0x4b8 = 0;
    self->field_0x4c0 = 0;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        u64 off = 0x2b5ea1fd8a588e4aull;
        u64 on = 0xd3ff1fc021c49baaull;
        const u64* key = self->field_0x5d8 ? &on : &off;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, key, &mask.id, false, -1.0f);
    }
    return true;
}
