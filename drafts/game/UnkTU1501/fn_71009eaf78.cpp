// CSamusMovement's unit (unit 1501): where an animation's motion takes the actor, and whether the way
// there is clear (half way up, then half way across, then the end). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CAnimationComponent.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkStruct_7101176ea8;

struct UnkStruct_71007142e8 {
    u8 pad_0x0[0x2c];
    f32 field_0x2c;
    u8 pad_0x30[0x40];
};

struct UnkMotionTracks;
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

void fn_7101179d08(UnkAnimationComponent* self, UnkStruct_71007142e8* motion, const void* key);

// A sampled motion: a transform first, then whether it holds anything (+0x5c).
struct UnkSampledMotion_l9eaf78 {
    UnkStruct_71007142e8 motion;

    UnkSampledMotion_l9eaf78() { fn_71007142e8(&motion); }
    const UnkMatrix44* Matrix() const { return reinterpret_cast<const UnkMatrix44*>(&motion); }
    u32 Count() const { return reinterpret_cast<const u32*>(&motion)[0x5c / 4]; }
};

static inline void Sample(UnkTimeline* timeline, UnkSampledMotion_l9eaf78* motion) {
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
}

static inline UnkMatrix44 operator*(const UnkMatrix44& a, const UnkMatrix44& b) { return fn_71000127c8(a, b); }

struct UnkSamusMovement_l9eaf78 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // +0x30
};

bool fn_71009eaf78(UnkSamusMovement_l9eaf78* self, const u64* key, void* arg, u32 value, const UnkVec3f* from,
                   UnkVec3f* out) {
    UnkActor* actor = self->actor;
    if (!actor)
        return false;
    UnkStruct_7101176ea8* animation = static_cast<UnkStruct_7101176ea8*>(actor->field_0x220);
    if (!animation)
        return false;
    UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176d34(animation, reinterpret_cast<void*>(*key)));
    if (!timeline)
        return false;

    UnkSampledMotion_l9eaf78 motion;
    Sample(timeline, &motion);
    if (motion.Count() == 0)
        return false;

    UnkSampledMotion_l9eaf78 current;
    fn_7101179d08(static_cast<UnkAnimationComponent*>(self->actor->field_0x220), &current.motion,
                  &g_unk_717270ec68);
    UnkXform end(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38)) *
                 fn_710002614c(current.Matrix()) * fn_710002614c(motion.Matrix()));
    UnkVec3f there = end.GetTranslation();
    f32 dx = there.x - actor->field_0x44;
    f32 dy = there.y - actor->field_0x54;
    f32 dz = there.z - actor->field_0x64;

    UnkGrappleTarget* target = reinterpret_cast<UnkGrappleTarget*>(self);
    UnkVec3f up(from->x + 0.0f, from->y + dy * 0.5f, from->z + 0.0f);
    UnkVec3f across(from->x + dx * 0.5f, from->y + dy, from->z + 0.0f);
    if (fn_7100976c94(target, &up, value, arg, true, 0.1f))
        return false;
    if (fn_7100976c94(target, &across, value, arg, true, 0.1f))
        return false;
    UnkVec3f end_(from->x + dx, from->y + dy, from->z + dz);
    if (fn_7100976c94(target, &end_, value, arg, true, 0.1f))
        return false;
    if (out)
        *out = there;
    return true;
}
