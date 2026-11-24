// CChozoWarriorDeflectorShieldAttack's unit (tools/tu_map.py, unit 729): the shield's height from its
// animation. Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/UnkMotionRule.h"
#include "game/UnkStruct_71007142e8.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTimeline.h"
#include "types.h"

// fn_7100494578: the shield's height (+0x4c): cleared, then, when the owner's actions (+0x220) have the
// timeline 0x04dc4c0c7b23af17 (fn_7101176e0c), the absolute z translation of the motion over the whole
// timeline (fn_7100b8b3bc), as fn_7100425840
// (src/game/CChozoCommanderAirChargeAttack/CChozoCommanderAirChargeAttack_Charge_ovb0113.cpp) does.
struct UnkStruct_7101176ea8;

struct UnkMotionTracks;

struct UnkShieldOwner_ovb0130 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* actions;  // 0x220
};

struct UnkShieldAttack_ovb0130 {
    u8 pad_0x0[8];
    UnkShieldOwner_ovb0130* owner;
    u8 pad_0x10[0x4c - 0x10];
    f32 height;  // 0x4c
};

void fn_7100494578(UnkShieldAttack_ovb0130* self) {
    self->height = 0.0f;
    UnkStruct_7101176ea8* actions = self->owner->actions;
    if (!actions)
        return;
    UnkTimeline* timeline =
        static_cast<UnkTimeline*>(fn_7101176e0c(actions, 0x04dc4c0c7b23af17ull, 0x137e114c0de4fb8dull));
    if (!timeline)
        return;
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    self->height = __builtin_fabsf(motion.xform.m[2][3]);
}
