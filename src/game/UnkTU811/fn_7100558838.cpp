// Unit 811 (CEmmyAIComponent hint): the 2D extent (max minus min) of the owner's collider, taken from the
// object registered in the owner's collision container under a name when asked for, else from the
// movement's shape (zero when there is none). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkBox3D_558838 {
    CVector3D min;
    CVector3D max;
};

struct UnkBox2D_558838 {
    CVector2D min;
    CVector2D max;
};

struct UnkCollider_558838 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkBox3D_558838 vf_0x28(const void* data) const;
    u8 data[8];  // 0x8
};

struct UnkShape_558838 {
    u8 pad_0x0[0x158];
    UnkCollider_558838* collider;  // 0x158
};

struct UnkMovement_558838 {
    void** vtable;

    UnkShape_558838* GetShape() {
        typedef UnkShape_558838* (*Fn)(UnkMovement_558838*);
        return reinterpret_cast<Fn>(vtable[0x2a8 / 8])(this);
    }
};

struct UnkOwner_558838 {
    u8 pad_0x0[0x218];
    UnkMovement_558838* movement;  // 0x218
    u8 pad_0x220[0x80];
    UnkCollisionContainer* container;  // 0x2a0
};

struct UnkEmmyAI_558838 {
    u8 pad_0x0[0x30];
    UnkOwner_558838* owner;  // 0x30
};

struct UnkExtent_558838 {
    f32 x, y;
    UnkExtent_558838() {}
    UnkExtent_558838(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkExtent_558838(const UnkExtent_558838& o) : x(o.x), y(o.y) {}
};

static inline UnkExtent_558838 ShapeExtent(UnkShape_558838* shape) {
    UnkCollider_558838* collider = shape->collider;
    UnkBox2D_558838 box2;
    UnkBox3D_558838 box = collider->vf_0x28(collider->data);
    fn_710007b430(&box2.min, &box.min);
    fn_710007b430(&box2.max, &box.max);
    return UnkExtent_558838(box2.max.x - box2.min.x, box2.max.y - box2.min.y);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkExtent_558838 fn_7100558838(UnkEmmyAI_558838* self, bool useNamed) {
    if (useNamed && self->owner->container) {
        UnkShape_558838* named =
            reinterpret_cast<UnkShape_558838*>(fn_71008640a4(self->owner->container, &g_unk_71726e1f68));
        if (named) {
            UnkCollider_558838* collider = named->collider;
            UnkBox2D_558838 box2;
            UnkBox3D_558838 box = collider->vf_0x28(collider->data);
            fn_710007b430(&box2.min, &box.min);
            fn_710007b430(&box2.max, &box.max);
            UnkExtent_558838 extent;
            extent.y = box2.max.y - box2.min.y;
            extent.x = box2.max.x - box2.min.x;
            return extent;
        }
    }
    UnkShape_558838* shape = self->owner->movement->GetShape();
    if (!shape)
        return UnkExtent_558838(0.0f, 0.0f);
    return ShapeExtent(shape);
}
