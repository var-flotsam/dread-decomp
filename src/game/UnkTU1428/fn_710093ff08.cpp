// CMorphBallMovement's unit (hint only): drops the ball's collision shape from `pos` straight down by up to
// `distance` through the game manager's collision tree and gives where it stops (`pos` when nothing is in
// the way), whether it hit, and the hit. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/CAbilityShinespark.h"
#include "game/CCharacterMovement.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "game/logic/collision/UnkBvhTreeSweep.h"
#include "types.h"

using base::math::CVector2D;

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

// A reference to a counted object, released when the count drops to zero; assigned out of line.
struct UnkDropRef_l93ff08 {
    base::UnkRefCounted* ptr;

    UnkDropRef_l93ff08() : ptr(nullptr) {}
    ~UnkDropRef_l93ff08() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkDropRef_l93ff08& operator=(const UnkDropRef_l93ff08& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
};

struct UnkDropVec_l93ff08 {
    f32 x, y, z;

    UnkDropVec_l93ff08(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkDropVec_l93ff08& operator=(const UnkDropVec_l93ff08& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// A collision hit (0x48 bytes); `fraction` is how far along the sweep it lies.
struct UnkDropHit_l93ff08 {
    UnkDropRef_l93ff08 object;
    UnkDropRef_l93ff08 field_0x8;
    s32 id;
    UnkDropVec_l93ff08 point;
    UnkDropVec_l93ff08 field_0x20;
    UnkDropVec_l93ff08 normal;
    f32 fraction;  // 0x38
    u32 flags;
    u32 field_0x40;

    UnkDropHit_l93ff08()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), fraction(0),
          flags(0), field_0x40(0) {}
    UnkDropHit_l93ff08& operator=(const UnkDropHit_l93ff08& o) {
        if (this == &o)
            return *this;
        object = o.object;
        field_0x8 = o.field_0x8;
        id = o.id;
        point = o.point;
        field_0x20 = o.field_0x20;
        normal = o.normal;
        fraction = o.fraction;
        flags = o.flags;
        field_0x40 = o.field_0x40;
        return *this;
    }
};
static_assert(sizeof(UnkDropHit_l93ff08) == 0x48, "UnkDropHit_l93ff08 layout");

// The hit query the sweep fills.
struct UnkDropQuery_l93ff08 {
    void** vtable;
    UnkDropHit_l93ff08 hit;
    bool field_0x50;

    UnkDropQuery_l93ff08() {
        vtable = g_unk_7101a23590 + 2;
        field_0x50 = false;
    }
    ~UnkDropQuery_l93ff08() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkDropCollider_l93ff08 {
    u8 pad_0x0[0x158];
    UnkShape* shape;  // 0x158
};

struct UnkDropBall_l93ff08 {
    u8 pad_0x0[0x3b8];
    UnkDropCollider_l93ff08* collider;  // 0x3b8
    UnkDropCollider_l93ff08* override_collider;  // 0x3c0
    u32 mask;  // 0x3c8
};

UnkVec3f fn_710093ff08(UnkDropBall_l93ff08* self, const UnkVec3f* pos, f32 distance, bool* out_hit,
                       UnkDropHit_l93ff08* out_info) {
    UnkVec3f result = *pos;
    UnkGameManagerSub_0x608* collision = g_unk_7101d47310->field_0x608;
    if (!collision)
        return result;
    UnkTree* tree = static_cast<UnkTree*>(collision->field_0x820);

    UnkVec3f start = *pos;
    CVector2D sweep = {0.0f, -distance};
    UnkDropCollider_l93ff08* collider = self->override_collider ? self->override_collider : self->collider;
    UnkShape* shape = collider ? collider->shape : nullptr;

    UnkDropQuery_l93ff08 query;
    u32 mask = self->mask;
    UnkMatrix44_7100078c9c xf = fn_7100078cd8(start);
    bool hit = false;
    if (fn_7100d5babc(tree, mask, shape, reinterpret_cast<const UnkMtx44*>(&xf), reinterpret_cast<const Vec2*>(&sweep),
                      &query, false)) {
        result = start + UnkVec3f(sweep.x, sweep.y, 0.0f) * query.hit.fraction;
        hit = true;
        if (out_info)
            *out_info = query.hit;
    }
    if (out_hit)
        *out_hit = hit;
    return result;
}
