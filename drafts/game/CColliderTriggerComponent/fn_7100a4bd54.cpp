// CColliderTriggerComponent unit (tools/tu_map.py unit 1564): collects the entities the trigger's collider
// overlaps, either through the collision tree or, against the player only, by testing the player's
// colliders one by one. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CDroppableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCollisionObject;
bool fn_7100d5bfbc(void* tracker, u32 mask, UnkCollisionObject* object, void* arg, u32);

struct UnkCollisionShape_l864374;
struct UnkCollisionRecord_l864374 {
    u8 unk_0x0[0x18];
    bool dirty;  // 0x18
    u8 unk_0x19[0xf];
    UnkCollisionShape_l864374* object;  // 0x28
};
struct UnkCollisionContainer_l864374;
void fn_7100864374(UnkCollisionContainer_l864374* self, u32 mask,
                   base::global::CRntVector<UnkCollisionRecord_l864374*>* out, bool all);

// The records fn_7100864374 fills (a shared scratch vector).
extern base::global::CRntVector<UnkCollisionRecord_l864374*> g_unk_717274b2e8;
extern void* g_unk_7101a8f230[];
extern void* g_unk_7101a22c60[];

// A collider: its mask, its 2D bounds (refreshed when dirty) and the actor that owns it.
struct UnkBounds_a4bd54 {
    f32 x0, y0, x1, y1;
};

struct UnkTriggerCollider_a4bd54 {
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
    virtual u32 vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    u8 pad_0x8[0x38];
    UnkBounds_a4bd54 bounds;  // 0x40
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60
    u8 pad_0x61[0x158 - 0x61];
    void* owner;  // 0x158
};

static inline const UnkBounds_a4bd54& Bounds(UnkTriggerCollider_a4bd54* c) {
    if (c->dirty)
        c->vf_0x98();
    return c->bounds;
}

// What the shape tester reports for a hit (vtable g_unk_7101a22c60): the contact point first.
struct UnkTriggerContact_a4bd54 {
    void** vtable;
    s32 field_0x8;
    base::math::CVector3D point;   // 0xc
    base::math::CVector3D normal;  // 0x18
    u32 field_0x24[5];

    UnkTriggerContact_a4bd54()
        : vtable(g_unk_7101a22c60 + 2), field_0x8(-1), point(), normal(), field_0x24() {}
};

// The hits of a tree query: up to 32 records of 0x48 bytes after the count, each starting with
// two reference-counted pointers. The first leads to the entity hit.
struct UnkTriggerHitOwner_a4bd54 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* entity;  // 0x10
};
struct UnkTriggerHitShape_a4bd54 : UnkRefObject {
    UnkTriggerHitOwner_a4bd54* owner;  // 0x10
};

#pragma pack(push, 4)
struct UnkTriggerHitRecord_a4bd54 {
    UnkTriggerHitShape_a4bd54* shape;
    UnkRefObject* field_0x8;
    u32 field_0x10;
    base::math::CVector3D point;  // 0x14
    u8 pad_0x20[0x28];
};

static inline void ReleaseRef(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

struct UnkTriggerHitListBase_a4bd54 {
    void** vtable;
    u32 count;
    UnkTriggerHitRecord_a4bd54 items[32];

    UnkTriggerHitRecord_a4bd54* begin() { return items; }
    UnkTriggerHitRecord_a4bd54* end() { return items + count; }

    ~UnkTriggerHitListBase_a4bd54() {
        vtable = g_unk_7101a70eb0 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef(items[i].field_0x8);
            ReleaseRef(items[i].shape);
        }
    }
};

struct UnkTriggerHitList_a4bd54 : UnkTriggerHitListBase_a4bd54 {
    UnkTriggerHitList_a4bd54() {
        count = 0;
        vtable = g_unk_7101a8f230 + 2;
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkTriggerHitRecord_a4bd54) == 0x48, "record");

// The output: entities with the point where each was hit.
struct UnkTriggerOverlap_a4bd54 {
    void* entity;
    base::math::CVector3D point;
};

#pragma pack(push, 4)
struct UnkTriggerOverlapList_a4bd54 {
    u32 count;
    UnkTriggerOverlap_a4bd54 items[32];

