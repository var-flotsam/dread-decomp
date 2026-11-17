// UnkTU1125 (tools/tu_map.py unit 1125): pushes the owner away from the colliders that overlap its circle
// shape. Each contact adds to `push`, weighted by how deep it reaches into the circle.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkAtExit.h"
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU109.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkTree;
struct UnkBvhCollider;
struct UnkPolygonCollection2D;
struct UnkShapeFilter_590ac;

struct UnkUnit110Vec2 {
    f32 x;
    f32 y;
    UnkUnit110Vec2() {}
    UnkUnit110Vec2(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkUnit110Vec2(const UnkUnit110Vec2& o) : x(o.x), y(o.y) {}
};

struct UnkUnit110Segment {
    UnkUnit110Vec2 start;
    UnkUnit110Vec2 end;
};

bool fn_7100d5bfbc(UnkTree* self, u32 mask, UnkBvhCollider* shape, void* arg, u32 arg2);
UnkSegment2D fn_710012436c(const UnkPolygonCollection2D* polygons, u32 handle);
UnkUnit110Vec2 fn_71001261e8(const UnkUnit110Segment* seg, const UnkUnit110Vec2* p);
UnkColliderResult fn_7100d56604(UnkShapeTester_A5_b68* tester, void* a, void* owner, const UnkMatrix44& transform,
                                UnkShapeFilter_590ac* filter, u32 flags);

extern void* g_unk_7101a6ba70[];
extern void* g_unk_7101a23080[];

// A 3x4 transform; its translation is the last column.
struct UnkTransform_l70419c {
    f32 m[3][4];
    CVector3D Translation() const {
        CVector3D t;
        t.x = m[0][3];
        t.y = m[1][3];
        t.z = m[2][3];
        return t;
    }
};

// A collision shape: slot 0x18 says which kind it is.
struct UnkShape_l70419c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 GetType();
    u8 pad_0x8[0xa8 - 0x8];
};

struct UnkCircle_l70419c : UnkShape_l70419c {
    f32 radius;  // 0xa8
};

struct UnkPolygons_l70419c : UnkShape_l70419c {
    u8 polygons[1];  // 0xa8
};

template <typename T, s32 Type>
inline T* As(UnkShape_l70419c* shape) {
    return shape->GetType() == Type ? static_cast<T*>(shape) : nullptr;
}

struct UnkCollider_l70419c {
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual const UnkTransform_l70419c* GetTransform();
    u8 pad_0x8[0x8];
    void* container;  // 0x10
    u8 pad_0x10[0x158 - 0x10];
    UnkShape_l70419c* shape;  // 0x158
};

// A reference-counted handle on a collider.
struct UnkColliderRef_l70419c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Destroy();
    s32 ref_count;
    UnkCollider_l70419c* collider;  // 0x10
};

static inline UnkCollider_l70419c* Get(UnkColliderRef_l70419c* ref) {
    return ref ? ref->collider : nullptr;
}

static inline void Release(UnkColliderRef_l70419c* ref) {
    if (ref && --ref->ref_count == 0)
        ref->Destroy();
}

// One contact the tree query reports.
struct __attribute__((packed)) UnkContact_l70419c {
    UnkColliderRef_l70419c* ref;
    UnkColliderRef_l70419c* other;
    u32 handle;        // 0x10
    CVector3D point;   // 0x14
    u8 pad_0x20[0xc];
    CVector3D normal;  // 0x2c
    u8 pad_0x38[0x4];
    u8 flags;  // 0x3c
    u8 pad_0x3d[0xb];
};
static_assert(sizeof(UnkContact_l70419c) == 0x48, "UnkContact_l70419c layout");

// Up to ten contacts on the stack.
struct __attribute__((packed)) UnkContactList_l70419c {
    void** vtable;
    u32 count;
    UnkContact_l70419c contacts[10];

    UnkContactList_l70419c() : vtable(g_unk_7101a6ba70 + 2), count(0) {}
    UnkContact_l70419c* begin() { return contacts; }
    UnkContact_l70419c* end() { return begin() + count; }
    ~UnkContactList_l70419c() {
        vtable = g_unk_7101a6ba70 + 2;
        for (u64 i = 0; i < count; i++) {
            Release(contacts[i].other);
            Release(contacts[i].ref);
        }
        count = 0;
    }
};

// A ray query: the nearest hit along a segment.
struct UnkRayHit_l70419c {
    CVector3D point;
    CVector3D normal;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;

    UnkRayHit_l70419c() {
        point.x = 0.0f;
        point.y = 0.0f;
        point.z = 0.0f;
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
        field_0x18 = 0.0f;
        field_0x1c = 0.0f;
        field_0x20 = 0.0f;
        field_0x24 = 0.0f;
        field_0x28 = 0.0f;
    }
};

struct UnkQueryBase_l70419c {
    void** vtable;
    s32 field_0x8;
    UnkRayHit_l70419c nearest;  // 0xc

    UnkQueryBase_l70419c() : field_0x8(-1), nearest() {}
};

struct UnkRayQuery_l70419c : UnkQueryBase_l70419c {
    bool hit;  // 0x38
    u8 pad_0x39[0x40 - 0x39];
    CVector3D origin;  // 0x40
    u8 pad_0x4c[0x4];
    u64 field_0x50;

