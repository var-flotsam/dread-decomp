// behaviortree::CComputeTargetPositionInPathAction's unit (tools/tu_map.py, unit 474): runs the node. With a
// target and the AI's two path limits (fn_71010fc864, kind 2), runs the AI's path follower's side search
// (fn_710035e8d8) from the owner toward the target, then stores under the name at +0x30 (fn_7100395600) the
// limit the search point and the movement's path position both reach (within one unit), else the one the
// search's direction picks. Fails (6) without a target, limits or a path. Names and layouts are placeholders.
#include <functional>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkAIPathFollower;

// What a side search fills (0x14 bytes).
struct UnkAIPathSearchOut {
    u64 a;
    u64 b;
    f32 distance;

    UnkAIPathSearchOut() : a(0), b(0), distance(-1.0f) {}
};

bool fn_710035e8d8(UnkAIPathFollower* self, const CVector2D* from, const CVector2D* to, const CVector2D* dir_a,
                   const CVector2D* dir_b, bool flag, CVector2D* point, u32* out_0x7, f32 limit, bool* blocked,
                   f32* out_a, f32* out_b, u32* out_c, UnkAIPathSearchOut* out, std::function<void()> filter);
void fn_7100395600(UnkSelf* behavior, void* record, const CVector3D* value);

// The kind of limits asked for.
struct UnkPathLimitKind_l37e690 {
    u32 value;
};

// The movement's path state (at +0x8e8 of the movement, kind 1).
struct UnkPathState_l37e690 {
    u8 pad_0x0[0x89];
    u8 flag;        // 0x89
    u8 pad_0x8a[2];
    CVector2D pos;  // 0x8c
};

struct UnkPathMovement_l37e690 {
    u8 pad_0x0[0x58];
    CVector3D dir;  // 0x58
    u8 pad_0x64[0x8e8 - 0x64];
    UnkPathState_l37e690 state;  // 0x8e8
    u8 pad_0x97c[0xa0c - 0x97c];
    u8 reversed;  // 0xa0c
};
static_assert(__builtin_offsetof(UnkPathMovement_l37e690, reversed) == 0xa0c, "layout");

struct UnkPathAI_l37e690 {
    u8 pad_0x0[0x68];
    u8 follower[0x30];  // 0x68, the path follower
};

struct UnkPathOwner_l37e690 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1f0 - 0x78];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkPathAI_l37e690* ai;  // 0x240
};

struct UnkComputeTargetPosition_l37e690 {
    u8 pad_0x0[0x18];
    UnkPathOwner_l37e690* owner;  // 0x18
    u8 pad_0x20[0x10];
    u8 target[0x8];  // 0x30, the blackboard name the position goes under
};

static inline CVector3D Translation(const UnkMatrix44& m) {
    CVector3D v;
    v.x = m.m[3];
    v.y = m.m[7];
    v.z = m.m[11];
    return v;
}

static inline CVector3D Translation(const UnkActor* actor) {
    CVector3D v;
    v.x = actor->field_0x44;
    v.y = actor->field_0x54;
    v.z = actor->field_0x64;
    return v;
}

// The third column of a transform: the direction it faces.
static inline CVector3D AxisZ(const UnkMatrix44& m) {
    CVector3D v;
    v.x = m.m[2];
    v.y = m.m[6];
    v.z = m.m[10];
    return v;
}

// Whether `p` lies within one unit of `c` on both axes.
static inline bool Near(const CVector2D& c, const CVector2D& p) {
    return c.x + 1.0f >= p.x && c.x - 1.0f <= p.x && c.y + 1.0f >= p.y && c.y - 1.0f <= p.y;
}

// Whether both the search's point and the path position lie within one unit of `v`.
static inline bool Reaches(const CVector2D& point, const UnkPathState_l37e690* state, const CVector3D& v) {
    CVector2D p;
    fn_710007b430(&p, &v);
    if (!Near(point, p))
        return false;
    CVector2D q;
    fn_710007b430(&q, &v);
    return Near(state->pos, q);
}

static inline UnkActor* Target(UnkPathOwner_l37e690* owner) {
    return fn_7100d7c990(reinterpret_cast<UnkActor*>(owner));
}

s32 fn_710037e690(UnkComputeTargetPosition_l37e690* self) {
    UnkPathOwner_l37e690* owner = self->owner;
    UnkPathAI_l37e690* ai = owner ? owner->ai : nullptr;
    UnkActor* target = Target(owner);
    CVector3D result = Translation(self->owner->transform);
    if (!target)
        return 6;
    CVector3D low = {0.0f, 0.0f, 0.0f};
    CVector3D high = {0.0f, 0.0f, 0.0f};
    bool found;
    {
        UnkPathLimitKind_l37e690 kind;
        kind.value = 2;
        found = fn_71010fc864(reinterpret_cast<UnkBigFistAI3_ovb0050*>(ai),
                              reinterpret_cast<UnkBigFistKind_ovb0050*>(&kind),
                              reinterpret_cast<UnkBigFistLimit_ovb0050*>(&low),
                              reinterpret_cast<UnkBigFistLimit_ovb0050*>(&high));
    }
    if (!found)
        return 6;
    UnkReflObject* mv = self->owner->movement;
    UnkReflObject* movement_obj = mv && fn_710009d50c(mv->vf_0x0(), fn_71010c74e0()) ? mv : nullptr;
    UnkPathMovement_l37e690* movement = reinterpret_cast<UnkPathMovement_l37e690*>(movement_obj);
    if (fn_71010cc3a4(movement) != 1)
        return 6;
    UnkPathState_l37e690* state = &movement->state;

    bool flag = (movement->reversed != 0) != (state->flag != 0);
    CVector2D point = {0.0f, 0.0f};
    bool forward;
    {
        // The side search from the owner toward the target, along the path's direction.
        bool blocked;
        s32 edge_from = -1;
        s32 edge_to = -1;
        f32 distance = -1.0f;
        UnkAIPathSearchOut out;
        UnkAIPathFollower* follower = reinterpret_cast<UnkAIPathFollower*>(ai->follower);
        CVector2D from;
        CVector3D from_pos = Translation(self->owner->transform);
        fn_710007b430(&from, &from_pos);
        CVector2D to;
        CVector3D target_pos = Translation(Target(self->owner));
        fn_710007b430(&to, &target_pos);
        CVector2D dir;
        fn_710007b430(&dir, &movement->dir);
        CVector2D facing;
        CVector3D axis = AxisZ(fn_710002614c(&self->owner->transform));
        fn_710007b430(&facing, &axis);
        fn_710035e8d8(follower, &from, &to, &dir, &facing, flag, &point, reinterpret_cast<u32*>(&forward), 0.0f,
                      &blocked, reinterpret_cast<f32*>(&edge_from), reinterpret_cast<f32*>(&edge_to),
                      reinterpret_cast<u32*>(&distance), &out, std::function<void()>());
    }

    const CVector3D* chosen;
    if (Reaches(point, state, high))
        chosen = &high;
    else if (Reaches(point, state, low))
        chosen = &low;
    else
        chosen = forward ? &high : &low;
    result = *chosen;
    fn_7100395600(reinterpret_cast<UnkSelf*>(self), self->target, &result);
    return 5;
}
