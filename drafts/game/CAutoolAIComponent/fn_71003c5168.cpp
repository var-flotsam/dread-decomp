// CAutoolAIComponent's unit (tools/tu_map.py): the update. With a navigation (+0x218) and a path
// component (+0x298) it measures the distance to the path's goal, turns it into the approach factors
// and the speed of the tunable, and points the move direction by the facing; then the base update runs
// and a pending stop (+0x14772) whose time is under 1 ends (fn_71003c4e64). Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAutoolAIComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkXformNode.h"
#include "types.h"

// CCharClassAutoolAIComponent::CTunableCharClassAutoolAIComponent
extern base::reflection::UnkTunable g_unk_71726beb18;

struct UnkAutoolTunable_l3c5168 {
    u8 pad_0x0[0x30];
    f32 far_distance;  // 0x30
    f32 min_approach;  // 0x34
    f32 turn_speed;    // 0x38
    u8 pad_0x3c[4];
    f32 far_speed;   // 0x40
    f32 near_speed;  // 0x44
};

// A reached position: valid when the flag is set.
struct UnkAutoolReach_l3c5168 {
    u8 pad_0x0[0x10];
    f32 distance;  // 0x10
    bool valid;    // 0x14
};

struct UnkAutoolNav_l3c5168 {
    u8 pad_0x0[0x40];
    f32 facing;  // 0x40
    u8 pad_0x44[0x38];
    f32 slope;  // 0x7c
};

UnkAutoolReach_l3c5168* fn_71010cc3c4(UnkAutoolNav_l3c5168* nav);

// CAIGotoPointNavigationState::STarget: a goal that gives its position.
struct UnkAutoolGoal_l3c5168 {
    void** vtable;
    UnkVec3f GetPosition() {
        return reinterpret_cast<UnkVec3f (*)(UnkAutoolGoal_l3c5168*)>(vtable[0x18 / 8])(this);
    }
};

struct UnkAutoolGoalHolder_l3c5168 {
    u8 pad_0x0[0x120];
    base::reflection::UnkReflValue goal;  // 0x120
};

struct UnkAutoolPathState_l3c5168 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10: 1 a goal point, 2 a reach query
};

struct UnkAutoolPathNode_l3c5168 {
    u8 pad_0x0[0x10];
    UnkAutoolPathState_l3c5168* state;  // 0x10
};

struct UnkAutoolPath_l3c5168 {
    UnkAutoolPathNode_l3c5168* current;     // 0x0
    UnkAutoolGoalHolder_l3c5168* goal;      // 0x8
    void* reach;                            // 0x10
};

struct UnkAutoolPathComponent_l3c5168 {
    u8 pad_0x0[0xb0];
    UnkAutoolPath_l3c5168* path;  // 0xb0
};

struct UnkAutoolPose_l3c5168 {
    u8 pad_0x0[0x70];
    f32 heading;  // 0x70
    u8 pad_0x74[8];
    f32 speed;  // 0x7c
};

struct UnkAutoolOwner_l3c5168 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x188];
    UnkAutoolPose_l3c5168* pose;  // 0x1f0
    u8 pad_0x1f8[0x20];
    UnkAutoolNav_l3c5168* nav;  // 0x218
    u8 pad_0x220[0x78];
    UnkAutoolPathComponent_l3c5168* path_component;  // 0x298
};
static_assert(__builtin_offsetof(UnkAutoolOwner_l3c5168, nav) == 0x218, "layout");
static_assert(__builtin_offsetof(UnkAutoolOwner_l3c5168, path_component) == 0x298, "layout");

struct UnkAutoolDir_l3c5168 {
    f32 x, y, z;
};

struct UnkAutoolAI_l3c5168 {
    u8 pad_0x0[0x30];
    UnkAutoolOwner_l3c5168* owner;  // 0x30
    u8 pad_0x38[0x1473a];
    bool stop_pending;  // 0x14772
    u8 pad_0x14773;
    f32 stop_time;     // 0x14774
    f32 goal_distance;  // 0x14778
    f32 approach;       // 0x1477c
    f32 approach_min;   // 0x14780
    f32 speed;          // 0x14784
    UnkAutoolDir_l3c5168 dir;  // 0x14788
};
static_assert(__builtin_offsetof(UnkAutoolAI_l3c5168, stop_pending) == 0x14772, "layout");
static_assert(__builtin_offsetof(UnkAutoolAI_l3c5168, dir) == 0x14788, "layout");

