// Unit 1428 (CMorphBallMovement's neighbours): tests a slightly shrunk copy of the ball's shape, placed
// back along a direction from a point, against the collision tree, and gives the closest hit.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector3D;

// A reference to a counted object, released when the count drops to zero.
struct UnkHitRef_l948b68 {
    base::UnkRefCounted* ptr;

    UnkHitRef_l948b68() : ptr(nullptr) {}
    UnkHitRef_l948b68(const UnkHitRef_l948b68& o) {
        ptr = nullptr;
        if (base::UnkRefCounted* p = o.ptr) {
            ++p->ref_count;
            ptr = p;
        }
    }
    ~UnkHitRef_l948b68() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkHitVec_l948b68 {
    f32 x, y, z;

    UnkHitVec_l948b68(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, two vectors
// and flags.
struct UnkHit_l948b68 {
    UnkHitRef_l948b68 object;
    UnkHitRef_l948b68 field_0x8;
    s32 id;
    UnkHitVec_l948b68 point;
    UnkHitVec_l948b68 field_0x20;
    UnkHitVec_l948b68 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkHit_l948b68()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkHit_l948b68) == 0x48, "UnkHit_l948b68 layout");

// The shape query's callback object: the closest hit so far and the origin it is measured from. The
// vtables (reached through the GOT) are the callback's and its base's.
struct UnkShapeQuery_l948b68 {
    void** vtable;
    UnkHit_l948b68 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkShapeQuery_l948b68(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkShapeQuery_l948b68() { vtable = g_unk_7101a20ed8 + 2; }
};

// A collision shape: vf_0x30 scales it, vf_0x10 deletes it.
struct UnkBallShape_l948b68 {
    void** vtable;

    void Scale(f32 factor) { reinterpret_cast<void (*)(UnkBallShape_l948b68*, f32)>(vtable[0x30 / 8])(this, factor); }
    void Delete() { reinterpret_cast<void (*)(UnkBallShape_l948b68*)>(vtable[0x10 / 8])(this); }
};

// The mover's collider: its kind (vf_0x18; 2 is a ball), a copy of its shape (vf_0x40), its radius.
struct UnkBallCollider_l948b68 {
    void** vtable;
    u8 pad_0x8[0xa8 - 0x8];
    f32 radius;  // +0xa8

    s32 GetKind() { return reinterpret_cast<s32 (*)(UnkBallCollider_l948b68*)>(vtable[0x18 / 8])(this); }
    UnkBallShape_l948b68* CloneShape() {
        return reinterpret_cast<UnkBallShape_l948b68* (*)(UnkBallCollider_l948b68*)>(vtable[0x40 / 8])(this);
    }
};

struct UnkBallMover_l948b68 {
    u8 pad_0x0[0x158];
    UnkBallCollider_l948b68* collider;
};

struct UnkBallActor_l948b68 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;

    CVector3D GetPosition() const { return CVector3D{transform.m[3], transform.m[7], transform.m[11]}; }
};

struct UnkBallConfig_l948b68 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkBallOwner_l948b68 {
    void** vtable;
    u8 pad_0x8[0x30 - 0x8];
    UnkBallActor_l948b68* actor;  // +0x30
    u8 pad_0x38[0x178 - 0x38];
    UnkBallConfig_l948b68* field_0x178;
    u8 pad_0x180[0x18c - 0x180];
    CVector3D center;  // +0x18c
    u8 pad_0x198[0x3c8 - 0x198];
    u32 mask;  // +0x3c8

    UnkBallMover_l948b68* GetMover() {
        return reinterpret_cast<UnkBallMover_l948b68* (*)(UnkBallOwner_l948b68*)>(vtable[0x2a8 / 8])(this);
    }
};

// The ray a probe follows: its start at +0x14 and its direction at +0x20.
struct UnkBallRay_l948b68 {
    u8 pad_0x0[0x14];
    CVector3D start;
    CVector3D dir;
};

UnkHit_l948b68 fn_7100948b68(UnkBallOwner_l948b68* self, const UnkBallRay_l948b68* ray) {
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    UnkTree* tree = sub ? static_cast<UnkTree*>(sub->field_0x820) : nullptr;

    UnkBallCollider_l948b68* any = self->GetMover()->collider;
    UnkBallCollider_l948b68* ball = any->GetKind() == 2 ? any : nullptr;
    UnkBallShape_l948b68* shape = ball->CloneShape();
    shape->Scale(0.97f);

    UnkMatrix44 xf;
    UnkCopyMatrix(&xf, fn_710002614c(&self->actor->transform));
    // The shape sits one radius back from the start of the ray.
    f32 back = ball->radius;
    f32 x = ray->start.x - back * ray->dir.x;
    f32 y = ray->start.y - back * ray->dir.y;
    f32 z = ray->start.z - back * ray->dir.z;
    xf.m[3] = x;
    xf.m[7] = y;
    xf.m[11] = z;

    CVector3D origin = self->field_0x178 && self->field_0x178->field_0x10 ? self->center : self->actor->GetPosition();
    UnkShapeQuery_l948b68 query(origin);
    fn_7100d5b484(tree, self->mask, reinterpret_cast<UnkShape*>(shape), reinterpret_cast<const UnkMtx44*>(&xf),
                  &query, 0);
    shape->Delete();
    return query.hit;
}
