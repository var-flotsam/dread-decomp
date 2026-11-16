// Unit 437 (aim component): fn_7100345464 rates the room in front of the owner along its facing (or against it):
// the player's probe from the owner's position gives a reach; the segment from the owner over that reach is tested
// against the scenario's collision tree, and the result (never below zero) depends on how far the hit is and on
// what was hit. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkTU367.h"
#include "game/UnkXformNode.h"
#include "types.h"

#include <math.h>

using base::math::CVector2D;
using base::math::CVector3D;

extern void* g_unk_7101a23e70[];

struct UnkProbeResult_l345464 {
    f32 field_0x0[7];
};
bool fn_71010e6680(void* player, const UnkVec3f* pos, const UnkVec3f* dir, f32* field_out, f32* reach,
                   UnkProbeResult_l345464* result);

// A reference to a counted object, released when the count drops to zero.
struct UnkHitRef_l345464 {
    base::UnkRefCounted* ptr;

    UnkHitRef_l345464() : ptr(nullptr) {}
    ~UnkHitRef_l345464() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkHitVec_l345464 {
    f32 x, y, z;

    UnkHitVec_l345464(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_l345464 {
    UnkHitRef_l345464 object;
    UnkHitRef_l345464 field_0x8;
    s32 id;
    UnkHitVec_l345464 point;
    UnkHitVec_l345464 field_0x20;
    UnkHitVec_l345464 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l345464()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkCollisionHit_l345464) == 0x48, "layout");

// The query's callback object: the closest hit so far and the segment's origin, twice. The vtables (reached
// through the GOT) are the callback's and its base's.
struct UnkClearanceQuery_l345464 {
    void** vtable;
    UnkCollisionHit_l345464 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;
    bool field_0x70;
    u32 field_0x74;
    CVector3D field_0x78;
    u64 field_0x88;

    explicit UnkClearanceQuery_l345464(const CVector3D& o) {
        vtable = g_unk_7101a23e70 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
        field_0x70 = false;
        field_0x78.x = o.x;
        field_0x78.y = o.y;
        field_0x78.z = o.z;
        field_0x88 = 0;
    }
    ~UnkClearanceQuery_l345464() { vtable = g_unk_7101a20ed8 + 2; }
};
static_assert(sizeof(UnkClearanceQuery_l345464) == 0x90, "layout");

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkClearanceSegment_l345464 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkClearanceSegment_l345464(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct UnkClearancePoint_l345464 : CVector2D {
    explicit UnkClearancePoint_l345464(const UnkVec3f& v) { fn_710007b430(this, reinterpret_cast<const CVector3D*>(&v)); }
};

struct UnkClearanceComponent_l345464 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x28];
    UnkWeakTarget* lock;  // 0x30
};

struct UnkClearanceEntity_l345464 {
    u8 pad_0x0[0x10];
    UnkClearanceComponent_l345464* component;
};

struct UnkClearanceObject_l345464 {
    u8 pad_0x0[0x10];
    UnkClearanceEntity_l345464* entity;
};

struct UnkClearanceActor_l345464 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;
    UnkVec3f Position() const { return UnkVec3f(transform.m[3], transform.m[7], transform.m[11]); }
};

struct UnkClearanceOwner_l345464 {
    u8 pad_0x0[0x30];
    UnkClearanceActor_l345464* actor;
};

static inline UnkVec3f Facing(UnkClearanceActor_l345464* actor) {
    const UnkMatrix44& m = fn_710002614c(&actor->transform);
    return UnkVec3f(m.m[2], m.m[6], m.m[10]);
}

// Whether `mark` lies within 10 units of `depth`.
static inline bool Near(f32 depth, f32 mark) {
    return depth + 10.0f >= mark && depth - 10.0f <= mark;
}

// Whether the hit object is a collision component that guards a door lock.
static inline bool HitsDoorLock(const UnkClearanceQuery_l345464& query) {
    auto* object = reinterpret_cast<UnkClearanceObject_l345464*>(query.hit.object.ptr);
    if (!object || !object->entity)
        return false;
    UnkClearanceComponent_l345464* component = object->entity->component;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_710085fc44()))
        return false;
    return component->lock && fn_71002af860(component->lock);
}

f32 fn_7100345464(UnkClearanceOwner_l345464* self, bool backwards) {
    void* player = fn_7100defcec(g_unk_7101d47310);
    f32 field_out;
    f32 reach;
    UnkProbeResult_l345464 probe;
    {
        UnkVec3f pos = self->actor->Position();
        UnkVec3f ahead = Facing(self->actor);
        UnkVec3f facing = backwards ? UnkVec3f(-ahead.x, -ahead.y, -ahead.z) : ahead;
        if (!fn_71010e6680(player, &pos, &facing, &field_out, &reach, &probe))
            return 0.0f;
    }
    UnkGameManager* gm = g_unk_7101d47310;
    UnkTree* tree = gm->field_0x608 ? static_cast<UnkTree*>(gm->field_0x608->field_0x820) : nullptr;
    CVector2D start = UnkClearancePoint_l345464(self->actor->Position());
    CVector2D dir = UnkClearancePoint_l345464(Facing(self->actor));
    f32 sign = backwards ? -1.0f : 1.0f;
    dir.x = sign * dir.x;
    dir.y = sign * dir.y;
    CVector2D end;
    end.x = start.x - dir.x * reach;
    end.y = start.y - dir.y * 0.0f;
    UnkClearanceSegment_l345464 segment(start, end);
    UnkClearanceQuery_l345464 query(CVector3D{start.x, start.y, 0.0f});
    f32 clearance;
    if (fn_7100d5c518(tree, 9, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
        f32 dist = fabsf(start.x - query.hit.point.x);
        f32 depth = reach - dist;
        if (depth > 252.0f) {
            clearance = 200.0f - (reach - 100.0f);
        } else {
            f32 room;
            if (HitsDoorLock(query))
                room = 200.0f;
            else if (Near(depth, 50.0f))
                room = 270.0f;
            else if (Near(depth, 100.0f))
                room = 230.0f;
            else if (Near(depth, 150.0f))
                room = 270.0f;
            else if (Near(depth, 200.0f))
                room = 270.0f;
            else if (Near(depth, 250.0f))
                room = 230.0f;
            else
                room = 270.0f;
            clearance = room - dist;
        }
    } else {
        clearance = 200.0f - (reach - 100.0f);
    }
    return clearance > 0.0f ? clearance : 0.0f;
}