void fn_71003c54ac(UnkAutoolAI_l3c5168* self);

// Whether the path's current node is in a state of that kind.
inline bool IsPathKind_l3c5168(UnkAutoolPath_l3c5168* path, s32 kind) {
    UnkAutoolPathState_l3c5168* state = path->current->state;
    return state && state->kind == kind;
}

// The goal of a path heading for a point.
inline UnkAutoolGoalHolder_l3c5168* GoalOf_l3c5168(UnkAutoolPath_l3c5168* path) {
    return IsPathKind_l3c5168(path, 1) ? path->goal : nullptr;
}

inline f32 Clamp_l3c5168(f32 v, f32 lo, f32 hi) {
    return v > lo ? (!(v >= hi) ? v : hi) : lo;
}

inline f32 Lerp_l3c5168(f32 a, f32 b, f32 t) {
    if (t >= 1.0f)
        return b;
    if (t < 0.0f)
        return a;
    return (1.0f - t) * a + t * b;
}

inline f32 Sign_l3c5168(f32 x) {
    return (f32)((x > 0.0f) - (x < 0.0f));
}

void fn_71003c5168(UnkAutoolAI_l3c5168* self, f32 dt) {
    UnkAutoolOwner_l3c5168* owner = self->owner;
    UnkAutoolNav_l3c5168* nav = owner->nav;
    UnkAutoolPathComponent_l3c5168* component = owner->path_component;
    if (nav && component) {
        self->goal_distance = 3.4028235e38f;
        UnkAutoolPath_l3c5168* path = component->path;
        if (path->current) {
            if (UnkAutoolGoalHolder_l3c5168* holder = GoalOf_l3c5168(path)) {
                base::reflection::UnkReflValue* value = &holder->goal;
                // The goal is an STarget (checked, the answer unused).
                fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(value->type), fn_71007019f0());
                UnkAutoolGoal_l3c5168* goal =
                    const_cast<UnkAutoolGoal_l3c5168*>(base::reflection::refl_ArgValue<UnkAutoolGoal_l3c5168>(value));
                UnkVec3f pos = goal->GetPosition();
                UnkAutoolOwner_l3c5168* me = self->owner;
                f32 dx = pos.x - me->x;
                f32 dy = pos.y - me->y;
                f32 dz = pos.z - me->z;
                self->goal_distance = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
            } else if (IsPathKind_l3c5168(path, 2) && path->reach) {
                UnkAutoolReach_l3c5168* reach = fn_71010cc3c4(nav);
                if (reach->valid)
                    self->goal_distance = reach->distance;
            }
        }
        f32 distance = self->goal_distance;
        const UnkAutoolTunable_l3c5168* tunable = reinterpret_cast<const UnkAutoolTunable_l3c5168*>(&g_unk_71726beb18);
        f32 approach = Clamp_l3c5168(distance / tunable->far_distance, 0.0f, 1.0f);
        self->approach = approach;
        self->approach_min = Clamp_l3c5168(approach, tunable->min_approach, 1.0f);
        f32 slope = Clamp_l3c5168((self->owner->pose->speed - tunable->min_approach) / (1.0f - tunable->min_approach),
                                  0.0f, 1.0f);
        self->speed = Lerp_l3c5168(tunable->far_speed, tunable->near_speed, approach);
        self->dir.x = Sign_l3c5168(self->owner->pose->heading);
        self->dir.y = 0.0f;
        self->dir.z = 0.0f;
        f32 facing = nav->facing;
        if (__builtin_fabsf(facing) > 0.1f && (nav->slope + 0.001f < 0.0f || nav->slope - 0.001f > 0.0f)) {
            self->dir.y = -(tunable->turn_speed * slope);
            self->dir.x = Sign_l3c5168(facing);
        }
        UnkAutoolDir_l3c5168& dir = self->dir;
        f32 inv = 1.0f / (__builtin_sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) + 0.0001f);
        dir.y = inv * dir.y;
        dir.x = dir.x * inv;
        dir.z = inv * dir.z;
    }
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    fn_71003c54ac(self);
    if (self->stop_pending && self->stop_time < 1.0f) {
        self->stop_pending = false;
        fn_71003c4e64(reinterpret_cast<UnkSelf*>(self), 1);
    }
}
