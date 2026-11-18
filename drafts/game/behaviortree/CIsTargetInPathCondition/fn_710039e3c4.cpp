// behaviortree::CIsTargetInPathCondition: whether the target stands in the actor's path: a downward segment
// from the target hits ground near enough to the AI's area, and (when asked) the two do not face the same way.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU108.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// The AI component: its path radius (slot 0x3c8) and the area paths are tested in (+0x68).
struct UnkPathAI_itp {
    virtual void vf_0x0();
    u8 pad_0x8[0x60];
    u8 area[0x8];  // 0x68

    f32 GetRadius() {
        return reinterpret_cast<f32 (*)(UnkPathAI_itp*)>((*reinterpret_cast<void***>(this))[0x3c8 / 8])(this);
    }
};

// The collision hit the query keeps: the object hit, a second reference, an id, the point and two vectors.
struct UnkPathHit_itp {
    base::UnkRefPtrMember object;
    base::UnkRefPtrMember field_0x8;
    s32 id;
    CVector3D point;
    CVector3D field_0x20;
    CVector3D normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;
};
static_assert(sizeof(UnkPathHit_itp) == 0x48, "UnkPathHit_itp layout");

// The segment query's callback object; the vtables (reached through the GOT) are the callback's and its base's.
struct UnkPathQuery_itp {
    void** vtable;
    UnkPathHit_itp hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;  // 0x58
    u64 field_0x68;

    explicit UnkPathQuery_itp(const CVector3D& o) : origin(o) {
        vtable = g_unk_7101a20fa8 + 2;
        hit.object.ptr = nullptr;
        hit.field_0x8.ptr = nullptr;
        hit.id = -1;
        hit.point.x = 0.0f;
        hit.point.y = 0.0f;
        hit.point.z = 0.0f;
        hit.field_0x20.x = 0.0f;
        hit.field_0x20.y = 0.0f;
        hit.field_0x20.z = 0.0f;
        hit.normal.x = 0.0f;
        hit.normal.y = 0.0f;
        hit.normal.z = 0.0f;
        hit.field_0x38 = 0;
        hit.flags = 0;
        hit.field_0x40 = 0;
        field_0x50 = false;
        field_0x68 = 0;
    }
    ~UnkPathQuery_itp() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkPathSegment_itp {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkPathSegment_itp(const CVector2D& s, const CVector2D& e) : start(s), end(e) {
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct UnkIsTargetInPath_itp {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
    u8 pad_0x20[0x18];
    f32 depth;   // 0x38: how far down the segment reaches
    f32 radius;  // 0x3c: the AI's own radius when not above zero
    bool check_facing;  // 0x40
};

static inline CVector3D Position(const UnkActor* actor) {
    CVector3D p;
    p.x = actor->field_0x44;
    p.y = actor->field_0x54;
    p.z = actor->field_0x64;
    return p;
}

static inline CVector3D Add(const CVector3D& a, const CVector3D& b) {
    CVector3D r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

static inline const UnkMatrix44& Transform(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

bool fn_710039e3c4(UnkIsTargetInPath_itp* self) {
    UnkPathAI_itp* ai = static_cast<UnkPathAI_itp*>(self->actor->field_0x240);
    UnkActor* target = fn_7100d7c990(self->actor);
    UnkGameManager* gm = g_unk_7101d47310;
    UnkTree* tree = gm->field_0x608 ? static_cast<UnkTree*>(gm->field_0x608->field_0x820) : nullptr;

    bool result = false;
    if (ai && target && tree) {
        f32 distance;
        CVector3D pos = Position(target);
        CVector2D start = {pos.x, pos.y};
        CVector2D end = {start.x - 0.0f * self->depth, start.y - 1.0f * self->depth};
        UnkPathSegment_itp segment(start, end);
        UnkPathQuery_itp query(pos);
        if (fn_7100d5c518(tree, 0x80401, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
            CVector3D above = Add(query.hit.point, CVector3D{0.0f, 5.0f, 0.0f});
            f32 fallback = ai->GetRadius();
            f32 radius = self->radius > -0.001f ? self->radius : fallback;
            CVector2D at;
            fn_710007b430(&at, &above);
            result = fn_71001176e8(reinterpret_cast<UnkShakernautArea_b65a4*>(ai->area), &at,
                                   reinterpret_cast<u32*>(&distance), nullptr, 1, radius) &&
                     distance < 100.0f;
        }
    }

    if (self->check_facing && target && target->field_0x208 && fn_71009e12d0(target->field_0x208) &&
        self->actor->field_0x54 < target->field_0x54) {
        const UnkMatrix44& own = Transform(self->actor);
        f32 x = own.m[2];
        f32 y = own.m[6];
        f32 z = own.m[10];
        const UnkMatrix44& other = Transform(target);
        f32 ox = other.m[2];
        f32 oy = other.m[6];
        f32 oz = other.m[10];
        if (x * ox + y * oy + z * oz > -0.001f)
            return false;
    }
    return result;
}
