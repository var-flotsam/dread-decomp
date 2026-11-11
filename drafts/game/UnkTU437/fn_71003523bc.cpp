// Unit 437: whether the segment between two points (projected onto the plane) hits the collision
// of the object the filter at **self names: false when nothing is hit or the game manager has no
// tracker, true when the first hit is not on a CCollisionComponent whose +0x30 is that object.
// Built from the segment query of src/game/CCharacterMovement.cpp. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
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
    union {
        base::UnkRefCounted* ptr;
    };

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
extern void* g_unk_7101a20fa8[];
extern void* g_unk_7101a20ed8[];
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


struct UnkTree;
struct UnkBox;
bool fn_7100d5c518(UnkTree* self, u32 mask, const UnkBox* box, void* arg, u32 arg2);

struct UnkProbePoint2D_ovb0198u : CVector2D {
    explicit UnkProbePoint2D_ovb0198u(const CVector3D& v) { fn_710007b430(this, &v); }
};

struct UnkHitComponent_ovb0198u : base::reflection::UnkReflObject {
    u8 pad_0x8[0x28];
    void* named;  // 0x30
};

struct UnkHitEntity_ovb0198u {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 GetFlags();  // 0x58
    u8 pad_0x8[0x8];
    UnkHitComponent_ovb0198u* component;  // 0x10
};

struct UnkHitObject_ovb0198u {
    u8 pad_0x0[0x10];
    UnkHitEntity_ovb0198u* entity;  // 0x10
};

static inline CVector3D Origin_ovb0198u(const CVector3D& a) {
    UnkProbePoint2D_ovb0198u first(a);
    return CVector3D{first.x, first.y, 0.0f};
}

struct UnkAimFilterSelf_ovb0198u {
    void** target;
};

bool fn_71003523bc(UnkAimFilterSelf_ovb0198u* self, const CVector3D* a, const CVector3D* b) {
    UnkGameManager* gm = g_unk_7101d47310;
    UnkTree* tree = gm->field_0x608 ? static_cast<UnkTree*>(gm->field_0x608->field_0x820) : nullptr;
    if (!tree)
        return false;
    UnkSegmentHitQuery_ovb0198u query(Origin_ovb0198u(*a));
    UnkHitSegment_ovb0198u segment(UnkProbePoint2D_ovb0198u{*a}, UnkProbePoint2D_ovb0198u{*b});
    if (!fn_7100d5c518(tree, 0x1089, reinterpret_cast<const UnkBox*>(&segment), &query, 2))
        return true;
    UnkHitObject_ovb0198u* object = reinterpret_cast<UnkHitObject_ovb0198u*>(query.hit.object.ptr);
    if (!object)
        return false;
    UnkHitEntity_ovb0198u* entity = object->entity;
    if (!entity)
        return false;
    if (entity->GetFlags() & 0x1000)
        return false;
    UnkHitComponent_ovb0198u* component = entity->component;
    void* named = nullptr;
    if (component && fn_710009d50c(component->vf_0x0(), fn_710085fc44()))
        named = component->named;
    return named == *self->target;
}
