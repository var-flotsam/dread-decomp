// CCharacterMovement (unit 1445): whether a shape, shrunk by `offset` for the query, overlaps anything in
// the collision tracker at `pos`. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CCharacterMovement.h"
#include "game/UnkCollisionShape.h"
#include "game/UnkGameManager.h"
#include "game/UnkMatrix44_7100078c9c.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU54.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "types.h"

// A shape's bounds: its two corners.
struct UnkShapeBounds_l976c94 {
    base::math::CVector3D min;
    base::math::CVector3D max;
};

// The shape queried: its +0x8 is its transform; slot 0x28 gives its bounds under a transform, slot
// 0x30 scales it.
struct UnkQueryShape_l976c94 {
    void** vtable;
    u8 field_0x8[1];

    UnkShapeBounds_l976c94 GetBounds(const void* xf) {
        return reinterpret_cast<UnkShapeBounds_l976c94 (*)(UnkQueryShape_l976c94*, const void*)>(
            vtable[0x28 / 8])(this, xf);
    }
    void Scale(f32 factor) {
        reinterpret_cast<void (*)(UnkQueryShape_l976c94*, f32)>(vtable[0x30 / 8])(this, factor);
    }
};

struct UnkShapeOwner_l976c94 {
    u8 pad_0x0[0x158];
    UnkQueryShape_l976c94* field_0x158;
};

struct UnkMover_l976c94 {
    u8 pad_0x0[0x3b8];
    UnkShapeOwner_l976c94* field_0x3b8;
    UnkShapeOwner_l976c94* field_0x3c0;
};

struct UnkBox2D_l976c94 {
    base::math::CVector2D min;
    base::math::CVector2D max;
};
// A collision shape (0xc0 bytes) built from a 2D box.
struct UnkBox2D {
    f32 min_x, min_y, max_x, max_y;

    UnkBox2D(f32 min_x_, f32 min_y_, f32 max_x_, f32 max_y_)
        : min_x(min_x_), min_y(min_y_), max_x(max_x_), max_y(max_y_) {}
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
bool fn_7100976c94(UnkGrappleTarget* target, const UnkVec3f* pos, u32 value, void* arg, u32 flag, f32 offset) {
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    if (!sub)
        return false;
    void* tracker = sub->field_0x820;

    UnkQueryShape_l976c94* shape = static_cast<UnkQueryShape_l976c94*>(arg);
    if (!shape) {
        UnkMover_l976c94* mover = reinterpret_cast<UnkMover_l976c94*>(target);
        UnkShapeOwner_l976c94* owner = mover->field_0x3c0 ? mover->field_0x3c0 : mover->field_0x3b8;
        shape = owner ? owner->field_0x158 : nullptr;
    }

    UnkShapeBounds_l976c94 bounds = shape->GetBounds(shape->field_0x8);
    UnkCollisionShape area;
    {
        UnkBox2D_l976c94 box;
        fn_710007b430(&box.min, &bounds.min);
        fn_710007b430(&box.max, &bounds.max);
        fn_7100d5e1f8(&area, reinterpret_cast<const UnkBox2D*>(&box));
    }

    f32 scale = 1.0f - offset;
    shape->Scale(scale);
    bool hit;
    {
        UnkMatrix44_7100078c9c xf = fn_7100078cd8(*pos);
        hit = fn_7100d5b484(static_cast<UnkTree*>(tracker), value, reinterpret_cast<UnkShape*>(shape),
                            reinterpret_cast<const UnkMtx44*>(&xf), nullptr, flag);
    }
    shape->Scale(1.0f / scale);
    fn_7100d5e2c8(&area);
    return hit;
}
