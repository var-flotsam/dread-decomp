// The CSamusAirMovementState::CParams unit: while rising with a capsule collider, probes the collision tree
// upward from the collider's centre and from its two sides; when only one side is blocked, pushes the
// position sideways away from it (by less near a polygon corner) and marks the push done. Names and
// layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkPolygonCollection2D;
UnkSegment2D fn_710012436c(const UnkPolygonCollection2D* self, u32 handle);

// The movement settings (g_unk_7172740de0), as far as this function reads them.
struct UnkAirSettings_l98d034 {
    u8 pad_0x0[0x4c];
    f32 probe_margin;  // 0x4c
    f32 push_free;     // 0x50
    f32 push_corner;   // 0x54
    f32 corner_range;  // 0x58
};

// A collision shape: its kind (8 a capsule, 0x10 a polygon collection) and its centre offset.
struct UnkAirShape_l98d034 {
    void** vtable;
    u8 pad_0x8[0xa0];
    u8 polygons[1];  // 0xa8, the polygon collection of a kind 0x10 shape

    s32 GetKind() { return reinterpret_cast<s32 (*)(UnkAirShape_l98d034*)>(vtable[0x18 / 8])(this); }
    UnkVec3f GetCenter() { return reinterpret_cast<UnkVec3f (*)(UnkAirShape_l98d034*)>(vtable[0x68 / 8])(this); }
};
f32 fn_7100d60238(UnkAirShape_l98d034* shape);
f32 fn_7100d60230(UnkAirShape_l98d034* shape);

struct UnkAirCollider_l98d034 {
    u8 pad_0x0[0x158];
    UnkAirShape_l98d034* shape;  // 0x158
};

// What a hit object belongs to.
struct UnkHitObject_l98d034 : base::UnkRefCounted {
    UnkAirCollider_l98d034* owner;
};

// A reference to a counted object, released when the count drops to zero.
struct UnkHitRef_l98d034 {
    UnkHitObject_l98d034* ptr;

