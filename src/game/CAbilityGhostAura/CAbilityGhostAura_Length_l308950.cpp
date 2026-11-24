// CAbilityGhostAura unit: the length of the translation of the motion that the timeline reports over its whole
// duration (fn_7100308950), and what the ability measures from its owner (fn_7100308d0c): that length for one
// animation, two named colliders' values and half widths, and (once each) the heights of six animations'
// motions, kept in globals. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkMotionRule.h"
#include "game/UnkStruct_71007142e8.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTimeline.h"
#include "types.h"

struct UnkMotionTracks;

f32 CAbilityGhostAura_ComputeTimelineTranslationLength(UnkTimeline* timeline) {
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    f32 x = motion.xform.m[0][3];
    f32 y = motion.xform.m[1][3];
    f32 z = motion.xform.m[2][3];
    return sqrtf(x * x + y * y + z * z);
}

f32 CCharacterMovement_GetNegatedShapeBoundsMinY(void*, char* owner);

// A collider as this function reads it: its bounds, refreshed through vf_0x98 while `dirty` is set.
struct UnkGhostAuraCollider_l308d0c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();

    u8 pad_0x8[0x38];
    f32 min_x;  // 0x40
    u8 pad_0x44[4];
    f32 max_x;  // 0x48
    u8 pad_0x4c[0x14];
    bool dirty;  // 0x60
};
static_assert(__builtin_offsetof(UnkGhostAuraCollider_l308d0c, dirty) == 0x60, "UnkGhostAuraCollider_l308d0c layout");

struct UnkGhostAuraOwnerComponents_l308d0c {
    u8 pad_0x0[0x1f8];
    void* field_0x1f8;
    u8 pad_0x200[0x20];
    UnkStruct_7101176ea8* animations;  // 0x220
    u8 pad_0x228[0x78];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkGhostAuraOwner_l308d0c {
    u8 pad_0x0[0x30];
    UnkGhostAuraOwnerComponents_l308d0c* components;
};

struct UnkGhostAura_l308d0c {
    u8 pad_0x0[0xc0];
    UnkGhostAuraOwner_l308d0c* owner;
    u8 pad_0xc8[0x480 - 0xc8];
    f32 field_0x480;
    u8 pad_0x484[4];
    f32 field_0x488;
    f32 field_0x48c;
    f32 half_width;  // 0x490
};

__attribute__((visibility("hidden"))) extern base::global::CStrId g_unk_71726af4a8;
__attribute__((visibility("hidden"))) extern base::global::CStrId g_unk_71726af4b0;

// The heights of six animations' motions, measured once (below zero until then).
extern f32 g_unk_7101a22a80;
extern f32 g_unk_7101a22a84;

// The height the motion of the timeline gains over its whole duration.
static inline f32 Height(UnkTimeline* timeline) {
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    f32 frames = timeline->frame_count;
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags(2);
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, frames);
    return fabsf(motion.xform.m[2][3]);
}

// Measures the height of animation `name` into *height unless it was measured already.
static inline void MeasureOnce(UnkStruct_7101176ea8* animations, f32* height, u64 name) {
    if (animations != nullptr && *height < -0.001f) {
        UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176d34(animations, reinterpret_cast<void*>(name)));
        if (timeline != nullptr)
            *height = Height(timeline);
    }
}

// Half the collider's width plus one.
static inline f32 HalfWidth(UnkGhostAuraCollider_l308d0c* collider) {
    if (collider->dirty)
        collider->vf_0x98();
    return (collider->max_x - collider->min_x) * 0.5f + 1.0f;
}

void fn_7100308d0c(UnkGhostAura_l308d0c* self) {
    self->field_0x480 = 0.0f;
    if (self->owner == nullptr)
        return;
    UnkGhostAuraOwnerComponents_l308d0c* components = self->owner->components;
    if (components != nullptr) {
        if (components->animations != nullptr) {
            void* timeline = fn_7101176e0c(components->animations, 0x30d6af40e0eb8a80ULL, ~0ULL);
            if (timeline != nullptr)
                self->field_0x480 = CAbilityGhostAura_ComputeTimelineTranslationLength(static_cast<UnkTimeline*>(timeline));
        }
        void* source = components->field_0x1f8;
        UnkCollisionContainer* collision = components->collision;
        if (source != nullptr && collision != nullptr) {
            UnkGhostAuraCollider_l308d0c* a =
                reinterpret_cast<UnkGhostAuraCollider_l308d0c*>(fn_71008640a4(collision, &g_unk_71726af4a8));
            if (a != nullptr) {
                self->field_0x488 = CCharacterMovement_GetNegatedShapeBoundsMinY(source, reinterpret_cast<char*>(a));
                self->half_width = HalfWidth(a);
            }
            UnkGhostAuraCollider_l308d0c* b =
                reinterpret_cast<UnkGhostAuraCollider_l308d0c*>(fn_71008640a4(collision, &g_unk_71726af4b0));
            if (b != nullptr) {
                self->field_0x48c = CCharacterMovement_GetNegatedShapeBoundsMinY(source, reinterpret_cast<char*>(b));
                f32 width = HalfWidth(b);
                self->half_width = self->half_width > width ? self->half_width : width;
            }
        }
    }

    if (self->owner == nullptr)
        return;
    MeasureOnce(self->owner->components->animations, &g_unk_7101a22a70, 0xd6efbfc3a147a7fcULL);
    if (self->owner == nullptr)
        return;
    MeasureOnce(self->owner->components->animations, &g_unk_7101a22a74, 0x1f08a38dc8378c2dULL);
    if (self->owner == nullptr)
        return;
    MeasureOnce(self->owner->components->animations, &g_unk_7101a22a78, 0xa6530f738afab46cULL);
    if (self->owner == nullptr)
        return;
    MeasureOnce(self->owner->components->animations, &g_unk_7101a22a7c, 0x9072e6e052108693ULL);
    if (self->owner == nullptr)
        return;
    MeasureOnce(self->owner->components->animations, &g_unk_7101a22a80, 0xe73df1db26120f44ULL);
    if (self->owner == nullptr)
        return;
    MeasureOnce(self->owner->components->animations, &g_unk_7101a22a84, 0x3f1324e59550c8cbULL);
}
