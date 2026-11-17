// Unit 1449: picks the motion that best covers the body's move: for a vertical move (up or down)
// or a level one (forward or back), plays each motion of the matching list and keeps the one whose
// reach, scaled to the move, is closest to it; hands out its id, the scale and whether it goes back.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CAnimationComponent.h"
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1450.h"
#include "game/UnkTU1728.h"
#include "game/UnkXformNode.h"
#include "types.h"

// The motion fn_71007142e8 initialises: a 3x4 transform first.
struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

// A list of motion ids (0x28 bytes).
struct UnkMotionIds_l97db3c {
    base::global::CStrId* data;
    u32 count;  // 0x8
    u8 pad_0xc[0x1c];
};
static_assert(sizeof(UnkMotionIds_l97db3c) == 0x28, "list");

// The motions for a vertical move (down, up) and for a level one (forward, back).
extern UnkMotionIds_l97db3c g_unk_717273ad58[2] __attribute__((visibility("hidden")));
extern UnkMotionIds_l97db3c g_unk_717273ada8[2] __attribute__((visibility("hidden")));

struct UnkOwner_l97db3c {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x220 - 0x78];
    UnkStruct_7101176ea8* actions;  // 0x220
};

UnkOwner_l97db3c* fn_710097f1a8(UnkUnit1452_b56* self);

struct UnkSelf_l97db3c {
    u8 pad_0x0[0x23c];
    base::math::CVector3D move;  // 0x23c
};

static inline bool IsAbout(f32 v, f32 target, f32 epsilon) {
    return v - epsilon <= target && v + epsilon >= target;
}

static inline f32 Dot(const base::math::CVector3D& a, const base::math::CVector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline base::math::CVector3D Normalized(const base::math::CVector3D& v) {
    f32 inv = 1.0f / (__builtin_sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);
    base::math::CVector3D r;
    r.x = v.x * inv;
    r.y = v.y * inv;
    r.z = inv * v.z;
    return r;
}

// `a / b`, or 0 when `b` is about 0.
static inline f32 Ratio(f32 a, f32 b) {
    return IsAbout(b, 0.0f, 0.001f) ? 0.0f : a / b;
}

// Plays the whole of the timeline's motion into `motion`.
static inline void Play(UnkTimeline* timeline, UnkStruct_71007142e8* motion, UnkMotionTracks* tracks,
                        UnkFlags_71007142e8 flags) {
    fn_7100b8b3bc(timeline, motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
}

bool fn_710097db3c(UnkSelf_l97db3c* self, base::global::CStrId* out_id, UnkVec3f* out_scale,
                   bool* out_back) {
    UnkUnit1452_b56* unit = reinterpret_cast<UnkUnit1452_b56*>(self);
    const base::math::CVector3D up = {0.0f, 1.0f, 0.0f};
    const base::math::CVector3D side = {1.0f, 0.0f, 0.0f};
    base::math::CVector3D dir = Normalized(self->move);

    UnkMotionIds_l97db3c* list;
    f32 speed;
    f32 along_x;
    f32 along_y;
    bool back;
    f32 rise = Dot(dir, up);
    if (IsAbout(__builtin_fabsf(rise), 1.0f, 0.001f)) {
        bool upward = IsAbout(rise, 1.0f, 0.001f);
        list = upward ? &g_unk_717273ad58[1] : &g_unk_717273ad58[0];
        speed = upward ? 0.95f : 0.85f;
        back = true;
        along_x = 0.0f;
        along_y = 1.0f;
    } else {
        if (!IsAbout(__builtin_fabsf(Dot(dir, side)), 1.0f, 0.001f))
            return false;
        const UnkMatrix44& m = fn_710002614c(&fn_710097f1a8(unit)->transform);
        back = dir.x * m.m[2] + dir.y * m.m[6] + dir.z * m.m[10] < 0.0f;
        list = back ? &g_unk_717273ada8[1] : &g_unk_717273ada8[0];
        f32 x = self->move.x;
        along_x = (f32)((x > 0.0f) - (x < 0.0f));
        along_y = 0.0f;
        speed = 0.9f;
    }

    UnkStruct_7101176ea8* actions = fn_710097f1a8(unit)->actions;
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    f32 best_error = 3.4028235e38f;
    const base::global::CStrId* best = nullptr;
    UnkVec3f best_scale(0.0f, 0.0f, 0.0f);
    const base::global::CStrId* id = list->data;
    do {
        fn_71007143c8(&motion, UnkFlagsAll_71007142e8());
        UnkTimeline* timeline =
            static_cast<UnkTimeline*>(fn_7101176d34(actions, reinterpret_cast<void*>(id->data->Key())));
        Play(timeline, &motion, timeline->tracks, UnkFlagsAll_71007142e8());
        // The motion's reach along the move.
        // The motion's reach along the move.
        base::math::CVector3D reach = {along_x * motion.m[11], along_y * motion.m[7], motion.m[3] * 0.0f};
        UnkVec3f scale(Ratio(speed * self->move.x, reach.x), Ratio(speed * self->move.y, reach.y),
                       Ratio(speed * self->move.z, reach.z));
        f32 error = __builtin_fabsf(1.0f - (scale.x * scale.x + scale.y * scale.y + scale.z * scale.z));
        if (!best || error < best_error) {
            best_error = error;
            best = id;
            best_scale = scale;
        }
    } while (++id != list->data + list->count);

    *out_id = *best;
    *out_scale = best_scale;
    *out_back = back;
    return true;
}
