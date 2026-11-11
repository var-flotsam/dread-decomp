// CCharacterMovement unit (tools/tu_map.py, reflection hint). fn_7100977bec: how far the shape's
// bounds reach below its origin: the shape comes from +0x3c0, else +0x3b8, else the owner's virtual
// slot 0x2a8 (none: 0); its collider (+0x158) gives a 3D box (slot 0x28, with the collider's data at
// +0x8), projected to 2D with fn_710007b430; the result is 0 minus the 2D minimum's y. Names and
// layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CCharacterMovement.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkBox3D_b42 {
    CVector3D min;
    CVector3D max;
};

struct UnkBox2D_b42 {
    CVector2D min;
    CVector2D max;
};

struct UnkCollider_b42 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkBox3D_b42 vf_0x28(const void* data) const;
    u8 data[8];  // 0x8
};

struct UnkShape_b42 {
    u8 pad_0x0[0x158];
    UnkCollider_b42* collider;  // 0x158
};

struct UnkMovement_b42 {
    void** vtable;
    u8 pad_0x8[0x3b0];
    UnkShape_b42* shape_b;  // 0x3b8
    UnkShape_b42* shape_a;  // 0x3c0

    UnkShape_b42* GetShapeFromSlot() {
        typedef UnkShape_b42* (*Fn)(UnkMovement_b42*);
        return reinterpret_cast<Fn>(vtable[0x2a8 / 8])(this);
    }
};

// Declared as src/game/CEntity.cpp does.
struct UnkFlagged;

f32 fn_7100977bec(UnkFlagged* flagged) {
    UnkMovement_b42* self = reinterpret_cast<UnkMovement_b42*>(flagged);
    UnkShape_b42* shape = self->shape_a;
    if (!shape && !(shape = self->shape_b) && !(shape = self->GetShapeFromSlot()))
        return 0.0f;
    UnkCollider_b42* collider = shape->collider;
    UnkBox2D_b42 box2;
    UnkBox3D_b42 box = collider->vf_0x28(collider->data);
    fn_710007b430(&box2.min, &box.min);
    fn_710007b430(&box2.max, &box.max);
    return 0.0f - box2.min.y;
}
