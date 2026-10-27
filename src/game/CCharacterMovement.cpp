// CCharacterMovement (tools/tu_map.py unit 1445, fn_71009754a4 to fn_710097aa78). This file holds
// the downward collision probe fn_7100977e50: from the centre of the owner's bounding box (lifted
// to at most a step height above its bottom) it casts a segment along a direction through the
// collision tracker of the game manager's +0x608 (fn_7100d5c518) and accepts the first hit whose
// normal faces the direction closely enough.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// The out-of-line assignment both hit references share (declared as the other hand-written files
// do).
struct UnkAssignedSlot {
    void* ptr;
};
void fn_71002a5d1c(UnkAssignedSlot* self, const UnkAssignedSlot* other);

// A reference to a counted object (base::UnkRefCounted), released when the count drops to zero.
// The second one keeps its pointer in a union: the original reloads it after the count increment.
struct UnkHitRefA {
    base::UnkRefCounted* ptr;

    UnkHitRefA() : ptr(nullptr) {}
    UnkHitRefA(const UnkHitRefA& other) : ptr(nullptr) { Set(other.ptr); }
    ~UnkHitRefA() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRefA& operator=(const UnkHitRefA& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
    void Set(base::UnkRefCounted* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (base::UnkRefCounted* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
};
struct UnkHitRefB {
    union {
        base::UnkRefCounted* ptr;
    };

    UnkHitRefB() : ptr(nullptr) {}
    UnkHitRefB(const UnkHitRefB& other) : ptr(nullptr) { Set(other.ptr); }
    ~UnkHitRefB() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRefB& operator=(const UnkHitRefB& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
    void Set(base::UnkRefCounted* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (base::UnkRefCounted* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
};

// A vector copied component by component.
struct UnkHitVec {
    f32 x, y, z;

    UnkHitVec() {}
    UnkHitVec(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkHitVec(const UnkHitVec& o) : x(o.x), y(o.y), z(o.z) {}
    UnkHitVec& operator=(const UnkHitVec& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, the
// reversed normal and the normal, and flags.
struct UnkCollisionHit {
    UnkHitRefA object;
    UnkHitRefB field_0x8;
    s32 id;
    UnkHitVec point;
    UnkHitVec field_0x20;
    UnkHitVec normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
    // The hit as seen from the other side: the id kept only when flag 0 says it is set, the
    // second vector the reversed normal, flags 0x21 added.
    struct Reversed {};
    UnkCollisionHit(const UnkCollisionHit& o, Reversed)
        : object(o.object), field_0x8(o.field_0x8), id(o.flags & 1 ? o.id : -1), point(o.point), normal(o.normal),
          field_0x38(o.field_0x38), flags(o.flags | 0x21), field_0x40(o.field_0x40) {
        field_0x20 = UnkHitVec(-normal.x, -normal.y, -normal.z);
    }
    UnkCollisionHit& operator=(const UnkCollisionHit& o) {
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
};
static_assert(sizeof(UnkCollisionHit) == 0x48, "UnkCollisionHit layout");

// The segment query's callback object: the closest hit so far and the segment's origin. The
// vtables (reached through the GOT) are the callback's and its base's.
extern void* g_unk_7101a20fa8[];
extern void* g_unk_7101a20ed8[];
struct UnkSegmentHitQuery {
    void** vtable;
    UnkCollisionHit hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentHitQuery(const CVector3D& o) : field_0x50(false), field_0x68(0) {
        vtable = g_unk_7101a20fa8 + 2;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
    }
    ~UnkSegmentHitQuery() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkHitSegment {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkHitSegment(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// The tree query (src/game_hi/fn_7100d5c518.cpp, whose names for the tree and the box these are).
struct UnkTree;
struct UnkBox;
bool fn_7100d5c518(UnkTree* self, u32 mask, const UnkBox* box, void* arg, u32 arg2);

// A 3D point projected onto the plane (fn_710007b430 builds it in place).
struct UnkProbePoint2D : CVector2D {
    explicit UnkProbePoint2D(const CVector3D& v) { fn_710007b430(this, &v); }
};

// The owner's shape at +0x158: its vf_0x28 gives the bounding box of the shape at its +0x8.
struct UnkProbeBounds {
    CVector3D min;
    CVector3D max;
};
struct UnkMovementShape {
    void** vtable;
    u8 field_0x8[1];

    UnkProbeBounds GetBounds() {
        return reinterpret_cast<UnkProbeBounds (*)(UnkMovementShape*, void*)>(vtable[0x28 / 8])(this, field_0x8);
    }
};
struct UnkCharacterMovement {
    u8 pad_0x0[0x158];
    UnkMovementShape* shape;
};

// Casts the probe and, on an accepted hit (dot(dir, normal) <= max_dot), copies it to out_hit and
// its point, relative to the bottom of the bounding box, to out_point.
// NON_MATCHING(minor): instruction scheduling and float register allocation of the probe geometry
// (the original keeps back in s11 and step in s10, ours the other way round, and pairs the loads of
// pos differently), of the reversed copy of the hit (stores paired otherwise) and of out_point (the
// original stores x and y as a pair, ours y and z).
bool fn_7100977e50(UnkCharacterMovement* self, f32 max_dot, f32 extra, f32 step, f32 back, u32 mask,
                   const CVector3D* pos, CVector3D* out_point, UnkCollisionHit* out_hit, const CVector3D* dir) {
    UnkGameManager* gm = g_unk_7101d47310;
    UnkTree* tree = gm->field_0x608 ? static_cast<UnkTree*>(gm->field_0x608->field_0x820) : nullptr;

    UnkProbeBounds bounds = self->shape->GetBounds();
    f32 sum_x = bounds.min.x + bounds.max.x;
    f32 sum_z = bounds.min.z + bounds.max.z;
    f32 center_y = (bounds.min.y + bounds.max.y) * 0.5f;
    f32 top = bounds.min.y + step;
    f32 cx = sum_x * 0.5f;
    f32 y = center_y < top ? center_y : top;
    f32 px = pos->x;
    f32 py = pos->y;
    f32 cz = sum_z * 0.5f;
    f32 bx = dir->x * back;
    f32 by = dir->y * back;
    f32 bz = dir->z * back;
    f32 rise = y - bounds.min.y;
    f32 sx = cx + px;
    f32 sy = y + py;
    f32 length = rise + extra;
    f32 sz = cz + pos->z;
    CVector3D start = {bx + sx, by + sy, bz + sz};
    f32 dx = dir->x * length - bx;
    f32 dy = dir->y * length - by;
    f32 dz = dir->z * length - bz;

    UnkSegmentHitQuery query(start);
    UnkHitSegment segment(UnkProbePoint2D(start), UnkProbePoint2D(CVector3D{dx + start.x, dy + start.y, dz + start.z}));

    bool result;
    if (tree == nullptr || !fn_7100d5c518(tree, mask, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
        result = false;
    } else {
        UnkCollisionHit hit(query.hit, UnkCollisionHit::Reversed());
        f32 d = dir->x * hit.normal.x + dir->y * hit.normal.y + dir->z * hit.normal.z;
        result = d <= max_dot;
        if (result) {
            if (out_hit != nullptr)
                *out_hit = hit;
            if (out_point != nullptr) {
                out_point->x = hit.point.x + 0.0f;
                out_point->y = hit.point.y - bounds.min.y;
                out_point->z = hit.point.z + 0.0f;
            }
        }
    }
    return result;
}