    void Push(void* entity, const base::math::CVector3D& point) {
        items[count].entity = entity;
        items[count].point = point;
        ++count;
    }
};
#pragma pack(pop)

// The player as seen here: whether it has a collision body (+0x1a8) and its colliders (+0x2a0).
struct UnkTriggerPlayer_a4bd54 {
    u8 pad_0x0[0x1a8];
    void* body;  // 0x1a8
    u8 pad_0x1b0[0x2a0 - 0x1b0];
    UnkCollisionContainer_l864374* colliders;  // 0x2a0
};

struct UnkColliderTrigger_a4bd54 {
    void** vtable;
    u8 pad_0x8[0x48];
    u32 mask;  // 0x50
    UnkTriggerCollider_a4bd54* collider;  // 0x58
    u8 pad_0x60[0x204 - 0x60];
    bool use_tree;  // 0x204

    // The point of the named locator, if the trigger has one (vtable slot 0x178).
    bool GetPoint(const base::global::CStrId& name, base::math::CVector3D* out) {
        return reinterpret_cast<bool (*)(UnkColliderTrigger_a4bd54*, const base::global::CStrId&,
                                         base::math::CVector3D*)>(vtable[0x178 / 8])(this, name, out);
    }
};

static inline UnkShapeTester_A5_b68* Tester() {
    UnkShapeTester_A5_b68* t = g_unk_71726a47a0;
    if (!t) {
        t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return t;
}

void fn_7100a4bd54(UnkColliderTrigger_a4bd54* self, UnkTriggerOverlapList_a4bd54* out) {
    if (!self->collider)
        return;
    UnkGameManager* gm = g_unk_7101d47310;
    void* tree = nullptr;
    if (self->use_tree) {
        tree = gm->field_0x608 ? gm->field_0x608->field_0x820 : nullptr;
    } else {
        UnkTriggerPlayer_a4bd54* player = reinterpret_cast<UnkTriggerPlayer_a4bd54*>(fn_7100dec788(gm));
        if (!player || !player->body)
            return;
        u32 mask = self->collider->vf_0x58();
        if (mask != 0) {
            g_unk_717274b2e8.Clear();
            fn_7100864374(player->colliders, mask, &g_unk_717274b2e8, false);
            for (UnkCollisionRecord_l864374** it = g_unk_717274b2e8.begin(); it != g_unk_717274b2e8.end(); ++it) {
                UnkTriggerCollider_a4bd54* other = reinterpret_cast<UnkTriggerCollider_a4bd54*>((*it)->object);
                UnkBounds_a4bd54 a = Bounds(self->collider);
                const UnkBounds_a4bd54& b = Bounds(other);
                if (!(b.x1 >= a.x0 && b.y0 <= a.y1 && b.y1 >= a.y0 && b.x0 <= a.x1))
                    continue;
                UnkTriggerContact_a4bd54 contact;
                UnkShapeTester_A5_b68* tester = Tester();
                void* ownerA = self->collider->owner;
                UnkMatrix44 ta = fn_7100d36a2c(reinterpret_cast<UnkShape*>(self->collider));
                UnkColliderResult result =
                    fn_7100d51368(tester, ownerA, &ta, other->owner, fn_7100d36a2c(reinterpret_cast<UnkShape*>(other)),
                                  reinterpret_cast<UnkShapeFilter_ovb0107*>(&contact), 0);
                if (result.bits & 1) {
                    out->Push(player, contact.point);
                    return;
                }
            }
        } else {
            UnkTriggerOverlap_a4bd54 overlap;
            overlap.point = base::math::CVector3D();
            if (!self->GetPoint(g_unk_7101d470e0, &overlap.point))
                return;
            overlap.entity = player;
            out->Push(overlap.entity, overlap.point);
        }
        return;
    }
    {
        UnkTriggerHitList_a4bd54 hits;
        if (self->mask)
            fn_7100d5bfbc(tree, self->mask, reinterpret_cast<UnkCollisionObject*>(self->collider), &hits, 0);
        for (UnkTriggerHitRecord_a4bd54* hit = hits.begin(); hit != hits.end(); ++hit) {
            base::reflection::UnkReflObject* entity = hit->shape->owner->entity;
            if (entity && fn_710009d50c(entity->vf_0x0(), fn_7100d70c20()))
                out->Push(entity, hit->point);
        }
    }
}
