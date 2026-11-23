// CBeamBoxComponent's unit (tools/tu_map.py unit 1355): the update. Until it has started, it either asks
// the owner's track holder (+0x220) to play the named track and hooks a listener up to it, or moves the
// owner between two points by a clamped progress; then, while a count or one of three timeline ids is set,
// it runs a timer that clears them. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern const u64 g_unk_710164afa0;
extern const u64 g_unk_710164afc0;
extern const u64 g_unk_710164afc8;
extern const u64 g_unk_710164afd0;

struct UnkBeamBox_l8a733c;
void fn_71008ac55c();
void fn_71008a6d40(UnkBeamBox_l8a733c* self, bool flag);

struct UnkBeamBoxOwner_l8a733c : UnkActor {
    u8 pad_0x380[0x30];
    bool field_0x3b0;  // the transform changed
};

struct UnkVec3_l8a733c {
    f32 x, y, z;
};

struct UnkBeamBox_l8a733c {
    u8 pad_0x0[0x30];
    UnkBeamBoxOwner_l8a733c* owner;  // 0x30
    u8 pad_0x38[0x3e8 - 0x38];
    u8 link[0x8];                      // 0x3e8
    UnkBeamBox_l8a733c* link_owner;    // 0x3f0
    void (*link_callback)();           // 0x3f8
    UnkWeakHandle* handle;             // 0x400
    u8 pad_0x408[0x428 - 0x408];
    f32 progress;  // 0x428
    f32 timer;     // 0x42c
    f32 limit;     // 0x430
    s32 count;     // 0x434
    bool done;     // 0x438
    bool started;  // 0x439
    bool idle;     // 0x43a
    u8 pad_0x43b[0x448 - 0x43b];
    UnkVec3_l8a733c from;  // 0x448
    UnkVec3_l8a733c to;    // 0x454
    u8 pad_0x460[0x478 - 0x460];
    base::global::UnkStrIdBase track;  // 0x478
};
static_assert(__builtin_offsetof(UnkBeamBox_l8a733c, track) == 0x478, "layout");

static inline f32 Clamp01_l8a733c(f32 v) {
    return !(v <= 0.0f) ? (!(v >= 1.0f) ? v : 1.0f) : 0.0f;
}

static inline UnkVec3_l8a733c Lerp_l8a733c(const UnkVec3_l8a733c& a, const UnkVec3_l8a733c& b, f32 t) {
    f32 s = 1.0f - t;
    UnkVec3_l8a733c r;
    r.x = s * a.x + t * b.x;
    r.y = s * a.y + t * b.y;
    r.z = s * a.z + t * b.z;
    return r;
}

static inline bool IsNear_l8a733c(f32 v, f32 target) {
    return v - 0.001f <= target && v + 0.001f >= target;
}

// Whether the timeline shows any of the three ids the timer clears.
static inline bool ShowsAny_l8a733c(UnkTimelineComponent* timeline) {
    u64 first = 0x4644f692882c4234;
    if (fn_7100a2be94(timeline, &first))
        return true;
    u64 second = 0xc02677ebfb1e00ab;
    if (fn_7100a2be94(timeline, &second))
        return true;
    u64 third = 0xa3f6ed53cf867060;
    return fn_7100a2be94(timeline, &third);
}

static inline bool HasId_l8a733c(UnkTimelineComponent* timeline, const u64& id) {
    return fn_7100a2be94(timeline, &id);
}

void fn_71008a733c(UnkBeamBox_l8a733c* self, f32 dt) {
    if (self->done)
        return;
    if (!self->started) {
        UnkBeamBoxOwner_l8a733c* owner = self->owner;
        UnkWeakTarget* holder;
        if (self->track.data != &g_unk_7101d47108->empty &&
            (holder = static_cast<UnkWeakTarget*>(owner->field_0x220))) {
            UnkSpitSend(holder, self->track.data->Key(), 0xf4061ff25a2163d6);
            self->started = true;
            fn_7100d7c828(self->owner, &g_unk_710164afa0, false);
            self->link_owner = self;
            self->link_callback = fn_71008ac55c;
            UnkWeakTarget* tracks = static_cast<UnkWeakTarget*>(self->owner->field_0x220);
            if (UnkWeakHandle* handle = self->handle) {
                if (UnkWeakTarget* target = handle->target)
                    fn_710117b3f0(target, self->link);
                ResetRef(&self->handle);
            }
            if (tracks) {
                fn_7100316018(tracks, &self->handle);
                fn_710117b3e8(self->handle ? self->handle->target : nullptr, self->link);
            }
        } else {
            f32 t = Clamp01_l8a733c(self->progress);
            UnkVec3_l8a733c pos;
            if (t >= 1.0f)
                pos = self->to;
            else
                pos = Lerp_l8a733c(self->from, self->to, t);
            owner->field_0x44 = pos.x;
            owner->field_0x54 = pos.y;
            owner->field_0x64 = pos.z;
            owner->field_0x3b0 = true;
            if (IsNear_l8a733c(t, 1.0f)) {
                self->started = true;
                fn_7100d7c94c(self->owner, &g_unk_710164afa0);
                fn_71008a6d40(self, true);
            }
            if (IsNear_l8a733c(self->progress, 0.0f))
                fn_7100d7c828(self->owner, &g_unk_710164afa0, false);
            self->progress += dt;
        }
    }
    if (self->idle)
        return;
    UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (self->count > 0 || ShowsAny_l8a733c(timeline)) {
        if (self->timer >= self->limit) {
            self->count = 0;
            if (timeline) {
                if (HasId_l8a733c(timeline, 0x4644f692882c4234))
                    fn_7100d7c828(self->owner, &g_unk_710164afc0, false);
                if (HasId_l8a733c(timeline, 0xc02677ebfb1e00ab))
                    fn_7100d7c828(self->owner, &g_unk_710164afc8, false);
                if (HasId_l8a733c(timeline, 0xa3f6ed53cf867060))
                    fn_7100d7c828(self->owner, &g_unk_710164afd0, false);
            }
            self->timer = 0.0f;
        } else {
            self->timer += dt;
        }
    }
}
