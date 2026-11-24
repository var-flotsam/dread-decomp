// CBaseBigFistAIComponent's unit (tools/tu_map.py unit 591): after the base's step (fn_71003ac9ec), sets
// the reach (+0x1499c) to the owner's radius and adds how far sideways the two attack motions the
// component names (its vf_0x840 and vf_0x848) carry it, measured upright. Names and layouts are
// placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CBossAIComponent.h"
#include "game/UnkMat4f.h"
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU54.h"
#include "game/UnkTimeline.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

extern __attribute__((visibility("hidden"))) const UnkVec3f g_unk_7101649350;

struct UnkMotionTracks;

// The motion fn_71007142e8 initialises: a 3x4 matrix and what follows it (0x70 bytes).
struct UnkBigFistMotion_l3e5a70 {
    f32 m[3][4];
    u8 pad_0x30[0x40];

    UnkVec3f Translation() const { return UnkVec3f(m[0][3], m[1][3], m[2][3]); }
};

struct UnkBigFistOwner_l3e5a70 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;  // 0x220
    u8 pad_0x228[0x37c - 0x228];
    f32 radius;  // 0x37c
};

struct UnkBigFistAI_l3e5a70 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkBigFistOwner_l3e5a70* owner;  // 0x30
    u8 pad_0x38[0x1499c - 0x38];
    f32 reach;  // 0x1499c

    // The names of the two attack motions.
    const CStrId* vf_0x840() {
        return reinterpret_cast<const CStrId* (*)(UnkBigFistAI_l3e5a70*)>(vtable[0x840 / 8])(this);
    }
    const CStrId* vf_0x848() {
        return reinterpret_cast<const CStrId* (*)(UnkBigFistAI_l3e5a70*)>(vtable[0x848 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkBigFistAI_l3e5a70, reach) == 0x1499c, "layout");

// As the other users of the base's step declare it (src/game/CQuetzoaAIComponent/).
struct UnkQuetzoaAI_l64a058;

// The derived Goliath component calls this as its base (CGoliathAIComponent_Reach_l590cc0.cpp).
struct UnkGoliathAI_l590cc0;

// The rotation from g_unk_7101649350 to straight up.
static inline UnkMat4f UpRotation_l3e5a70() {
    UnkVec3f up(0.0f, 1.0f, 0.0f);
    return fn_7100079c60(&g_unk_7101649350, &up);
}

// The motion over the whole timeline, every channel.
static inline void Sample_l3e5a70(UnkTimeline* timeline, UnkBigFistMotion_l3e5a70* motion) {
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
}

void CBaseBigFistAIComponent_ComputeSidewaysAnimationReach(UnkGoliathAI_l590cc0* component) {
    auto* self = reinterpret_cast<UnkBigFistAI_l3e5a70*>(component);
    fn_71003ac9ec(reinterpret_cast<UnkQuetzoaAI_l64a058*>(self));
    UnkMat4f rotation = UpRotation_l3e5a70();
    fn_7100077bbc(&rotation);
    if (!self->owner->animation)
        return;
    self->reach = self->owner->radius;
    // Adds how far sideways each of the two motions carries the owner, turned upright.
    for (u32 i = 0; i < 2; ++i) {
        const CStrId* name = i == 0 ? self->vf_0x840() : self->vf_0x848();
        u64 key = name->data->Key();
        if (key == ~0ull)
            continue;
        UnkTimeline* timeline =
            static_cast<UnkTimeline*>(fn_7101176e0c(self->owner->animation, key, 0x09f60a5ab4b640caull));
        if (!timeline)
            continue;
        UnkBigFistMotion_l3e5a70 motion;
        fn_71007142e8(reinterpret_cast<UnkStruct_71007142e8*>(&motion));
        Sample_l3e5a70(timeline, &motion);
        UnkVec3f offset = motion.Translation();
        UnkVec3f moved = TransformPointByMatrixWithPerspectiveDivide(&rotation, &offset);
        self->reach += fabsf(moved.x);
    }
}