    UnkHitRef_l98d034() : ptr(nullptr) {}
    ~UnkHitRef_l98d034() {
        if (UnkHitObject_l98d034* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkHitVec_l98d034 {
    f32 x, y, z;

    UnkHitVec_l98d034(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, two
// vectors and flags.
struct UnkHit_l98d034 {
    UnkHitRef_l98d034 object;
    UnkHitRef_l98d034 field_0x8;
    s32 id;
    UnkHitVec_l98d034 point;
    UnkHitVec_l98d034 field_0x20;
    UnkHitVec_l98d034 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkHit_l98d034()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkHit_l98d034) == 0x48, "UnkHit_l98d034 layout");

// The segment query's callback object: the closest hit so far and the segment's origin. The
// vtables (reached through the GOT) are the callback's and its base's.
struct UnkSegmentQuery_l98d034 {
    void** vtable;
    UnkHit_l98d034 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentQuery_l98d034(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentQuery_l98d034() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkSegment_l98d034 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSegment_l98d034(const CVector3D& s, const CVector3D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

static inline CVector3D operator+(const CVector3D& a, const CVector3D& b) {
    return CVector3D{a.x + b.x, a.y + b.y, a.z + b.z};
}
static inline CVector3D operator*(const CVector3D& v, f32 s) {
    return CVector3D{v.x * s, v.y * s, v.z * s};
}

static inline f32 Sign(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

static inline f32 Square(f32 v) {
    return v * v;
}

struct UnkAirMovement_l98d034 {
    void** vtable;
    u8 pad_0x8[0x3b0];
    UnkAirCollider_l98d034* collider;       // 0x3b8
    UnkAirCollider_l98d034* collider_alt;   // 0x3c0
    u8 pad_0x3c8[4];
    u32 mask;  // 0x3cc

    bool vf_0x350() { return reinterpret_cast<bool (*)(UnkAirMovement_l98d034*)>(vtable[0x350 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkAirMovement_l98d034, mask) == 0x3cc, "layout");

struct UnkAirOwner_l98d034 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkAirState_l98d034 {
    u8 pad_0x0[0x30];
    UnkAirOwner_l98d034* owner;          // 0x30
    UnkAirMovement_l98d034* movement;    // 0x38
    u8 pad_0x40[0x210 - 0x40];
    bool pushed;  // 0x210
};

static inline bool Probe(UnkTree* tree, UnkAirState_l98d034* self, const UnkSegment_l98d034& segment,
                         UnkSegmentQuery_l98d034& query) {
    return fn_7100d5c518(tree, self->movement->mask, reinterpret_cast<const UnkBox*>(&segment), &query, 2);
}

UnkVec3f fn_710098d034(UnkAirState_l98d034* self, const UnkVec3f* in) {
    const UnkAirSettings_l98d034* settings = reinterpret_cast<const UnkAirSettings_l98d034*>(&g_unk_7172740de0);
    if (settings->probe_margin <= 0.0f)
        return *in;
    if (!self->movement->vf_0x350())
        return *in;
    if (in->y <= 0.0f)
        return *in;
    if (self->pushed)
        return *in;
    UnkAirMovement_l98d034* movement = self->movement;
    UnkAirCollider_l98d034* collider = movement->collider_alt ? movement->collider_alt : movement->collider;
    UnkAirShape_l98d034* shape = collider->shape;
    s32 kind = shape->GetKind();
    if (!shape || kind != 8)
        return *in;

    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    f32 height = fn_7100d60238(shape) * 2.0f + settings->probe_margin;
    f32 width = fn_7100d60230(shape) * 0.999f;
    UnkVec3f position = self->owner->xform.GetTranslation();
    CVector3D center;
    {
        UnkVec3f offset = shape->GetCenter();
        center = {position.x + offset.x, position.y + offset.y, position.z + offset.z};
    }
    CVector3D up = {0.0f, 1.0f, 0.0f};
    CVector3D side = {1.0f, 0.0f, 0.0f};

    UnkSegment_l98d034 center_segment(center, center + up * height);
    UnkSegmentQuery_l98d034 center_query(center);
    bool center_hit = Probe(tree, self, center_segment, center_query);

    CVector3D left = center + CVector3D{-1.0f, 0.0f, 0.0f} * width;
    UnkSegment_l98d034 left_segment(left, left + up * height);
    UnkSegmentQuery_l98d034 left_query(left);
    bool left_hit = Probe(tree, self, left_segment, left_query);

    CVector3D right = center + side * width;
    UnkSegment_l98d034 right_segment(right, right + up * height);
    UnkSegmentQuery_l98d034 right_query(right);
    bool right_hit = Probe(tree, self, right_segment, right_query);

    f32 push_by = 0.0f;
    if (left_hit || right_hit) {
        f32 limit;
        if (center_hit && left_hit != right_hit) {
            // Pushed by the corner amount only near the x of the closer end of the polygon edge hit.
            limit = settings->push_corner;
            bool near_corner = false;
            const UnkHit_l98d034& hit = center_query.hit;
            UnkAirShape_l98d034* hit_shape = hit.object.ptr->owner->shape;
            if (hit_shape && hit_shape->GetKind() == 0x10) {
                UnkSegment2D edge =
                    fn_710012436c(reinterpret_cast<const UnkPolygonCollection2D*>(hit_shape->polygons), hit.id);
                CVector2D point;
                fn_710007b430(&point, reinterpret_cast<const CVector3D*>(&hit.point));
                f32 to_start = Square(edge.start.x - point.x) + Square(edge.start.y - point.y);
                f32 to_end = Square(edge.end.x - point.x) + Square(edge.end.y - point.y);
                f32 corner_x = to_start > to_end ? edge.end.x : edge.start.x;
                near_corner = fabsf(corner_x - point.x) < settings->corner_range;
            }
            if (!near_corner)
                limit = 0.0f;
        } else {
            limit = settings->push_free;
        }

        f32 push;
        if (left_hit && !right_hit)
            push = 1.0f;
        else if (!left_hit && right_hit)
            push = -1.0f;
        else
            push = 0.0f;

        f32 x = in->x;
        if (Sign(push) == Sign(x)) {
            f32 room = limit - fabsf(x);
            limit = room <= 0.0f ? 0.0f : (room >= limit ? limit : room);
        }
        push_by = push * limit;
        self->pushed = true;
    }
    return UnkVec3f(push_by + in->x, in->y + 0.0f, in->z + 0.0f);
}