    UnkRayQuery_l70419c(const CVector3D& from)
        : hit(false), origin(from), field_0x50(0) {
        vtable = g_unk_7101a23080 + 2;
    }
};

struct UnkRaySegment_l70419c {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkRaySegment_l70419c(CVector2D from, CVector2D to) : start(from), end(to) {
        delta.x = to.x - from.x;
        delta.y = to.y - from.y;
    }
};

struct UnkOwner_l70419c {
    u8 pad_0x0[0x38];
    UnkTransform_l70419c transform;  // 0x38
    u8 pad_0x68[0x2a0 - 0x68];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkSelf_l70419c {
    u8 pad_0x0[0x30];
    UnkOwner_l70419c* owner;  // 0x30
};

inline const CStrId& ShapeId_l70419c() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return reinterpret_cast<const CStrId*>(&s_table.field_0x20f0)[0x20];
}

static inline UnkShapeTester_A5_b68* Tester() {
    UnkShapeTester_A5_b68* t = g_unk_71726a47a0;
    if (!t) {
        t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return t;
}

static inline f32 Length(f32 x, f32 y) {
    return sqrtf(x * x + y * y);
}

static inline CVector2D Flat(const CVector3D& v) {
    CVector2D flat;
    fn_710007b430(&flat, &v);
    return flat;
}

// The flat direction from a contact to the owner, or the contact's own normal when it has one.
static inline void GetDirection(CVector2D* dir, UnkSelf_l70419c* self, const UnkContact_l70419c* contact) {
    if (contact->flags & 0x10) {
        CVector2D normal;
        fn_710007b430(&normal, &contact->normal);
        *dir = normal;
        return;
    }
    CVector3D owner = self->owner->transform.Translation();
    CVector3D away;
    away.x = owner.x - contact->point.x;
    away.y = owner.y - contact->point.y;
    away.z = owner.z - contact->point.z;
    CVector2D flat;
    fn_710007b430(&flat, &away);
    CVector2D v = flat;
    f32 length = Length(v.x, v.y);
    if (length != 0.0f) {
        v.x = v.x / length;
        v.y = v.y / length;
    }
    *dir = v;
}

static inline void Accumulate(CVector3D* push, const CVector2D& dir, f32 depth, f32 scale) {
    CVector3D step;
    step.x = depth * dir.x * scale;
    step.y = depth * dir.y * scale;
    step.z = 0.0f * scale;
    push->x = push->x + step.x;
    push->y = push->y + step.y;
    push->z = push->z + step.z;
}

void fn_710070419c(UnkSelf_l70419c* self, const CVector2D* pos, u32 mask, CVector3D* push, f32 scale,
                   f32 strength) {
    UnkCollisionContainer* collision = self->owner->collision;
    if (!collision)
        return;
    UnkCollisionObject* object = fn_71008640a4(collision, &ShapeId_l70419c());
    if (!object)
        return;
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    UnkTree* tree = sub ? static_cast<UnkTree*>(sub->field_0x820) : nullptr;
    UnkContactList_l70419c contacts;
    if (!fn_7100d5bfbc(tree, mask, reinterpret_cast<UnkBvhCollider*>(object), &contacts, 0))
        return;
    UnkCircle_l70419c* circle =
        As<UnkCircle_l70419c, 2>(reinterpret_cast<UnkCollider_l70419c*>(object)->shape);
    if (!circle)
        return;
    f32 radius = circle->radius;
    for (UnkContact_l70419c* contact = contacts.begin(); contact != contacts.end(); contact++) {
        UnkPolygons_l70419c* polygons = As<UnkPolygons_l70419c, 0x10>(contact->ref->collider->shape);
        if (polygons) {
            if (!fn_710012431c(reinterpret_cast<UnkPool_0x48*>(polygons->polygons), contact->handle))
                continue;
            UnkSegment2D edge = fn_710012436c(
                reinterpret_cast<const UnkPolygonCollection2D*>(polygons->polygons), contact->handle);
            UnkUnit110Vec2 nearest = fn_71001261e8(reinterpret_cast<const UnkUnit110Segment*>(&edge),
                                                   reinterpret_cast<const UnkUnit110Vec2*>(pos));
            f32 distance = Length(nearest.x - pos->x, nearest.y - pos->y);
            CVector2D dir;
            GetDirection(&dir, self, contact);
            f32 depth = (radius - distance) * strength / radius;
            Accumulate(push, dir, depth, scale);
        } else if (contact->ref->collider->container != collision) {
            CVector2D dir;
            GetDirection(&dir, self, contact);
            CVector2D target = Flat(contact->ref->collider->GetTransform()->Translation());
            UnkRaySegment_l70419c ray(*pos, target);
            UnkRayQuery_l70419c query(self->owner->transform.Translation());
            UnkShapeTester_A5_b68* tester = Tester();
            UnkCollider_l70419c* collider = Get(contact->ref);
            fn_7100d56604(tester, &ray, collider->shape, fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider)),
                          reinterpret_cast<UnkShapeFilter_590ac*>(&query), 2);
            f32 distance = 10.0f;
            if (query.hit) {
                CVector2D hit = Flat(query.nearest.point);
                distance = Length(hit.x - pos->x, hit.y - pos->y);
            }
            f32 depth = (radius - distance) * strength / radius;
            Accumulate(push, dir, depth, scale);
        }
    }
}
