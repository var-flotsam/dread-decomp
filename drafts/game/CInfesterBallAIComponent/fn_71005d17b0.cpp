// CInfesterBallAIComponent's unit (tools/tu_map.py unit 898): probes the collision tree below the ball and to
// either side of it to tell whether it stands at an edge. The hit and query types are those of
// src/game/UnkTU1501/UnkTU1501_FloorProbe_l9ee384.cpp. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
// What a hit object belongs to, and that owner's kind (its hash at +0x30).
struct UnkHitKind_l9ee384 {
    u8 pad_0x0[0x30];
    u64 hash;
};
struct UnkHitOwner_l9ee384 {
    u8 pad_0x0[0x20];
    UnkHitKind_l9ee384* kind;
};
struct UnkHitObject_l9ee384 : base::UnkRefCounted {
    UnkHitOwner_l9ee384* owner;
};

// A reference to a counted object, released when the count drops to zero; assigned out of line.
struct UnkHitRef_l9ee384 {
    UnkHitObject_l9ee384* ptr;

    UnkHitRef_l9ee384() : ptr(nullptr) {}
    ~UnkHitRef_l9ee384() {
        if (UnkHitObject_l9ee384* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRef_l9ee384& operator=(const UnkHitRef_l9ee384& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
};

struct UnkHitVec_l9ee384 {
    f32 x, y, z;

    UnkHitVec_l9ee384(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkHitVec_l9ee384& operator=(const UnkHitVec_l9ee384& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, two
// vectors and flags.
struct UnkHit_l9ee384 {
    UnkHitRef_l9ee384 object;
    UnkHitRef_l9ee384 field_0x8;
    s32 id;
    UnkHitVec_l9ee384 point;
    UnkHitVec_l9ee384 field_0x20;
    UnkHitVec_l9ee384 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkHit_l9ee384()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
    UnkHit_l9ee384& operator=(const UnkHit_l9ee384& o) {
        if (this == &o)
            return *this;
        object = o.object;
        field_0x8 = o.field_0x8;
        id = o.id;
        point = o.point;
        field_0x20 = o.field_0x20;
        normal = o.normal;
        field_0x38 = o.field_0x38;
        flags = o.flags;
        field_0x40 = o.field_0x40;
        return *this;
    }

    // Whether the object hit is of one of the two kinds that are probed through again.
    bool IsPassThrough() const {
        if (!object.ptr || !object.ptr->owner)
            return false;
        u64 hash = object.ptr->owner->kind->hash;
        return hash == 0x02906979c2f19539 || hash == 0xfb8851303c4b6176;
    }
};
static_assert(sizeof(UnkHit_l9ee384) == 0x48, "UnkHit_l9ee384 layout");

// The segment query's callback object: the closest hit so far and the segment's origin. The
// vtables (reached through the GOT) are the callback's and its base's.
struct UnkSegmentQuery_l9ee384 {
    void** vtable;
    UnkHit_l9ee384 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentQuery_l9ee384(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentQuery_l9ee384() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkSegment_l9ee384 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSegment_l9ee384(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct UnkTree;
struct UnkBox;

struct UnkInfesterOwner_l5d17b0 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38; its translation is the position
    u8 pad_0x78[0x304];
    f32 radius;  // 0x37c
};
static_assert(__builtin_offsetof(UnkInfesterOwner_l5d17b0, radius) == 0x37c, "layout");

struct UnkInfesterBallAI_l5d17b0 {
    u8 pad_0x0[0x30];
    UnkInfesterOwner_l5d17b0* owner;
    u8 pad_0x38[0x14740];
    CVector2D direction;  // 0x14778
};
static_assert(__builtin_offsetof(UnkInfesterBallAI_l5d17b0, direction) == 0x14778, "layout");

static inline CVector3D Position(const UnkInfesterOwner_l5d17b0* owner) {
    return CVector3D{owner->transform.m[3], owner->transform.m[7], owner->transform.m[11]};
}

// Whether the ball sits on an edge: nothing below it, but the floor is hit 100 units to either side of the
// point below it, along its direction, at points 100 units apart.
bool fn_71005d17b0(UnkInfesterBallAI_l5d17b0* self) {
    bool on_edge = false;
    CVector2D pos = {self->owner->transform.m[3], self->owner->transform.m[7]};
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    f32 depth = self->owner->radius + 10.0f;
    UnkSegment_l9ee384 down(pos, CVector2D{pos.x - m.m[1] * depth, pos.y - m.m[5] * depth});
    UnkSegmentQuery_l9ee384 below(Position(self->owner));
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    if (!fn_7100d5c518(tree, 1, reinterpret_cast<const UnkBox*>(&down), &below, 2)) {
        CVector2D center = {self->owner->transform.m[3], self->owner->transform.m[7]};
        const UnkMatrix44& m2 = fn_710002614c(&self->owner->transform);
        f32 depth2 = self->owner->radius + 10.0f;
        CVector2D foot = {center.x - m2.m[1] * depth2, center.y - m2.m[5] * depth2};
        CVector2D side = {self->direction.x * 100.0f, self->direction.y * 100.0f};
        UnkSegment_l9ee384 ahead(foot, CVector2D{foot.x + side.x, foot.y + side.y});
        UnkSegment_l9ee384 behind(foot, CVector2D{foot.x - side.x, foot.y - side.y});
        UnkSegmentQuery_l9ee384 hit_ahead(Position(self->owner));
        UnkSegmentQuery_l9ee384 hit_behind(Position(self->owner));
        bool found_ahead = fn_7100d5c518(tree, 1, reinterpret_cast<const UnkBox*>(&ahead), &hit_ahead, 2);
        bool found_behind = fn_7100d5c518(tree, 1, reinterpret_cast<const UnkBox*>(&behind), &hit_behind, 2);
        if (found_ahead && found_behind) {
            f32 dx = hit_ahead.hit.point.x - hit_behind.hit.point.x;
            f32 dy = hit_ahead.hit.point.y - hit_behind.hit.point.y;
            f32 dz = hit_ahead.hit.point.z - hit_behind.hit.point.z;
            f32 dist_sq = dx * dx + dy * dy + dz * dz;
            if (dist_sq - 1.0f <= 10000.0f && dist_sq + 1.0f >= 10000.0f)
                on_edge = true;
        }
    }
    return on_edge;
}