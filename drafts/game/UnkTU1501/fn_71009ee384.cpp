// Unit 1501 (CSamusMovement's neighbours): probes the collision tree straight down through a point
// for the floor below it, probing again from the hit when it is one of two kinds of object, and gives
// the hit and its height relative to the point. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkAssignedSlot {
    void* ptr;
};

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
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
        field_0x50 = false;
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

struct UnkProbeConfig_l9ee384 {
    u8 pad_0x0[0x378];
    u32 mask;
};
struct UnkProbeOwner_l9ee384 {
    u8 pad_0x0[0x30];
    UnkProbeConfig_l9ee384* config;
};

void fn_71009ee384(UnkProbeOwner_l9ee384* self, const CVector3D* pos, UnkHit_l9ee384* out_hit, f32* out_height) {
    *out_height = 0.0f;
    UnkGameManager* gm = g_unk_7101d47310;
    if (!gm->field_0x608)
        return;
    UnkTree* tree = static_cast<UnkTree*>(gm->field_0x608->field_0x820);
    if (!tree)
        return;

    CVector3D start = {pos->x + 0.0f, pos->y + 80.0f, pos->z + 0.0f};
    UnkSegmentQuery_l9ee384 query(start);
    UnkSegment_l9ee384 segment(CVector2D{start.x, start.y}, CVector2D{pos->x + 0.0f, pos->y + -80.0f});
    if (!fn_7100d5c518(tree, self->config->mask, reinterpret_cast<const UnkBox*>(&segment), &query, 2))
        return;
    *out_hit = query.hit;

    if (out_hit->IsPassThrough()) {
        query.field_0x50 = false;
        UnkSegment_l9ee384 again(CVector2D{out_hit->point.x + 0.0f, out_hit->point.y + 80.0f},
                                 CVector2D{pos->x + 0.0f, pos->y + -80.0f});
        if (fn_7100d5c518(tree, self->config->mask, reinterpret_cast<const UnkBox*>(&again), &query, 2))
            *out_hit = query.hit;
    }
    *out_height = query.hit.point.y - pos->y;
}
