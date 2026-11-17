// CCameraBoundaryCtrl unit: when the controller is active, tests the two colliders its owner's weak
// references name against each other; on a hit it fits a 2D box to the camera's view (at most 50 units
// inside each edge) and tests it against the second collider again, otherwise it drops the reference at
// +0xc30. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU311.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "game/logic/collision/CCollider.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

static const f64 kPi = 3.14159265358979323846;

// As src/game_aca57c/fn_7100d78378.cpp defines it.
template <u32 Offset>
struct UnkWeakRefAt {
    u8 pad_0x0[Offset];
    UnkWeakHandle* ref;
};
void fn_71010e6038(UnkWeakRefAt<0xc30>* self, UnkWeakTarget* other);

// As src/game/CBreakableTileGroup.cpp defines them.
struct UnkBox2D {
    f32 min_x, min_y, max_x, max_y;

    UnkBox2D(f32 min_x_, f32 min_y_, f32 max_x_, f32 max_y_)
        : min_x(min_x_), min_y(min_y_), max_x(max_x_), max_y(max_y_) {}
};

// An axis-aligned box shape (0xc0 bytes): its transform follows the vtable.
struct UnkBoxShape_l763fc0 {
    void* vtable;
    UnkMatrix44 transform;
    u8 pad_0x48[0x78];
};
static_assert(sizeof(UnkBoxShape_l763fc0) == 0xc0, "");

// A collider as the shape tester takes it: the object it belongs to at +0x158.
struct UnkBoundaryCollider_l763fc0 {
    u8 pad_0x0[0x158];
    void* owner;
};

struct UnkBoundaryObject_l763fc0 {
    u8 pad_0x0[0xc30];
    UnkWeakHandle* ref_0xc30;
    UnkWeakHandle* ref_0xc38;
};

struct UnkBoundaryOwner_l763fc0 {
    u8 pad_0x0[0x10];
    UnkBoundaryObject_l763fc0* object;
};

struct UnkCameraBoundaryCtrl_l763fc0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual f32 vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    u8 pad_0x8[0x2c];
    bool active;          // +0x34
    f32 x;                // +0x38
    f32 y;                // +0x3c
    f32 z;                // +0x40
    u8 pad_0x44[0x18];
    f32 fov;              // +0x5c, degrees
    u8 pad_0x60[0x4c];
    bool disabled;        // +0xac
    UnkBoundaryOwner_l763fc0* owner;  // +0xb0
    u8 pad_0xb8[0x188];
    f32 height_offset;    // +0x240
};
static_assert(__builtin_offsetof(UnkCameraBoundaryCtrl_l763fc0, owner) == 0xb0, "");
static_assert(__builtin_offsetof(UnkCameraBoundaryCtrl_l763fc0, height_offset) == 0x240, "");

static inline UnkShapeTester_A5_b68* Tester() {
    UnkShapeTester_A5_b68* t = g_unk_71726a47a0;
    if (!t) {
        t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return t;
}

static inline UnkBoundaryCollider_l763fc0* Target(UnkWeakHandle* handle) {
    return handle ? reinterpret_cast<UnkBoundaryCollider_l763fc0*>(handle->target) : nullptr;
}

static inline UnkShape* AsShape(UnkBoundaryCollider_l763fc0* collider) {
    return reinterpret_cast<UnkShape*>(collider);
}

// Tests one collider (the object and its transform) against another.
static inline UnkColliderResult Test(UnkShapeTester_A5_b68* tester, void* owner, const UnkMatrix44& transform,
                                     void* other_owner, const UnkMatrix44& other_transform) {
    return fn_7100d51368(tester, owner, const_cast<UnkMatrix44*>(&transform), other_owner, other_transform,
                         nullptr, 1);
}

void fn_7100763fc0(UnkCameraBoundaryCtrl_l763fc0* self) {
    if (!self->active || self->disabled)
        return;

    UnkBoundaryObject_l763fc0* object = self->owner->object;
    UnkBoundaryCollider_l763fc0* first = Target(object->ref_0xc30);
    UnkBoundaryCollider_l763fc0* second = Target(object->ref_0xc38);
    if (!second || !first)
        return;

    UnkColliderResult hit = Test(Tester(), first->owner, fn_7100d36a2c(AsShape(first)), second->owner,
                                 fn_7100d36a2c(AsShape(second)));
    if (!(hit.bits & 1)) {
        UnkBoundaryOwner_l763fc0* owner = self->owner;
        fn_71010e6038(reinterpret_cast<UnkWeakRefAt<0xc30>*>(owner ? owner->object : nullptr), nullptr);
        return;
    }

    self->vf_0x150();
    self->vf_0x158();
    f32 base = self->vf_0x160() + self->height_offset;
    f32 center_x = self->x;
    f32 center_y = self->y;
    f32 half_fov = (f32)(self->fov * kPi / 180.0) * 0.5f;
    f32 distance = self->z - base;
    f32 half_width = tanf(half_fov) * distance + -40.0f;
    f32 scale = fn_7100132fa0();
    f32 vertical_fov = fn_7100217ee0((f32)(half_fov * 180.0f / kPi), scale);
    f32 half_height = distance * tanf((f32)(vertical_fov * kPi / 180.0)) + -24.0f;

    f32 mid_y = center_y + half_width * 0.0f;
    f32 right = (center_x + half_width) + half_height * 0.0f;
    f32 left = (center_x - half_width) + half_height * 0.0f;
    f32 top = mid_y + half_height;
    f32 bottom = mid_y - half_height;
    f32 extent_x = fabsf(right - left) * 0.5f;
    f32 extent_y = fabsf(top - bottom) * 0.5f;
    f32 margin_y = !(extent_y >= 50.0f) ? extent_y : 50.0f;
    f32 margin_x = !(extent_x >= 50.0f) ? extent_x : 50.0f;

    UnkBox2D box(self->x - extent_x + margin_x, self->y - extent_y + margin_y, self->x + extent_x - margin_x,
                 self->y + extent_y - margin_y);
    UnkBoxShape_l763fc0 shape;
    fn_7100d5e1f8(reinterpret_cast<UnkCollisionShape*>(&shape), &box);
    UnkColliderResult inside =
        Test(Tester(), &shape, shape.transform, second->owner, fn_7100d36a2c(AsShape(second)));
    if (!(inside.bits & 1))
        self->vf_0x1b8();
    fn_7100d5e2c8(reinterpret_cast<UnkCollisionShape*>(&shape));
}
