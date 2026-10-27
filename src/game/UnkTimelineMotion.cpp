// From unit 1728 of tools/tu_map.py (0x7100b892d4-0x7100b8bb94, no reflected class): an action's
// timeline (the layout src/game/CCutscene.cpp reads: its frame count at +0x24 and its events at
// +0x170) and the motion it applies between two times. The motion tracks each map a span of time
// onto a span of their own and give a transform for it; the transforms are chained, their
// translations scaled by the displacement and compute-delta events playing at the time, and the
// result goes to fn_7100714700 with the number of frames between the two times. Names and layouts
// are placeholders.
// Holds fn_7100b8b3bc.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "types.h"

// A timeline event (base::global::timeline; as src/game/CCutscene.cpp's): its class, then the
// frames it spans.
struct UnkTimelineEvent {
    void** vtable;
    const base::reflection::CClass* cls;
    f32 start;
    f32 end;
};
struct UnkTimelineEventList {
    UnkTimelineEvent** data;
    u32 count;
};

// The event of `cls` playing at `frame`, or null (as src/game/CCutscene.cpp declares it).
UnkTimelineEvent* fn_710006bb80(f32 frame, UnkTimelineEventList* events, const base::reflection::CClass* cls,
                                s32 frame_count);

// A displacement event (CCharClassTweakDisplacementTrack): the translation's scale.
struct UnkDisplacementEvent : UnkTimelineEvent {
    f32 scale;
};
// A compute-delta event (CCharClassTweakComputeDeltaTrack): a scale per axis.
struct UnkComputeDeltaEvent : UnkTimelineEvent {
    f32 x;
    f32 y;
    f32 z;
};

// A time key: the frame at `time`, and the time one frame lasts from there.
struct UnkTimeKey {
    f32 frame;
    f32 time;
    f32 frame_time;
};
struct UnkTimeKeys {
    UnkTimeKey* data;
    u32 count;
};
// The key at `index` (out of line).
UnkTimeKey* fn_7100b8ad14(UnkTimeKeys* self, u32 index);

struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    f32 last_frame;  // the frame at frame_count
    u8 pad_0x2c[0x8];
    u32 flags;  // bit 0: has a first motion, bit 1: has a second
    u8 pad_0x38[0x20];
    UnkTimeKeys keys;
    u8 pad_0x68[0x108];
    UnkTimelineEventList events;
};
static_assert(__builtin_offsetof(UnkTimeline, keys) == 0x58, "UnkTimeline placeholder layout");
static_assert(__builtin_offsetof(UnkTimeline, events) == 0x170, "UnkTimeline placeholder layout");

// A motion source: the transform for a span of its own time.
struct UnkMotionSource {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual bool vf_0x38(f32 from, f32 to, UnkMatrix44* out, bool first, bool second);
};

// A motion track: the span of the timeline it covers, the span of its source's time it maps it
// onto (from `base`, `length` long) and the source.
struct UnkMotionTrack {
    f32 start;
    f32 end;
    f32 base;
    f32 length;
    u8 pad_0x10[0x8];
    UnkMotionSource* source;
    u8 pad_0x20[0x8];
};
static_assert(sizeof(UnkMotionTrack) == 0x28, "UnkMotionTrack placeholder layout");

struct UnkMotionTracks {
    u8 pad_0x0[0x8];
    UnkMotionTrack* data;
    u32 count;
};

// Which motions were applied (bit 1: the first, bit 2: the second). It is passed by value with a
// non-trivial copy, so in memory.
struct UnkMotionMode {
    u32 bits;

    explicit UnkMotionMode(u32 b) : bits(b) {}
    UnkMotionMode(const UnkMotionMode& other) : bits(other.bits) {}
};

void fn_7100714700(f32 frames, void* target, const UnkMatrix44* motion, UnkMotionMode mode);

static inline f32 Clamp(f32 x, f32 lo, f32 hi) {
    return x <= lo ? lo : (x >= hi ? hi : x);
}

// The frame at time `t`.
static inline f32 FrameAt(UnkTimeline* self, f32 t) {
    if (t == 0.0f)
        return 0.0f;
    if ((f32)self->frame_count == t)
        return self->last_frame;
    if (self->keys.count == 1)
        return t / fn_7100b8ad14(&self->keys, 0)->frame_time;
    // The first key after `t`; the one before it holds `t`.
    UnkTimeKey* it = self->keys.data;
    UnkTimeKey* last = self->keys.data + self->keys.count;
    u64 n = ((u64)last - (u64)it) / sizeof(UnkTimeKey);
    while (n != 0) {
        u64 half = n >> 1;
        UnkTimeKey* mid = it + half;
        if (mid->time <= t) {
            it = mid + 1;
            n = n - half - 1;
        } else {
            n = half;
        }
    }
    return it[-1].frame + (t - it[-1].time) / it[-1].frame_time;
}

// Applies the motion of the tracks between times `from` and `to` (the motions `*flags` asks for
// and the timeline has) to `target`.
// NON_MATCHING(minor): in a track's time mapping the original multiplies length * ratio where ours
// has ratio * length (commuted, same result) and schedules and allocates the subtractions and
// clamps differently; the key search runs on the count itself with branches where the original
// divides the keys' byte size by 12 and selects with csel (the same search: advance while the key's
// time <= t). Everything else is identical.
void fn_7100b8b3bc(UnkTimeline* self, void* target, UnkMotionTracks* tracks, const u32* flags, f32 from,
                   f32 to) {
    u32 f = *flags;
    u32 m = self->flags;
    bool first = (f & 2) && (m & 1);
    bool second = (f & 4) && (m & 2);
    if (!first && !second)
        return;
    UnkMatrix44 motion = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    UnkMatrix44 step;
    bool any = false;
    f32 t = from;
    UnkMotionTrack* end = tracks->data + tracks->count;
    for (UnkMotionTrack* track = tracks->data; track != end; ++track) {
        if (!(t >= track->start && t < track->end))
            continue;
        UnkMotionSource* source = track->source;
        if (source &&
            source->vf_0x38(Clamp(track->base + track->length * ((t - track->start) / (track->end - track->start)),
                                  track->base, track->base + track->length),
                            Clamp(track->base + track->length * ((to - track->start) / (track->end - track->start)),
                                  track->base, track->base + track->length),
                            &step, first, second)) {
            if (*flags & 2) {
                bool scaled = false;
                f32 k = 1.0f;
                if (UnkDisplacementEvent* d = static_cast<UnkDisplacementEvent*>(
                        fn_710006bb80(t, &self->events, fn_7100d2f3c8(), self->frame_count))) {
                    k = d->scale;
                    scaled = true;
                }
                f32 x = 1.0f, y = 1.0f, z = 1.0f;
                if (UnkComputeDeltaEvent* c = static_cast<UnkComputeDeltaEvent*>(
                        fn_710006bb80(t, &self->events, fn_7100d2d958(), self->frame_count))) {
                    x = c->x;
                    y = c->y;
                    z = c->z;
                    scaled = true;
                }
                if (scaled) {
                    step.m[3] = k * x * step.m[3];
                    step.m[7] = k * y * step.m[7];
                    step.m[11] = k * z * step.m[11];
                }
            }
            UnkCopyMatrix(&motion, fn_71000127c8(motion, step));
            any = true;
        }
        t = track->end;
        if (!(t < to))
            break;
    }
    UnkMotionMode mode(first && any ? 2 : 0);
    if (second && any)
        mode.bits |= 4;
    f32 to_frame = FrameAt(self, to);
    f32 from_frame = FrameAt(self, from);
    fn_7100714700(to_frame - from_frame, target, &motion, mode);
}
