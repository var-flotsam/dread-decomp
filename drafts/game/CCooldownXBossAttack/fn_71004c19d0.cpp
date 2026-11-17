// CCooldownXBossAttack's unit (tools/tu_map.py): places the attack's box collider (created on first use in
// the owner's collision container) along the node the owner's model aims with, or straight down from above
// the owner when the model has no such node, and enables it. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CCollisionComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726d7890;  // the collider's name
extern char g_unk_7101649a48[];                                         // the aiming node's key

static inline UnkVec3f Scaled_l4c19d0(const UnkVec3f& v, f32 f) { return UnkVec3f(v.x * f, v.y * f, v.z * f); }

static inline UnkVec3f operator+(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

// An oriented box in the plane: its centre, its two axes, its size and its angle.
struct UnkBox_l4c19d0 {
    f32 center_x;  // 0x0
    f32 center_y;
    f32 axis_x[2];  // 0x8
    f32 axis_y[2];  // 0x10
    f32 length;     // 0x18
    f32 width;      // 0x1c
    f32 angle;      // 0x20
    u32 field_0x24;

    // A box of the given size, centred on the origin and turned by `angle_`.
    UnkBox_l4c19d0(f32 length_, f32 width_, f32 angle_)
        : center_x(0.0f), center_y(0.0f), axis_x{cosf(angle_), sinf(angle_)}, axis_y{-sinf(angle_), cosf(angle_)},
          length(length_), width(width_), angle(angle_), field_0x24(0) {}

    UnkBox_l4c19d0& operator=(const UnkBox_l4c19d0& o) {
        center_x = o.center_x;
        center_y = o.center_y;
        axis_x[0] = o.axis_x[0];
        axis_x[1] = o.axis_x[1];
        axis_y[0] = o.axis_y[0];
        axis_y[1] = o.axis_y[1];
        length = o.length;
        width = o.width;
        field_0x24 = o.field_0x24;
        angle = o.angle;
        return *this;
    }
};

// A collider's shape; kind 0x40 is the box.
struct UnkShape_l4c19d0 {
    void** vtable;
    u8 pad_0x8[0xa0];
    UnkBox_l4c19d0 box;  // 0xa8

    s32 vf_0x18() { return reinterpret_cast<s32 (*)(UnkShape_l4c19d0*)>(vtable[0x18 / 8])(this); }
    void vf_0x70(const UnkVec3f* center) {
        reinterpret_cast<void (*)(UnkShape_l4c19d0*, const UnkVec3f*)>(vtable[0x70 / 8])(this, center);
    }
    void vf_0x80() { reinterpret_cast<void (*)(UnkShape_l4c19d0*)>(vtable[0x80 / 8])(this); }
};

static inline UnkShape_l4c19d0* AsBox_l4c19d0(UnkShape_l4c19d0* shape) {
    return shape->vf_0x18() == 0x40 ? shape : nullptr;
}

struct UnkCollider_l4c19d0 {
    void** vtable;
    u8 pad_0x8[0x150];
    UnkShape_l4c19d0* shape;  // 0x158

    // The shape when it is a box.
    UnkShape_l4c19d0* Box() { return shape ? AsBox_l4c19d0(shape) : nullptr; }
    void vf_0x48() { reinterpret_cast<void (*)(UnkCollider_l4c19d0*)>(vtable[0x48 / 8])(this); }
};

using UnkCreateCollider_l4c19d0 = UnkCollider_l4c19d0* (*)(UnkCollisionContainer*, const CStrId*, const CStrId*,
                                                           const CStrId*, const CStrId*, u32, u32, u32, void*, s32,
                                                           void*, s32, void*, s32, bool, bool, bool);

// An empty string id: the pool's empty string, the pool created on first use.
struct UnkEmptyId_l4c19d0 {
    base::global::UnkStringData* data;

    UnkEmptyId_l4c19d0() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkEmptyId_l4c19d0() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
};

struct UnkCooldownOwner_l4c19d0 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1c0];
    UnkEntity* model;  // 0x228
    u8 pad_0x230[0x70];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkCooldownDef_l4c19d0 {
    u8 pad_0x0[0xec];
    f32 width;  // 0xec
};

struct UnkCooldownBeam_l4c19d0 {
    void** vtable;
    UnkCooldownOwner_l4c19d0* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkCooldownDef_l4c19d0* def;  // 0x28
    UnkBox_l4c19d0 box;           // 0x30
    UnkCollider_l4c19d0* collider;  // 0x58
};

void fn_71004c19d0(UnkCooldownBeam_l4c19d0* self) {
    if (!self->collider) {
        UnkCollisionContainer* collision = self->owner->collision;
        {
            UnkEmptyId_l4c19d0 none;
            const CStrId* type = reinterpret_cast<const CStrId*>(
                reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0x19d0);
            UnkCreateCollider_l4c19d0 create = (*reinterpret_cast<UnkCreateCollider_l4c19d0**>(collision))[0x168 / 8];
            self->collider = create(collision, &g_unk_71726d7890, &g_unk_71726d7890,
                                    reinterpret_cast<const CStrId*>(&none), type, 0x100, 0, 1, nullptr, -1, nullptr,
                                    -1, nullptr, -1, false, false, true);
        }
        if (!self->collider)
            return;
    }

    UnkCooldownOwner_l4c19d0* owner = self->owner;
    UnkVec3f pos(owner->x, owner->y, owner->z);
    UnkMatrix44 m = {};
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    UnkVec3f dir;
    if (fn_71011735e8(owner->model, g_unk_7101649a48, &m, 0)) {
        dir = UnkVec3f(m.m[0], m.m[4], m.m[8]);
        pos = UnkVec3f(m.m[3], m.m[7], m.m[11]);
    } else {
        // No node: aim straight down from 1000 units above the owner.
        dir = UnkVec3f(0.0f, -1.0f, 0.0f);
        pos = UnkVec3f(pos.x + 0.0f, pos.y + 1000.0f, pos.z + 0.0f);
    }
    f32 angle = atan2f(dir.y, dir.x);
    const f32 length = 2500.0f;
    UnkVec3f center = Scaled_l4c19d0(Scaled_l4c19d0(dir, length), 0.5f) + pos;
    self->box = UnkBox_l4c19d0(length, self->def->width, angle);

    UnkShape_l4c19d0* box = AsBox_l4c19d0(self->collider->shape);
    box->box = self->box;
    box->vf_0x80();
    self->collider->Box()->vf_0x70(&center);
    self->collider->vf_0x48();
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(self->collider), true);
}
