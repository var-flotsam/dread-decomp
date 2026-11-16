// CChozoRobotSoldierCannonShotAttack's unit (tools/tu_map.py unit 680): picks the shot's aim point
// (fn_710046514c), builds the cannon's frame facing the target, sweeps a 20-unit shape from 50 behind the
// muzzle to the aim point and accepts the point when nothing but the owner's target is in the way.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/UnkBvhTree.h"
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

extern void* g_unk_7101a23590[];

// The sweep's callback object: the first hit. The vtables (reached through the GOT) are the
// callback's and its base's.
struct UnkCannonSweepQuery_l464be8 {
    void** vtable;
    UnkCollisionHit_ovb0198u hit;
    bool field_0x50;

    UnkCannonSweepQuery_l464be8() {
        vtable = g_unk_7101a23590 + 2;
        field_0x50 = false;
    }
    ~UnkCannonSweepQuery_l464be8() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkLavaDropsShape_A2_b71;
void fn_7100d6169c(UnkLavaDropsShape_A2_b71* self, const void* source, f32 size);
void fn_7100d61750(UnkLavaDropsShape_A2_b71* self);

struct UnkLavaDropsShape_A2_b71 {
    u8 pad_0x0[0x8];
    u8 body[0xa8];

    UnkLavaDropsShape_A2_b71(const void* source, f32 size) { fn_7100d6169c(this, source, size); }
    ~UnkLavaDropsShape_A2_b71() { fn_7100d61750(this); }
};

struct UnkSelf;
struct UnkVec3;
bool fn_710046514c(UnkSelf* self, UnkVec3* out);
void fn_71000769f0(UnkMatrix44* out, const void* a, const void* b, const void* c, const void* d);
bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg,
                   bool flag);
struct UnkCannonShotAttack_l464be8;
bool fn_7100466d54(UnkCannonShotAttack_l464be8* self, const UnkMatrix44* frame, const CVector3D* aim, f32* out,
                   UnkMatrix44* muzzle, bool flag);

__attribute__((visibility("hidden"))) extern const CVector3D g_unk_7101649770;  // facing when the aim is ahead
__attribute__((visibility("hidden"))) extern const CVector3D g_unk_710164977c;  // facing otherwise
__attribute__((visibility("hidden"))) extern const u8 g_unk_7101649758[];      // the swept shape's source

struct UnkCannonOwnerData_l464be8 {
    u8 pad_0x0[0x3cc];
    u32 collision_mask;  // 0x3cc
};

struct UnkCannonOwner_l464be8 {
    u8 pad_0x0[0x1f8];
    UnkCannonOwnerData_l464be8* data;  // 0x1f8
};

struct UnkCannonShotAttack_l464be8 {
    u8 pad_0x0[0x8];
    UnkCannonOwner_l464be8* owner;  // 0x8
};

struct UnkCannonCollider_l464be8 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x28];
    UnkActor* actor;  // 0x30
};

struct UnkCannonHitEntity_l464be8 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* component;  // 0x10
};

struct UnkCannonHitObject_l464be8 {
    u8 pad_0x0[0x10];
    UnkCannonHitEntity_l464be8* entity;  // 0x10
};

static inline UnkMatrix44 Identity_l464be8() {
    UnkMatrix44 m;
    for (f32& v : m.m)
        v = 0.0f;
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    return m;
}

static inline CVector3D Sub_l464be8(const CVector3D& a, const CVector3D& b) {
    return CVector3D{a.x - b.x, a.y - b.y, a.z - b.z};
}

bool fn_7100464be8(UnkCannonShotAttack_l464be8* self, const CVector3D* target, CVector3D* out) {
    CVector3D aim;
    aim.x = 0.0f;
    aim.y = 0.0f;
    aim.z = 0.0f;
    if (!fn_710046514c(reinterpret_cast<UnkSelf*>(self), reinterpret_cast<UnkVec3*>(&aim)))
        return false;
    UnkActor* actor = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    UnkTree* tree = sub ? static_cast<UnkTree*>(sub->field_0x820) : nullptr;
    if (tree) {
        const CVector3D& facing = aim.x > target->x ? g_unk_7101649770 : g_unk_710164977c;
        CVector3D front;
        front.x = facing.x;
        front.y = facing.y;
        front.z = facing.z;
        CVector3D up;
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        CVector3D side = {up.y * front.z - up.z * front.y, up.z * front.x - up.x * front.z,
                          up.x * front.y - up.y * front.x};
        UnkMatrix44 frame;
        fn_71000769f0(&frame, &side, &up, &front, target);
        f32 angle;
        UnkMatrix44 muzzle = Identity_l464be8();
        fn_7100466d54(self, &frame, &aim, &angle, &muzzle, false);

        UnkLavaDropsShape_A2_b71 shape(g_unk_7101649758, 20.0f);
        CVector3D from = {muzzle.m[3] - muzzle.m[0] * 50.0f, muzzle.m[7] - muzzle.m[4] * 50.0f,
                          muzzle.m[11] - muzzle.m[8] * 50.0f};
        CVector2D delta;
        {
            CVector3D move = Sub_l464be8(aim, from);
            fn_710007b430(&delta, &move);
        }
        UnkMatrix44 start = Identity_l464be8();
        start.m[3] = from.x;
        start.m[7] = from.y;
        start.m[11] = from.z;
        UnkCannonSweepQuery_l464be8 query;
        bool blocked = false;
        if (fn_7100d5babc(tree, self->owner->data->collision_mask, reinterpret_cast<UnkShape*>(&shape),
                          reinterpret_cast<const UnkMtx44*>(&start), reinterpret_cast<const Vec2*>(&delta), &query,
                          false)) {
            UnkActor* hit = nullptr;
            UnkCannonHitObject_l464be8* object = reinterpret_cast<UnkCannonHitObject_l464be8*>(query.hit.object.ptr);
            if (object && object->entity) {
                base::reflection::UnkReflObject* component = object->entity->component;
                if (component && fn_710009d50c(component->vf_0x0(), fn_710085fc44()))
                    hit = static_cast<UnkCannonCollider_l464be8*>(component)->actor;
            }
            blocked = hit != actor;
        }
        if (blocked)
            return false;
    }
    if (out)
        *out = aim;
    return true;
}
