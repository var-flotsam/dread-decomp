// CShakernautAIComponent (unit 1047): whether the way to a target is clear. Builds a frame facing the
// target, asks fn_71006afc28 for a pose, then sweeps the shape from 50 units behind that pose and reports
// clear when nothing is hit or the hit belongs to the owner itself. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkCollisionMask_af8ec {
    u8 pad_0x0[0x3cc];
    u32 mask;
};

struct UnkShakernaut_af8ec {
    u8 pad_0x0[0x30];
    UnkActor* actor;
};

struct UnkCollider_af8ec {
    virtual const base::reflection::CClass* vf_0x0();
    u8 pad_0x8[0x28];
    UnkActor* owner;
};
static_assert(__builtin_offsetof(UnkCollider_af8ec, owner) == 0x30, "owner");

struct UnkColliderLink_af8ec {
    u8 pad_0x0[0x10];
    UnkCollider_af8ec* collider;
};

struct UnkHitRef_af8ec {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Destroy();
    u32 refCount;
    UnkColliderLink_af8ec* link;
};

struct UnkHitRefPtr_af8ec {
    UnkHitRef_af8ec* ptr;

    UnkHitRefPtr_af8ec() : ptr(nullptr) {}
    ~UnkHitRefPtr_af8ec() {
        if (ptr && --ptr->refCount == 0)
            ptr->Destroy();
    }
};

extern void* g_unk_7101a23590[];

// The sweep's hit report.
struct UnkHit_af8ec {
    void** vtable;
    UnkHitRefPtr_af8ec object;
    UnkHitRefPtr_af8ec part;
    u32 index;
    CVector3D point;
    CVector3D normal;
    CVector3D field_0x34;
    CVector3D field_0x40;
    u32 field_0x4c;  // unset in the original's constructor
    bool found;

    UnkHit_af8ec()
        : index(-1),
          point{0.0f, 0.0f, 0.0f},
          normal{0.0f, 0.0f, 0.0f},
          field_0x34{0.0f, 0.0f, 0.0f},
          field_0x40{0.0f, 0.0f, 0.0f},
          found(false) {
        vtable = g_unk_7101a23590 + 2;
    }
    ~UnkHit_af8ec() { vtable = g_unk_7101a20ed8 + 2; }

    // The collider that was hit, when the hit object is one.
    UnkCollider_af8ec* GetCollider() const {
        UnkHitRef_af8ec* ref = object.ptr;
        UnkColliderLink_af8ec* link = ref ? ref->link : nullptr;
        UnkCollider_af8ec* collider = link ? link->collider : nullptr;
        if (collider && fn_710009d50c(collider->vf_0x0(), fn_710085fc44()))
            return collider;
        return nullptr;
    }
};
static_assert(__builtin_offsetof(UnkHit_af8ec, found) == 0x50, "found");

__attribute__((visibility("hidden"))) extern UnkVec3f g_unk_710164a698;
__attribute__((visibility("hidden"))) extern UnkVec3f g_unk_710164a6a4;

void fn_71000769f0(UnkMatrix44* out, const void* a, const void* b, const void* c, const void* d);
bool SweepShapeAgainstCollisionTree(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);
void fn_71006afc28(UnkShakernaut_af8ec* self, const UnkMatrix44* frame, const CVector3D* target, f32* angle,
                   UnkMatrix44* pose, bool* reachable);

inline UnkVec3f Cross_af8ec(const UnkVec3f& a, const UnkVec3f& b) {
    UnkVec3f r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

inline UnkMatrix44 Identity_af8ec() {
    UnkMatrix44 m = {{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    }};
    return m;
}

bool fn_71006af8ec(UnkShakernaut_af8ec* self, const CVector3D* from, const CVector3D* to, UnkShape* shape,
                   bool* reachable) {
    UnkActor* actor = fn_7100d7c990(self->actor);
    UnkGameManagerSub_0x608* world = g_unk_7101d47310->field_0x608;
    if (!world)
        return true;
    UnkTree* tree = static_cast<UnkTree*>(world->field_0x820);
    if (!tree)
        return true;

    UnkVec3f forward = to->x > from->x ? g_unk_710164a698 : g_unk_710164a6a4;
    UnkVec3f up(0.0f, 1.0f, 0.0f);
    UnkVec3f side = Cross_af8ec(up, forward);
    UnkMatrix44 frame;
    fn_71000769f0(&frame, &side, &up, &forward, from);

    f32 angle;
    UnkMatrix44 pose = Identity_af8ec();
    fn_71006afc28(self, &frame, to, &angle, &pose, reachable);
    if (!*reachable)
        return false;

    CVector3D start;
    start.x = pose.m[3] - pose.m[0] * 50.0f;
    start.y = pose.m[7] - pose.m[4] * 50.0f;
    start.z = pose.m[11] - pose.m[8] * 50.0f;
    CVector2D delta;
    {
        CVector3D offset;
        offset.x = to->x - start.x;
        offset.y = to->y - start.y;
        offset.z = to->z - start.z;
        fn_710007b430(&delta, &offset);
    }
    UnkMatrix44 xf = Identity_af8ec();
    xf.m[3] = start.x;
    xf.m[11] = start.z;
    xf.m[7] = start.y;

    UnkHit_af8ec hit;
    u32 mask = static_cast<UnkCollisionMask_af8ec*>(self->actor->field_0x1f8)->mask;
    if (!SweepShapeAgainstCollisionTree(tree, mask, shape, reinterpret_cast<const UnkMtx44*>(&xf), reinterpret_cast<const Vec2*>(&delta),
                       &hit, false))
        return true;
    UnkCollider_af8ec* collider = hit.GetCollider();
    UnkActor* owner = collider ? collider->owner : nullptr;
    return owner == actor;
}
