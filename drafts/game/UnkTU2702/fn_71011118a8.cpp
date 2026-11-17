// UnkTU2702 (CAIComponent's unit): while the owner walks (movement state 1) and no step plan is set, plans
// one: the distance left (to the navigation goal by the side search, or to the movement's tracked point,
// whichever is nearer) over the step length gives a whole number of steps and the stretch to apply.
// Names and layouts are placeholders.
#include <float.h>
#include <functional>
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// From src/game/CAIComponent.cpp (the same definitions).
struct UnkAIPathFollower {
    u8 pad_0x0[0x8];
    u32 count;
    u8 pad_0xc[0x20];
    u8 flag;

    bool IsReady() const { return count != (u32)((count != 0) & !flag); }
};

struct UnkAIPathSearchOut {
    u64 a;
    u64 b;
    f32 distance;

    UnkAIPathSearchOut() : a(0), b(0), distance(-1.0f) {}
};

bool fn_710035e8d8(UnkAIPathFollower* self, const CVector2D* from, const CVector2D* to, const CVector2D* dir_a,
                   const CVector2D* dir_b, bool flag, CVector2D* point, u32* out_0x7, f32 limit, bool* blocked,
                   f32* out_a, f32* out_b, u32* out_c, UnkAIPathSearchOut* out, std::function<void()> filter);

struct UnkEntityCollided;
UnkVec3f fn_7100d7c7a0(const UnkEntityCollided* self);

struct UnkNavRecord_l1118a8 {
    u8 pad_0x0[0x10];
    s32 kind;
};

struct UnkNavList_l1118a8 {
    u8 pad_0x0[0x10];
    UnkNavRecord_l1118a8* first;
};

struct UnkNavRecords_l1118a8 {
    UnkNavList_l1118a8* list;
    void* field_0x8;

    bool HasPath() const { return list && list->first && list->first->kind == 1 && field_0x8; }
};

// The owner's navigation (+0x298): its records and its goal.
struct UnkNavigation_l1118a8 {
    u8 pad_0x0[0x40];
    CVector3D goal;
    u8 pad_0x4c[0x64];
    UnkNavRecords_l1118a8* records;  // 0xb0
};

// What the owner's movement (+0x218) tracks: a point at +0x4 (copied with fn_71002b58c0).
struct UnkTracked_l1118a8 {
    u32 field_0x0;
    f32 x;
    f32 y;
    u8 pad_0xc[0xf4];
};

struct UnkMovement_l1118a8 {
    u8 pad_0x0[0x58];
    CVector3D field_0x58;
    u8 pad_0x64[0x90d];
    bool field_0x971;
    u8 pad_0x972[0x2];
    f32 x;  // 0x974
    f32 y;  // 0x978
    u8 pad_0x97c[0x90];
    bool field_0xa0c;
    u8 pad_0xa0d[0x1b];
    u32 tracking;  // 0xa28
    UnkTracked_l1118a8 tracked;  // 0xa2c
};
static_assert(__builtin_offsetof(UnkMovement_l1118a8, field_0xa0c) == 0xa0c, "layout");
static_assert(__builtin_offsetof(UnkMovement_l1118a8, tracked) == 0xa2c, "layout");

struct UnkStepParams_l1118a8 {
    u8 pad_0x0[0x41a];
    bool enabled;
    u8 pad_0x41b;
    f32 min_steps;  // 0x41c
    f32 max_steps;  // 0x420
    f32 min_ratio;  // 0x424
    f32 max_ratio;  // 0x428
};

struct UnkStepPlan_l1118a8 {
    s32 count;
    f32 ratio;
    f32 x;
    f32 y;
};

struct UnkStepAI_l1118a8 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x30];
    UnkAIPathFollower follower;  // 0x68
    u8 pad_0x98[0x1578];
    UnkStepParams_l1118a8* params;  // 0x1610
    u8 pad_0x1618[0x12ee8];
    UnkStepPlan_l1118a8 steps;  // 0x14500
};
static_assert(__builtin_offsetof(UnkStepAI_l1118a8, params) == 0x1610, "layout");
static_assert(__builtin_offsetof(UnkStepAI_l1118a8, steps) == 0x14500, "layout");

