// CGoliathAIComponent's unit (tools/tu_map.py unit 844): after fn_71003e5a70, measures how far the
// animation picked by the id at +0x149b8 moves sideways (the motion's translation turned by the
// rotation from g_unk_710164a030 to straight up) and adds it to the reach at +0x14a28. Names and
// layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkMat4f.h"
#include "game/UnkMotionRule.h"
#include "game/UnkStruct_71007142e8.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU54.h"
#include "game/UnkTimeline.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

extern __attribute__((visibility("hidden"))) const UnkVec3f g_unk_710164a030;

// As src/game/CSamusFloorSlideMovementState/CParams/CParams_Slide_ovb0004.cpp defines them.
struct UnkMotionTracks;

struct UnkGoliathMotion_l590cc0 {
    UnkStruct_71007142e8 motion;

    // The translation column of the motion's 3x4 matrix.
    UnkVec3f Translation() const {
        const f32* m = reinterpret_cast<const f32*>(this);
        return UnkVec3f(m[3], m[7], m[11]);
    }
};

struct UnkGoliathOwner_l590cc0 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;  // 0x220
};

struct UnkGoliathAI_l590cc0 {
    u8 pad_0x0[0x30];
    UnkGoliathOwner_l590cc0* owner;  // 0x30
    u8 pad_0x38[0x14980];
    u8 field_0x149b8[0x70];
    f32 reach;  // 0x14a28
};
static_assert(__builtin_offsetof(UnkGoliathAI_l590cc0, reach) == 0x14a28, "layout");

void CBaseBigFistAIComponent_ComputeSidewaysAnimationReach(UnkGoliathAI_l590cc0* self);
const CStrId* fn_710058e660(void* self);

// The rotation from g_unk_710164a030 to straight up.
static inline UnkMat4f UpRotation() {
    UnkVec3f up(0.0f, 1.0f, 0.0f);
    return fn_7100079c60(&g_unk_710164a030, &up);
}

// The motion over the whole timeline, every channel.
static inline void Sample(UnkTimeline* timeline, UnkGoliathMotion_l590cc0* motion) {
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
}

void fn_7100590cc0(UnkGoliathAI_l590cc0* self) {
    CBaseBigFistAIComponent_ComputeSidewaysAnimationReach(self);
    self->reach = 0.0f;
    UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176e0c(
        self->owner->animation, fn_710058e660(self->field_0x149b8)->data->Key(), 0x6d7373f5fd58146aull));
    if (!timeline)
        return;
    UnkMat4f rotation = UpRotation();
    fn_7100077bbc(&rotation);
    UnkGoliathMotion_l590cc0 motion;
    fn_71007142e8(&motion.motion);
    Sample(timeline, &motion);
    UnkVec3f offset = motion.Translation();
    UnkVec3f moved = TransformPointByMatrixWithPerspectiveDivide(&rotation, &offset);
    self->reach += fabsf(moved.x);
}
