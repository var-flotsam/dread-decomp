// CChozoRobotSoldierUppercutAttack's unit (tools/tu_map.py unit 687): the length of the uppercut's motion: zero, then,
// when the owner's (+0x8) actions (+0x220) have the uppercut timeline (fn_7101176e0c), the length of the translation
// of the motion fn_71007142e8 sets up as that timeline reports it over its whole duration (fn_7100b8b3bc), as
// src/game/CChozoCommanderAirChargeAttack/CChozoCommanderAirChargeAttack_Charge_ovb0113.cpp builds the end
// offset. Names and layouts are placeholders.
#include <math.h>

#include "game/CAnimationComponent.h"
#include "game/UnkMotionRule.h"
#include "game/UnkStruct_71007142e8.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTimeline.h"
#include "types.h"

struct UnkStruct_7101176ea8;

struct UnkMotionTracks;

struct UnkUppercutOwner_ovb0245 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* actions;  // 0x220
};

struct UnkUppercutLength_ovb0245 {
    u8 pad_0x0[8];
    UnkUppercutOwner_ovb0245* owner;
    u8 pad_0x10[0x2c];
    f32 length;  // 0x3c
};

void fn_710046a228(UnkUppercutLength_ovb0245* self) {
    self->length = 0.0f;
    UnkStruct_7101176ea8* actions = self->owner->actions;
    if (!actions)
        return;
    UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176e0c(actions, 0x98c6fdc80e86d316ull, ~0ull));
    if (!timeline)
        return;
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    f32 x = motion.xform.m[0][3];
    f32 y = motion.xform.m[1][3];
    f32 z = motion.xform.m[2][3];
    self->length = sqrtf(x * x + y * y + z * z);
}