f32 fn_71010ff140(UnkStepAI_l1118a8* self);

static inline f32 Length(f32 x, f32 y, f32 z) {
    return sqrtf(x * x + y * y + z * z);
}

void fn_71011118a8(UnkStepAI_l1118a8* self) {
    UnkStepParams_l1118a8* params = self->params;
    if (!params || !params->enabled)
        return;
    UnkMovement_l1118a8* movement = static_cast<UnkMovement_l1118a8*>(self->owner->field_0x218);
    if (!movement)
        return;
    f32 min_steps = params->min_steps;
    f32 max_steps = params->max_steps;
    f32 min_ratio = params->min_ratio;
    f32 max_ratio = params->max_ratio;
    if (fn_71010cc3a4(movement) != 1)
        return;
    UnkStepPlan_l1118a8& steps = self->steps;
    if (!(steps.ratio < 0.0f))
        return;

    f32 distance = FLT_MAX;
    UnkActor* owner = self->owner;
    if (UnkNavigation_l1118a8* nav = static_cast<UnkNavigation_l1118a8*>(owner->field_0x298)) {
        if (nav->records->HasPath()) {
            CVector2D from;
            fn_710007b430(&from, reinterpret_cast<const CVector3D*>(&static_cast<const UnkVec3f&>(
                                     fn_7100d7c7a0(reinterpret_cast<const UnkEntityCollided*>(owner)))));
            CVector2D point = {0.0f, 0.0f};
            s32 first = -1;
            s32 second = -1;
            f32 left = 0.0f;
            bool mirrored = (movement->field_0xa0c != 0) ^ (movement->field_0x971 != 0);
            bool blocked = false;
            CVector3D goal = nav->goal;
            CVector2D to;
            fn_710007b430(&to, &goal);
            CVector2D dir_a;
            fn_710007b430(&dir_a, &movement->field_0x58);
            const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
            CVector2D dir_b;
            CVector3D forward = {m.m[2], m.m[6], m.m[10]};
            fn_710007b430(&dir_b, &forward);
            UnkAIPathSearchOut out;
            fn_710035e8d8(&self->follower, &from, &to, &dir_a, &dir_b, mirrored, &point,
                          reinterpret_cast<u32*>(&mirrored), 1.7446744e19f, &blocked, reinterpret_cast<f32*>(&first),
                          reinterpret_cast<f32*>(&second), reinterpret_cast<u32*>(&left), &out,
                          std::function<void()>());
            distance = left;
        }
    }
    if (movement->tracking) {
        UnkTracked_l1118a8 tracked;
        fn_71002b58c0(reinterpret_cast<UnkObject_0x100*>(&tracked),
                      reinterpret_cast<const UnkObject_0x100*>(&movement->tracked));
        steps.x = tracked.x;
        steps.y = tracked.y;
        f32 d = Length(tracked.x - movement->x, tracked.y - movement->y, 0.0f);
        distance = d < distance ? d : distance;
        fn_71002b6094(reinterpret_cast<UnkObject_0x100*>(&tracked));
    }

    if (distance + 0.1f < FLT_MAX || distance - 0.1f > FLT_MAX) {
        f32 t = distance / fn_71010ff140(self);
        if (t >= min_steps && t <= max_steps) {
            s32 up = (s32)ceilf(t);
            s32 down = (s32)floorf(t);
            f32 up_ratio = t / up;
            s32 count = down;
            f32 ratio = 1.0f;
            f32 error = FLT_MAX;
            if (down >= 1) {
                ratio = t / down;
                error = fabsf(ratio - 1.0f);
            }
            if (fabsf(1.0f - up_ratio) < error) {
                count = up;
                ratio = up_ratio;
            }
            steps.count = count;
            steps.ratio = ratio;
            if (!(ratio >= min_ratio && ratio <= max_ratio)) {
                steps.count = 0;
                steps.ratio = -1.0f;
            }
        }
    } else if (steps.count > 0) {
        --steps.count;
    }
}
