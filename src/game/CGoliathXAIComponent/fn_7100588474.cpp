// CGoliathXAIComponent's unit (tools/tu_map.py unit 838): after the base update (fn_7100590e68), records the
// height of one animation's motion (+0x14cc0) and, while the boss mode flag is set, stops the owner's layers 12,
// 6 and 11 and sends it "relax"; then updates the bombs attack at +0x14b60. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAnimationComponent.h"
#include "game/CBossAIComponent.h"
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "types.h"

struct UnkActorTarget;
struct UnkStruct_7101176ea8;

struct UnkGoliathAI5_ovb0106;
void CGoliathAIComponent_WriteAnimSpeedMultiplierToTunable(UnkGoliathAI5_ovb0106* self);
struct UnkGoliathXBombsAttack_l5858c4;
void CGoliathXBurstProjectionBombsAttack_CacheTimelineMotionValues(UnkGoliathXBombsAttack_l5858c4* self);

struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

struct UnkMotionTracks;

// The motion parts to sample, as UnkFlags_71007142e8 (game/UnkMotionRule.h) holds them but left unset until
// assigned: that type's default constructor zeroes, which the original does not do here.
struct UnkMotionParts_l588474 {
    u32 bits;
};
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

// What this unit reads of the boss mode flags: whether the boss is calm (+0xb8).
struct UnkGoliathModeFlags_l588474 {
    u8 pad_0x0[0xb8];
    bool calm;
};

struct UnkGoliathXOwner_l588474 {
    u8 pad_0x0[0x220];
    void* field_0x220;  // the animations
};

struct UnkGoliathXAI_l588474 {
    u8 pad_0x0[0x30];
    UnkGoliathXOwner_l588474* owner;  // +0x30
    u8 pad_0x38[0x14b60 - 0x38];
    u8 bombs[0x14cc0 - 0x14b60];  // +0x14b60, the bombs attack
    f32 motion_height;            // +0x14cc0
};

void fn_7100588474(UnkGoliathXAI_l588474* self) {
    CGoliathAIComponent_WriteAnimSpeedMultiplierToTunable(reinterpret_cast<UnkGoliathAI5_ovb0106*>(self));
    self->motion_height = 0.0f;
    if (UnkStruct_7101176ea8* animations = static_cast<UnkStruct_7101176ea8*>(self->owner->field_0x220)) {
        UnkTimeline* timeline =
            static_cast<UnkTimeline*>(fn_7101176d34(animations, reinterpret_cast<void*>(0xbe901ff6f5cba1f7ull)));
        UnkMotionParts_l588474 parts;  // all of them, set below
        UnkStruct_71007142e8 motion;
        if (timeline) {
            fn_71007142e8(&motion);
            UnkMotionTracks* tracks = timeline->tracks;
            parts.bits = UnkFlagsAll_71007142e8().bits;
            fn_7100b8b3bc(timeline, &motion, tracks, &parts.bits, 0.0f, timeline->frame_count);
            self->motion_height = motion.m[11];
        }
        if (reinterpret_cast<UnkGoliathModeFlags_l588474*>(g_unk_71726bc570)->calm) {
            UnkActorTarget* target = static_cast<UnkActorTarget*>(self->owner->field_0x220);
            fn_7101178e08(target, 12, false, false, -1.0f);
            fn_7101178e08(target, 6, false, false, -1.0f);
            fn_710117662c(target, 11, 0, false, false, -1.0f);
            u64 hash = StringCrc64("relax", 5);
            base::reflection::UnkReflTypeIdArg value;
            value.id = 0x82;
            base::reflection::UnkReflTypeIdArg arg;
            arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
            CAnimationComponent_SendActionToTrack(target, hash, 0xf4061ff25a2163d6, &value, &g_unk_710164b958, &g_unk_7172769740, &arg);
        }
    }
    CGoliathXBurstProjectionBombsAttack_CacheTimelineMotionValues(reinterpret_cast<UnkGoliathXBombsAttack_l5858c4*>(self->bombs));
}
