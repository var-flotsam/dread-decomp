// CSamusMovement unit (tools/tu_map.py, reflection hint). fn_71009e83fc: the offset from the
// player's movement to the entity the game manager tracks at +0x29a0 (its +0x10), while the
// movement's controller (+0x2d08, slot 0x108) allows it: x is the difference of the two entities'
// +0x37c, y the difference of their shapes' lower bounds (each shape's collider at +0x158 gives a 3D
// box through slot 0x28); otherwise zero. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CCharacterMovement.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::math::CVector3D;

struct UnkFlagged;
struct UnkShape;
// Declared as src/game/CEntity_2.cpp does.

struct UnkBox3D_sm42 {
    CVector3D min;
    CVector3D max;
};

struct UnkCollider_sm42 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkBox3D_sm42 vf_0x28(const void* data) const;
    u8 data[8];  // 0x8
};

struct UnkShape_sm42 {
    u8 pad_0x0[0x158];
    UnkCollider_sm42* collider;  // 0x158
};

struct UnkEntityMovement_sm42 {
    u8 pad_0x0[0x3b8];
    UnkShape_sm42* shape_b;  // 0x3b8
    UnkShape_sm42* shape_a;  // 0x3c0
};

struct UnkEntity_sm42 {
    u8 pad_0x0[0x200];
    UnkEntityMovement_sm42* movement;  // 0x200
    u8 pad_0x208[0x174];
    f32 field_0x37c;
};

struct UnkTracked_sm42 {
    u8 pad_0x0[0x10];
    UnkEntity_sm42* entity;  // 0x10
};

struct UnkController_sm42 {
    void** vtable;
    bool vf_0x108() { return reinterpret_cast<bool (*)(UnkController_sm42*)>(vtable[0x108 / 8])(this); }
};

struct UnkSamusMovement_sm42 {
    u8 pad_0x0[0x30];
    UnkEntity_sm42* owner;  // 0x30
    u8 pad_0x38[0x2cd0];
    UnkController_sm42* controller;  // 0x2d08
};

struct UnkVec3_sm42 {
    f32 x, y, z;
    UnkVec3_sm42(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    UnkVec3_sm42(const UnkVec3_sm42& o) : x(o.x), y(o.y), z(o.z) {}
};

static inline UnkTracked_sm42* Tracked(UnkGameManager* gm) {
    return *reinterpret_cast<UnkTracked_sm42**>(reinterpret_cast<char*>(gm) + 0x29a0);
}

UnkVec3_sm42 fn_71009e83fc(UnkSamusMovement_sm42* self) {
    f32 y = 0.0f;
    f32 x = 0.0f;
    UnkTracked_sm42* tracked = Tracked(g_unk_7101d47310);
    UnkEntity_sm42* entity;
    if (tracked && (entity = tracked->entity) && self->controller->vf_0x108()) {
        UnkShape_sm42* shape = reinterpret_cast<UnkShape_sm42*>(
            fn_7100976588(reinterpret_cast<UnkFlagged*>(self)));
        UnkCollider_sm42* collider = shape->collider;
        UnkBox3D_sm42 own = collider->vf_0x28(collider->data);
        f32 own_bottom = own.min.y;
        UnkShape_sm42* other_shape = entity->movement->shape_a;
        if (!other_shape)
            other_shape = entity->movement->shape_b;
        UnkCollider_sm42* other_collider = other_shape->collider;
        UnkBox3D_sm42 other = other_collider->vf_0x28(other_collider->data);
        y = own_bottom - other.min.y;
        x = entity->field_0x37c - self->owner->field_0x37c;
    }
    return UnkVec3_sm42(x, y, 0.0f);
}
