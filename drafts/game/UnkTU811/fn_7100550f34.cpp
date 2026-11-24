// UnkTU811 (CEmmyAIComponent): which sides of the owner are free. Queries the collision tree with the owner's
// bounds (scaled 2 by 0.9) under its collision transform and clears a side for each wall contact found there;
// contacts of the owner's own collision components and of smart objects are skipped. Names and layouts are
// placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU341.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

extern CStrId g_unk_71726e1f70 __attribute__((visibility("hidden")));
extern void* g_unk_7101a48c48[];

struct UnkScaledBoxShape2D;
void CAABoxShape2D_ScaleAboutCentreByVector2(UnkScaledBoxShape2D* self, const f32* scale);

// The sides, a two-bit set whose statics are guarded (their constructors are not constexpr).
struct UnkSides_l550f34 {
    u32 value;
    UnkSides_l550f34() { value = 0; }
    explicit UnkSides_l550f34(u32 v) { value = v; }
    UnkSides_l550f34(const UnkSides_l550f34& other) : value(other.value) {}
    UnkSides_l550f34& operator=(const UnkSides_l550f34& other) {
        value = other.value;
        return *this;
    }
};

inline const UnkSides_l550f34& NoSides_l550f34() {
    static UnkSides_l550f34 s_value asm("g_unk_71726e1f10");
    return s_value;
}

inline const UnkSides_l550f34& AllSides_l550f34() {
    static UnkSides_l550f34 s_value asm("g_unk_71726e1f00")(~NoSides_l550f34().value & 3);
    return s_value;
}

struct UnkAABB_l550f34 {
    CVector3D min;
    CVector3D max;
};

struct UnkBox2D_l550f34 {
    CVector2D min;
    CVector2D max;
    explicit UnkBox2D_l550f34(const UnkAABB_l550f34& box) {
        fn_710007b430(&min, &box.min);
        fn_710007b430(&max, &box.max);
    }
};

// A 2D box shape (CAABoxShape2D) over a box.
struct UnkBoxShape2D_l550f34 {
    u8 body[0xc0];
    explicit UnkBoxShape2D_l550f34(const UnkBox2D_l550f34& box) {
        fn_7100d5e1f8(reinterpret_cast<UnkCollisionShape*>(this), reinterpret_cast<const UnkBox2D*>(&box));
    }
    ~UnkBoxShape2D_l550f34() { fn_7100d5e2c8(reinterpret_cast<UnkCollisionShape*>(this)); }
};

struct UnkBoundsShape_l550f34 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkAABB_l550f34 vf_0x28(void* transform);
    u8 transform[0x8];  // 0x8
};

struct UnkCollider_l550f34 {
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
    virtual const UnkMatrix44& vf_0x70();
    u8 pad_0x8[0x158 - 0x8];
    UnkBoundsShape_l550f34* shape;  // 0x158
};

struct UnkCollisionComp_l550f34 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x28];
    void* owner;  // 0x30
};

struct UnkHitTarget_l550f34 {
    u8 pad_0x0[0x10];
    UnkCollisionComp_l550f34* component;  // 0x10
};

struct UnkHitHandle_l550f34 : UnkRefObject {
    UnkHitTarget_l550f34* target;  // 0x10
};

static inline void ReleaseRef_l550f34(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

#pragma pack(push, 4)
struct UnkHit_l550f34 {
    UnkHitHandle_l550f34* first;
    UnkRefObject* second;
    u8 pad_0x10[0x4];
    CVector3D point;  // 0x14
    u8 pad_0x20[0xc];
    CVector3D normal;  // 0x2c
    u8 pad_0x38[0x10];
};

// The hits of one query.
struct UnkHitList_l550f34 {
    void** vtable;
    u32 count;
    u8 storage[8 * sizeof(UnkHit_l550f34)];  // 0xc

    UnkHitList_l550f34() : count(0) {}

    UnkHit_l550f34* items() { return reinterpret_cast<UnkHit_l550f34*>(storage); }

    ~UnkHitList_l550f34() {
        vtable = g_unk_7101a23478 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef_l550f34(items()[i].second);
            ReleaseRef_l550f34(items()[i].first);
        }
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkHit_l550f34) == 0x48, "hit");

