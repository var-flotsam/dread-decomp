// CGooplotJumpAttack unit: sweeps a 32-unit circle from the attack's position towards its
// owner's and, when the collision tracker reports a hit, copies the hit out. Names and layouts
// are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);

// A reference to a counted object, released when the count drops to zero.
struct UnkHitRef_l59562c {
    base::UnkRefCounted* ptr;

    UnkHitRef_l59562c() : ptr(nullptr) {}
    ~UnkHitRef_l59562c() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRef_l59562c& operator=(const UnkHitRef_l59562c& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
};

// A vector copied component by component.
struct UnkHitVec_l59562c {
    f32 x, y, z;

    UnkHitVec_l59562c(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkHitVec_l59562c& operator=(const UnkHitVec_l59562c& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, three vectors and
// three words.
struct UnkCollisionHit_l59562c {
    UnkHitRef_l59562c object;
    UnkHitRef_l59562c field_0x8;
    s32 id;
    UnkHitVec_l59562c point;
    UnkHitVec_l59562c field_0x20;
    UnkHitVec_l59562c normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l59562c()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
    UnkCollisionHit_l59562c& operator=(const UnkCollisionHit_l59562c& o) {
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
static_assert(sizeof(UnkCollisionHit_l59562c) == 0x48, "UnkCollisionHit_l59562c layout");

// The query's callback object: the closest hit so far and the sweep's origin.
struct UnkHitQuery_l59562c {
    void** vtable;
    UnkCollisionHit_l59562c hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkHitQuery_l59562c(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin = o;
        field_0x68 = 0;
    }
    ~UnkHitQuery_l59562c() { vtable = g_unk_7101a20ed8 + 2; }
};

// A transform that only translates.
struct UnkTranslation_l59562c {
    f32 m[16];

    explicit UnkTranslation_l59562c(const CVector3D& t) {
        for (int i = 0; i < 16; ++i)
            m[i] = 0.0f;
        m[15] = 1.0f;
        m[10] = 1.0f;
        m[5] = 1.0f;
        m[0] = 1.0f;
        f32 x = t.x, y = t.y, z = t.z;
        m[3] = x;
        m[7] = y;
        m[11] = z;
    }
};

// A circle collision shape built and destroyed in place.
struct UnkCircleShape_l59562c {
    u8 data[0xb0];

    UnkCircleShape_l59562c() { fn_7100d61664(this); }
    ~UnkCircleShape_l59562c() { fn_7100d61750(reinterpret_cast<UnkLavaDropsShape_A2_b71*>(this)); }
    void SetRadius(f32 radius) { fn_7100d618f8(reinterpret_cast<UnkSphereShape_ovb0135*>(this), radius); }
};

struct UnkJumpOwner_l59562c {
    u8 pad_0x0[0x38];
    f32 transform[12];

    CVector3D GetPosition() const {
        CVector3D p;
        p.x = transform[3];
        p.y = transform[7];
        p.z = transform[11];
        return p;
    }
};

struct UnkGooplotJumpAttack_l59562c {
    u8 pad_0x0[0x8];
    UnkJumpOwner_l59562c* owner;
    u8 pad_0x10[0x60];
    CVector3D position;
};
static_assert(__builtin_offsetof(UnkGooplotJumpAttack_l59562c, position) == 0x70, "layout");

bool fn_710059562c(UnkGooplotJumpAttack_l59562c* self, UnkCollisionHit_l59562c* out) {
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    if (!sub)
        return false;
    UnkTree* tree = static_cast<UnkTree*>(sub->field_0x820);
    if (!tree)
        return false;

    UnkTranslation_l59562c xf(self->position);
    UnkCircleShape_l59562c shape;
    shape.SetRadius(32.0f);
    CVector2D from;
    fn_710007b430(&from, &self->position);
    CVector2D to;
    {
        CVector3D target = self->owner->GetPosition();
        fn_710007b430(&to, &target);
    }
    CVector2D delta;
    delta.x = to.x - from.x;
    delta.y = to.y - from.y;

    UnkHitQuery_l59562c query(self->position);
    if (fn_7100d5babc(tree, 0x80409, reinterpret_cast<UnkShape*>(&shape), reinterpret_cast<const UnkMtx44*>(&xf),
                      reinterpret_cast<const Vec2*>(&delta), &query, false)) {
        *out = query.hit;
        return true;
    }
    return false;
}
