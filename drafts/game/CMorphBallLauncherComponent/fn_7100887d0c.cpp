// CMorphBallLauncherComponent's unit (tools/tu_map.py, unit 1343): the per-frame update. While the launch
// timer runs, the launched body (and, when the flag at +0x300 is set, the nearest target's) is eased from its
// start to above the launcher; then a delay timer fires a timeline event, a blend timer drives a track value
// through a named curve, and when the last timer runs out the state machine is asked for state 4.
// Names and layouts are placeholders.
#include "../../../src/game/UnkSmRequest_A5_b73.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU53.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkMorphBallLauncher_ovb0123;
UnkReflObject* CMorphBallLauncherComponent_FindNearestEntity(UnkMorphBallLauncher_ovb0123* self);
f32 fn_71000768a0(f32 f0);
__attribute__((visibility("hidden"))) extern const u64 g_unk_710164aea8;

struct UnkLauncherXform_l887d0c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

// Something that can be moved to a position (vtable slot 0xd0).
struct UnkLauncherMovable_l887d0c {
    void** vtable;
    void SetPosition(const CVector3D* pos, bool flag) {
        reinterpret_cast<void (*)(UnkLauncherMovable_l887d0c*, const CVector3D*, bool)>(vtable[0xd0 / 8])(
            this, pos, flag);
    }
};

struct UnkLauncherBody_l887d0c {
    u8 pad_0x0[0x10];
    UnkLauncherMovable_l887d0c* movable;  // 0x10
};

struct UnkLauncherCurve_l887d0c {
    u8 pad_0x0[0x10];
    const char* name;  // 0x10
    u8 pad_0x18[0x2c];
    s32 reads;  // 0x44
};

struct UnkLauncherSettings_l887d0c {
    u8 pad_0x0[0x5c];
    f32 launch_time;  // 0x5c
    f32 blend_time;   // 0x60
    f32 blend_from;   // 0x64
    f32 blend_to;     // 0x68
    u8 pad_0x6c[0x4];
    UnkLauncherCurve_l887d0c* curve;  // 0x70
};

struct UnkLauncherTrack_l887d0c {
    u8 pad_0x0[0x164];
    f32 value;  // 0x164
    bool set;   // 0x168
};

struct UnkMorphBallLauncher_l887d0c {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1d0];
    UnkSmMachine_A5_b73 machine;  // 0x208
    u8 pad_0x2b8[0x48];
    bool follow_target;  // 0x300
    u8 pad_0x301[0x7];
    UnkLauncherSettings_l887d0c* settings;  // 0x308
    UnkLauncherBody_l887d0c* body;          // 0x310
    UnkWeakHandle* target;                  // 0x318
    f32 launch_timer;                       // 0x320
    f32 state_timer;                        // 0x324
    f32 blend_timer;                        // 0x328
    f32 event_timer;                        // 0x32c
    CVector3D body_start;                   // 0x330
    CVector3D target_start;                 // 0x33c
};
static_assert(__builtin_offsetof(UnkMorphBallLauncher_l887d0c, machine) == 0x208, "");
static_assert(__builtin_offsetof(UnkMorphBallLauncher_l887d0c, follow_target) == 0x300, "");
static_assert(__builtin_offsetof(UnkMorphBallLauncher_l887d0c, target_start) == 0x33c, "");

static inline CVector3D Pos(const UnkLauncherXform_l887d0c* t) {
    return {t->x, t->y, t->z};
}

static inline const char* OrEmpty_l887d0c(const char* s) {
    return s ? s : "";
}

// Above the owner, at the height of `start`, eased in from `start` by `t`.
static inline CVector3D LaunchPoint(const UnkActor* owner, const CVector3D& start, f32 t) {
    const CVector3D up = {0.0f, 0.0f, 1.0f};
    const CVector3D base = {owner->field_0x44, owner->field_0x54, 0.0f};
    CVector3D end;
    end.x = base.x + up.x * start.z;
    end.y = base.y + up.y * start.z;
    end.z = base.z + up.z * start.z;
    if (!(t >= 1.0f)) {
        f32 s = 1.0f - t;
        CVector3D from = {s * start.x, s * start.y, s * start.z};
        CVector3D to = {t * end.x, t * end.y, t * end.z};
        end.x = to.x + from.x;
        end.y = to.y + from.y;
        end.z = to.z + from.z;
    }
    return end;
}

void fn_7100887d0c(UnkMorphBallLauncher_l887d0c* self, f32 dt) {
    if (self->launch_timer > 0.0f) {
        self->launch_timer -= dt;
        f32 t = fn_71000768a0(1.0f - self->launch_timer / self->settings->launch_time);
        CVector3D pos = LaunchPoint(self->owner, self->body_start, t);
        self->body->movable->SetPosition(&pos, true);
        if (self->follow_target) {
            UnkWeakHandle* target = self->target;
            if (target == nullptr || target->target == nullptr) {
                UnkReflObject* nearest = CMorphBallLauncherComponent_FindNearestEntity(reinterpret_cast<UnkMorphBallLauncher_ovb0123*>(self));
                const UnkLauncherXform_l887d0c* xform = reinterpret_cast<const UnkLauncherXform_l887d0c*>(nearest);
                self->target_start = Pos(xform);
                target = fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(nearest));
                AssignRef(&self->target, target);
            }
            CVector3D target_pos = LaunchPoint(self->owner, self->target_start, t);
            reinterpret_cast<UnkLauncherMovable_l887d0c*>(target->target)->SetPosition(&target_pos, true);
        }
    }

    f32 event_timer = self->event_timer;
    if (event_timer > 0.0f) {
        self->event_timer = event_timer -= dt;
        if (event_timer <= 0.0f) {
            fn_7100d7c828(self->owner, &g_unk_710164aea8, false);
            event_timer = self->event_timer;
        }
    }
    if (event_timer <= 0.0f && self->blend_timer >= 0.0f) {
        self->blend_timer -= dt;
        UnkLauncherSettings_l887d0c* settings = self->settings;
        f32 ratio = 1.0f - self->blend_timer / settings->blend_time;
        ++settings->curve->reads;
        f32 eased = fn_7100076020(ratio, OrEmpty_l887d0c(settings->curve->name));
        f32 from = self->settings->blend_from;
        f32 to = self->settings->blend_to;
        f32 v = eased <= 0.0f ? 0.0f : (eased > 1.0f ? 1.0f : eased);
        f32 value = from + v * (to - from);
        UnkLauncherTrack_l887d0c* track = reinterpret_cast<UnkLauncherTrack_l887d0c*>(
            CAnimationComponent_FindTrackByNameHash(self->owner->field_0x220, 0x71b15e29dc542a11));
        track->value = value;
        track->set = true;
    }

    self->state_timer -= dt;
    if (self->state_timer <= 0.0f)
        RequestState_A5_b73<15, 5>(&self->machine, 4);
}
