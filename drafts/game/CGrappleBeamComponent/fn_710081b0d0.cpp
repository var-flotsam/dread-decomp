// CGrappleBeamComponent unit (tools/tu_map.py, unit 1277): whether the beam, once longer than 150, reaches its
// target with nothing in between: optionally refreshes the aim direction from the animation's bone, projects the
// target onto the beam on the plane and tests that segment for collision. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A reference to a counted object (base::UnkRefCounted), released when the count drops to zero.
struct UnkHitRefA_ovb0198u {
    base::UnkRefCounted* ptr;

    UnkHitRefA_ovb0198u() : ptr(nullptr) {}
    UnkHitRefA_ovb0198u(const UnkHitRefA_ovb0198u& other) : ptr(nullptr) { Set(other.ptr); }
    ~UnkHitRefA_ovb0198u() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRefA_ovb0198u& operator=(const UnkHitRefA_ovb0198u& other) {
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
struct UnkHitRefB_ovb0198u {
    base::UnkRefCounted* ptr;

    UnkHitRefB_ovb0198u() : ptr(nullptr) {}
    UnkHitRefB_ovb0198u(const UnkHitRefB_ovb0198u& other) : ptr(nullptr) { Set(other.ptr); }
    ~UnkHitRefB_ovb0198u() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRefB_ovb0198u& operator=(const UnkHitRefB_ovb0198u& other) {
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
struct UnkHitVec_ovb0198u {
    f32 x, y, z;

    UnkHitVec_ovb0198u() {}
    UnkHitVec_ovb0198u(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkHitVec_ovb0198u(const UnkHitVec_ovb0198u& o) : x(o.x), y(o.y), z(o.z) {}
    UnkHitVec_ovb0198u& operator=(const UnkHitVec_ovb0198u& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, the
// reversed normal and the normal, and flags.
struct UnkCollisionHit_ovb0198u {
    UnkHitRefA_ovb0198u object;
    UnkHitRefB_ovb0198u field_0x8;
    s32 id;
    UnkHitVec_ovb0198u point;
    UnkHitVec_ovb0198u field_0x20;
    UnkHitVec_ovb0198u normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_ovb0198u()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
    // The hit as seen from the other side: the id kept only when flag 0 says it is set, the
    // second vector the reversed normal, flags 0x21 added.
    struct Reversed {};
    UnkCollisionHit_ovb0198u(const UnkCollisionHit_ovb0198u& o, Reversed)
        : object(o.object), field_0x8(o.field_0x8), id(o.flags & 1 ? o.id : -1), point(o.point), normal(o.normal),
          field_0x38(o.field_0x38), flags(o.flags | 0x21), field_0x40(o.field_0x40) {
        field_0x20 = UnkHitVec_ovb0198u(-normal.x, -normal.y, -normal.z);
    }
    UnkCollisionHit_ovb0198u& operator=(const UnkCollisionHit_ovb0198u& o) {
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
static_assert(sizeof(UnkCollisionHit_ovb0198u) == 0x48, "UnkCollisionHit_ovb0198u layout");

// The segment query's callback object: the closest hit so far and the segment's origin. The
// vtables (reached through the GOT) are the callback's and its base's.
struct UnkSegmentHitQuery_ovb0198u {
    void** vtable;
    UnkCollisionHit_ovb0198u hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentHitQuery_ovb0198u(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentHitQuery_ovb0198u() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkHitSegment_ovb0198u {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkHitSegment_ovb0198u(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct UnkProbePoint2D_ovb0198u : CVector2D {
    explicit UnkProbePoint2D_ovb0198u(const CVector3D& v) { fn_710007b430(this, &v); }
};

// A point on the plane as a 3D point.
static inline CVector3D OnPlane_ovb0198u(const CVector2D& p) {
    return CVector3D{p.x, p.y, 0.0f};
}

struct UnkGrappleBeamPos_ovb0047;
void fn_71008191e4(UnkGrappleBeamPos_ovb0047* self);
struct UnkBeamVec_ovb0138;
void fn_710034f54c(void* object, u32 id, UnkBeamVec_ovb0138* dir);
base::reflection::UnkReflObject* fn_710034dd54(void* object);

struct UnkGrappleAnimation_l81b0d0 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x530 - 0x8];
    u32 field_0x530;
};

struct UnkGrappleActor_l81b0d0 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
    u8 pad_0x68[0x220 - 0x68];
    base::reflection::UnkReflObject* animation;  // 0x220
    u8 pad_0x228[0x2b0 - 0x228];
    void* field_0x2b0;

    CVector3D Position() const { return CVector3D{transform[0][3], transform[1][3], transform[2][3]}; }
    u32 AnimationId() const {
        base::reflection::UnkReflObject* a = animation;
        if (a && fn_710009d50c(a->vf_0x0(), fn_7100713bb0()))
            return static_cast<UnkGrappleAnimation_l81b0d0*>(a)->field_0x530;
        return 0;
    }
};

struct UnkGrappleLink_l81b0d0 {
    u8 pad_0x0[0x10];
    UnkGrappleActor_l81b0d0* target;  // 0x10
};

struct UnkGrappleCollider_l81b0d0 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x28];
    void* owner;  // 0x30
};

struct UnkGrappleHitEntity_l81b0d0 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* component;  // 0x10
};

struct UnkGrappleHitObject_l81b0d0 {
    u8 pad_0x0[0x10];
    UnkGrappleHitEntity_l81b0d0* entity;  // 0x10
};

struct UnkGrappleBeam_l81b0d0 {
    u8 pad_0x0[0x30];
    UnkGrappleActor_l81b0d0* actor;  // 0x30
    u8 pad_0x38[0x8];
    CVector3D dir;  // 0x40
    u8 pad_0x4c[0x230 - 0x4c];
    UnkGrappleLink_l81b0d0* link;  // 0x230
    u8 pad_0x238[0x538 - 0x238];
    f32 length;  // 0x538
};

static inline bool IsNearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

bool fn_710081b0d0(UnkGrappleBeam_l81b0d0* self, bool refresh) {
    if (!(self->length > 150.0f))
        return false;

    if (refresh) {
        fn_71008191e4(reinterpret_cast<UnkGrappleBeamPos_ovb0047*>(self));
        CVector3D dir;
        dir.x = 1.0f;
        dir.y = 0.0f;
        dir.z = 0.0f;
        UnkGrappleActor_l81b0d0* target = self->link->target;
        fn_710034f54c(target->field_0x2b0, target->AnimationId(), reinterpret_cast<UnkBeamVec_ovb0138*>(&dir));
        self->dir.x = dir.x;
        self->dir.y = dir.y;
        self->dir.z = dir.z;
    }

    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);

    UnkProbePoint2D_ovb0198u start(self->actor->Position());
    f32 length = self->length;
    CVector3D reach = {length * self->dir.x, length * self->dir.y, length * self->dir.z};
    CVector3D origin = self->actor->Position();
    UnkProbePoint2D_ovb0198u end(CVector3D{origin.x + reach.x, origin.y + reach.y, origin.z + reach.z});

    // Where the beam's line, from its start, comes nearest the target along the beam's longer axis.
    f32 dx = start.x - end.x;
    f32 dy = start.y - end.y;
    CVector2D nearest;
    {
        UnkProbePoint2D_ovb0198u target(self->link->target->Position());
        f32 tx = IsNearZero(dx) ? 0.0f : (target.x - start.x) / dx;
        f32 ty = IsNearZero(dy) ? 0.0f : (target.y - start.y) / dy;
        f32 t = __builtin_fabsf(dx) > __builtin_fabsf(dy) ? tx : (tx < ty && tx > 0.0f ? tx : ty);
        nearest.x = start.x + (target.x - start.x) * t;
        nearest.y = start.y + (target.y - start.y) * t;
    }

    UnkSegmentHitQuery_ovb0198u query(OnPlane_ovb0198u(start));
    UnkHitSegment_ovb0198u segment(end, nearest);
    if (!fn_7100d5c518(tree, 0x80401, reinterpret_cast<const UnkBox*>(&segment), &query, 2))
        return false;
    base::reflection::UnkReflObject* component =
        reinterpret_cast<UnkGrappleHitObject_l81b0d0*>(query.hit.object.ptr)->entity->component;
    if (component && fn_710009d50c(component->vf_0x0(), fn_710085fc44()))
        return fn_710034dd54(static_cast<UnkGrappleCollider_l81b0d0*>(component)->owner) == nullptr;
    return true;
}