// The query's visitor (vtable g_unk_7101a48c48).
struct UnkHitVisitor_l550f34 : UnkHitList_l550f34 {
    UnkHitVisitor_l550f34() { vtable = g_unk_7101a48c48 + 2; }
};

struct UnkTreeHolder_l550f34 {
    u8 pad_0x0[0x820];
    UnkTree* tree;  // 0x820
};

struct UnkOwner_l550f34 {
    u8 pad_0x0[0x54];
    f32 y;  // 0x54
    u8 pad_0x58[0x1a8 - 0x58];
    UnkTreeHolder_l550f34* field_0x1a8;
    u8 pad_0x1b0[0x2a0 - 0x1b0];
    UnkCollisionContainer* collisions;  // 0x2a0
};

struct UnkTargetMovement_l550f34 {
    u8 pad_0x0[0x3cc];
    u32 collision_mask;  // 0x3cc
};

struct UnkEmmyAI_l550f34 {
    u8 pad_0x0[0x30];
    UnkOwner_l550f34* owner;  // 0x30
};

// The component when it is of the getter's class (or derives from it), else null.
template <class T>
inline T* Cast(T* object, const base::reflection::CClass* (*get_class)()) {
    if (!object)
        return nullptr;
    return fn_710009d50c(object->vf_0x0(), get_class()) ? object : nullptr;
}

static inline f32 Dot(const CVector3D& a, f32 x, f32 y, f32 z) {
    return a.x * x + a.y * y + a.z * z;
}

UnkSides_l550f34 fn_7100550f34(UnkEmmyAI_l550f34* self) {
    UnkSides_l550f34 sides(AllSides_l550f34());
    UnkHitVisitor_l550f34 hits;
    UnkTree* tree = self->owner->field_0x1a8->tree;
    auto* movement = static_cast<UnkTargetMovement_l550f34*>(
        fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))->field_0x208);
    if (!movement)
        return sides;

    auto* collider = reinterpret_cast<UnkCollider_l550f34*>(fn_71008640a4(self->owner->collisions, &g_unk_71726e1f70));
    UnkMatrix44 xf;
    UnkCopyMatrix(&xf, collider->vf_0x70());
    UnkBoundsShape_l550f34* bounds = collider->shape;
    UnkBoxShape2D_l550f34 shape(UnkBox2D_l550f34(bounds->vf_0x28(bounds->transform)));
    f32 scale[2] = {2.0f, 0.9f};
    CAABoxShape2D_ScaleAboutCentreByVector2(reinterpret_cast<UnkScaledBoxShape2D*>(&shape), scale);

    if (fn_7100d5b484(tree, movement->collision_mask, reinterpret_cast<UnkShape*>(&shape),
                      reinterpret_cast<const UnkMtx44*>(&xf), &hits, 0)) {
        bool below = false;
        bool above = false;
        for (UnkHit_l550f34* it = hits.items(); it != hits.items() + hits.count; ++it) {
            UnkCollisionComp_l550f34* component = it->first->target->component;
            UnkCollisionComp_l550f34* collision = Cast(component, fn_710085fc44);
            if (collision && collision->owner == self->owner)
                continue;
            static const CStrId s_smart_object("SMARTOBJECT", true);
            if (collision && collision->owner && fn_710023341c(collision->owner, s_smart_object))
                continue;

            const CVector3D& normal = it->normal;
            if (fabsf(Dot(normal, 1.0f, 0.0f, 0.0f)) > 0.98f) {
                // A wall: the side is blocked unless it was already seen from that side.
                if (normal.y > 0.0f ? below : above)
                    continue;
                sides = NoSides_l550f34();
            } else {
                f32 down = Dot(normal, 0.0f, -1.0f, 0.0f);
                if (down > 0.02f) {
                    above |= !(it->point.y < self->owner->y);
                    below |= it->point.y < self->owner->y;
                } else if (!(down < -0.98f)) {
                    sides.value &= ~(1u << (normal.y > 0.0f));
                }
            }
        }
    }
    return sides;
}
