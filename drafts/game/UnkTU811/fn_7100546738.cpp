// UnkTU811 (CEmmyAIComponent's unit): the distance from the owner's movement to where it is heading,
// written to `distance` (-1 when there is none): zero when the movement is in mode 4; in mode 1 the
// nearer of the path's end and the navigation target (when the target lies ahead within a narrow cone).
// Returns whether a distance was found. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/CEnemyMovement.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

bool fn_7100707c80(char* self);

struct UnkMovement_l546738 {
    u8 pad_0x0[0x974];
    CVector2D position;  // 0x974
    u8 pad_0x97c[0xa28 - 0x97c];
    u32 pathLength;  // 0xa28
    u8 pad_0xa2c[4];
    CVector2D pathEnd;  // 0xa30
};

// The two kinds of navigation target: each has a path query and the point it is heading to.
struct UnkNavTargetA_l546738 {
    u8 pad_0x0[0x140];
    char query[0x3810c - 0x140];  // 0x140
    CVector3D point;  // 0x3810c
};
struct UnkNavTargetB_l546738 {
    u8 pad_0x0[0x150];
    char query[0x3811c - 0x150];  // 0x150
    CVector3D point;  // 0x3811c
};

struct UnkNavInfo_l546738 {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
};
struct UnkNavRecord_l546738 {
    u8 pad_0x0[0x10];
    UnkNavInfo_l546738* info;  // 0x10
};
struct UnkNavTargets_l546738 {
    UnkNavRecord_l546738* record;  // 0x0
    UnkNavTargetA_l546738* targetA;  // 0x8
    u8 pad_0x10[0x28];
    UnkNavTargetB_l546738* targetB;  // 0x38
};
struct UnkNavigation_l546738 {
    u8 pad_0x0[0xb0];
    UnkNavTargets_l546738* targets;  // 0xb0
};

struct UnkOwner_l546738 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38 (the start of the transform)
    u8 pad_0x78[0x218 - 0x78];
    UnkMovement_l546738* movement;  // 0x218
    u8 pad_0x220[0x298 - 0x220];
    UnkNavigation_l546738* navigation;  // 0x298
};

struct UnkEmmyAI_l546738 {
    u8 pad_0x0[0x30];
    UnkOwner_l546738* owner;  // 0x30
};

static inline f32 Distance(const CVector2D& a, const CVector2D& b) {
    f32 dx = a.x - b.x;
    dx *= dx;
    f32 dy = a.y - b.y;
    dy *= dy;
    return sqrtf(dx + dy);
}

// The z axis of a transform matrix.
static inline CVector3D Forward(const UnkMatrix44& m) {
    CVector3D v;
    v.x = m.m[2];
    v.y = m.m[6];
    v.z = m.m[10];
    return v;
}

// The navigation's target of each kind, when its record says that kind.
static inline u32 TargetKind(UnkNavTargets_l546738* targets) {
    if (!targets->record || !targets->record->info)
        return 0;
    return targets->record->info->kind;
}

// The point the navigation is heading to, when its path query has one.
static inline const CVector3D* TargetPoint(UnkNavTargets_l546738* targets) {
    u32 kind = TargetKind(targets);
    if (kind == 1 && targets->targetA) {
        UnkNavTargetA_l546738* target = targets->targetA;
        return fn_7100707c80(target->query) ? &target->point : nullptr;
    }
    if (TargetKind(targets) == 7 && targets->targetB) {
        UnkNavTargetB_l546738* target = targets->targetB;
        return fn_7100707c80(target->query) ? &target->point : nullptr;
    }
    return nullptr;
}

// The nearer of the path's end and the navigation target, written to `distance` when there is one.
static inline void MeasureDistance(UnkEmmyAI_l546738* self, UnkMovement_l546738* movement, f32* distance) {
    CVector2D forward;
    {
        CVector3D forward3 = Forward(fn_710002614c(&self->owner->transform));
        fn_710007b430(&forward, &forward3);
    }

    f32 pathDistance = movement->pathLength ? Distance(movement->pathEnd, movement->position) : -1.0f;

    f32 targetDistance = -1.0f;
    if (const CVector3D* point = TargetPoint(self->owner->navigation->targets)) {
        CVector2D target;
        {
            CVector3D target3 = *point;
            fn_710007b430(&target, &target3);
        }
        const CVector2D& pos = movement->position;
        if (target.x + 0.1f >= pos.x && target.x - 0.1f <= pos.x && target.y + 0.1f >= pos.y &&
            target.y - 0.1f <= pos.y) {
            targetDistance = 0.0f;
        } else {
            f32 dx = target.x - pos.x;
            f32 dy = target.y - pos.y;
            f32 length = sqrtf(dx * dx + dy * dy);
            if (length != 0.0f) {
                dx /= length;
                dy /= length;
            }
            if (dx * forward.x + dy * forward.y > 0.99f)
                targetDistance = Distance(target, movement->position);
        }
    }

    if (pathDistance >= 0.0f && targetDistance >= 0.0f)
        *distance = pathDistance < targetDistance ? pathDistance : targetDistance;
    else if (pathDistance >= 0.0f)
        *distance = pathDistance;
    else if (targetDistance >= 0.0f)
        *distance = targetDistance;
}

bool fn_7100546738(UnkEmmyAI_l546738* self, f32* distance) {
    *distance = -1.0f;
    if (self->owner->movement) {
        if (fn_71010cc3a4(self->owner->movement) == 4) {
            *distance = 0.0f;
            return true;
        }
        UnkMovement_l546738* movement = self->owner->movement;
        if (movement && fn_71010cc3a4(movement) == 1)
            MeasureDistance(self, movement, distance);
    }
    return *distance >= 0.0f;
}
