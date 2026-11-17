// CObsydomithonAttack's unit (tools/tu_map.py): whether the attack can hit at `angle`. Needs a
// CObsydomithonAIComponent and a target; true when the target is in reach (fn_710062ea78) or, when the AI's
// area (+0x68) has edges, when the point of the area nearest the target lies more than 10 units from the
// owner and the angle is within the tunable arc (+0x34) around 90 (target above) or 270 (target below).
// Names and layouts are placeholders.
#include <functional>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

// As src/base/spatial/UnkPolygon2D.cpp defines them.
struct UnkPolygonVertex {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPolygon2D {
    UnkPolygonVertex* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;
    bool box_dirty;  // 0x2d: the box below is stale
    u8 unk_0x2e[0x2];
    CVector2D box_min;  // 0x30
    CVector2D box_max;  // 0x38
    u8 unk_0x40[0x8];
};
static_assert(sizeof(UnkPolygon2D) == 0x48, "UnkPolygon2D layout");

using UnkPointFilter = std::function<bool(const CVector2D&)>;
using UnkEdgeVisitor = std::function<void(u32, const UnkSegment2D&)>;

void fn_7100117ba0(const UnkPolygon2D* polygon, const CVector2D* point, s32* edge, f32* t, CVector2D* closest,
                   f32* distance, bool flag, UnkPointFilter filter, UnkEdgeVisitor visit);

// As src/game/behaviortree/CTargetIsInHightPositionCondition/CTargetIsInHightPositionCondition.cpp
// defines it.
struct UnkActorPosition {
    f32 x, y, z, w;
    UnkActorPosition(const UnkActorPosition& other);
};

struct UnkObsydomithonAI_b66a4;
bool fn_710062ea78(UnkObsydomithonAI_b66a4* self);

extern UnkTunable g_unk_71726f8988;

// The Obsydomithon tunable's value this file reads.
struct UnkObsydomithonTunable_l629600 {
    u8 pad_0x0[0x34];
    f32 arc;  // 0x34
};

struct UnkObsydomithonAI_l629600 {
    u8 pad_0x0[0x68];
    UnkPolygon2D area;  // 0x68
};

struct UnkObsydomithonAttack_l629600 {
    void** vtable;
    UnkActor* owner;  // 0x8
};

static inline u32 EdgeCount(const UnkPolygon2D* p) {
    return p->count - (!p->closed & (p->count != 0));
}

static inline bool IsObsydomithonAI(UnkReflObject* ai) {
    return ai && fn_710009d50c(ai->vf_0x0(), fn_710062c51c());
}

static inline UnkObsydomithonAI_l629600* AsObsydomithonAI(UnkActor* owner) {
    auto* ai = static_cast<UnkReflObject*>(owner->field_0x240);
    return reinterpret_cast<UnkObsydomithonAI_l629600*>(IsObsydomithonAI(ai) ? ai : nullptr);
}

static inline CVector3D Translation(const UnkActor* actor) {
    CVector3D v;
    v.x = actor->field_0x44;
    v.y = actor->field_0x54;
    v.z = actor->field_0x64;
    return v;
}

static inline bool InArc(f32 angle, f32 centre, f32 arc) {
    return centre - arc <= angle && arc + centre >= angle;
}

bool fn_7100629600(UnkObsydomithonAttack_l629600* self, f32 angle) {
    auto* ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710062c51c()))
        return false;
    if (!fn_7100d7c990(self->owner))
        return false;

    bool in_reach = fn_710062ea78(reinterpret_cast<UnkObsydomithonAI_b66a4*>(AsObsydomithonAI(self->owner)));
    UnkPolygon2D* area = &AsObsydomithonAI(self->owner)->area;
    if (in_reach)
        return true;

    if (EdgeCount(area) != 0) {
        s32 edge = -1;
        f32 t = -1.0f;
        CVector2D closest = {0.0f, 0.0f};
        f32 distance = -1.0f;
        {
            CVector2D target;
            CVector3D target_pos = Translation(fn_7100d7c990(self->owner));
            fn_710007b430(&target, &target_pos);
            fn_7100117ba0(area, &target, &edge, &t, &closest, &distance, false, UnkPointFilter(),
                          UnkEdgeVisitor());
        }
        CVector2D own;
        UnkActorPosition own_pos = fn_7100d7b7a8(self->owner);
        fn_710007b430(&own, reinterpret_cast<const CVector3D*>(&own_pos));
        f32 dx = closest.x - own.x;
        f32 dy = closest.y - own.y;
        if (!(dx * dx + dy * dy > 100.0f))
            return true;
    }

    f32 arc = reinterpret_cast<UnkObsydomithonTunable_l629600&>(g_unk_71726f8988).arc;
    if (fn_7100d7c990(self->owner)->field_0x54 > self->owner->field_0x54 && !InArc(angle, 90.0f, arc))
        return false;
    if (fn_7100d7c990(self->owner)->field_0x54 < self->owner->field_0x54)
        return InArc(angle, 270.0f, arc);
    return true;
}
