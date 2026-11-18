// CMorphBallMovement's unit (tools/tu_map.py unit 1428): a contact. Unless the base handler takes it
// (fn_71010d9638, then fn_710093ed64), keeps the touched shape when it is of kind 0x10 (else clears it)
// and, when the touched element (fn_710012431c) changes, resets the roll axis from +0x154 if it lies
// along the contact, keeps the element and runs fn_7100943f3c; then the base contact (fn_710097a664).
// Names and layouts are placeholders.
#include "game/UnkTU109.h"
#include "game/UnkTU2689.h"
#include "types.h"

struct UnkMorphBall_eb64;
void fn_710093ed64(UnkMorphBall_eb64* self);

struct UnkAxis_l94dba0 {
    f32 x, y, z;
};

struct UnkShape_l94dba0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 Kind();
    u8 pad_0x8[0xa0];
    u8 elements[0xb0];  // 0xa8, a pool of 0x48-byte entries
};

struct UnkShapeOwner_l94dba0 {
    u8 pad_0x0[0x158];
    UnkShape_l94dba0* shape;  // 0x158
};

struct UnkHandle_l94dba0 {
    u8 pad_0x0[0x10];
    UnkShapeOwner_l94dba0* target;  // 0x10
};

struct UnkContact_l94dba0 {
    UnkHandle_l94dba0* other;  // 0x0
    u8 pad_0x8[0x8];
    u32 element;  // 0x10
    u8 pad_0x14[0x18];
    f32 normal_x;  // 0x2c
    f32 normal_y;  // 0x30
};

// What the ball is touching: the shape and its element.
struct UnkTouched_l94dba0 {
    UnkShape_l94dba0* shape;
    UnkStruct_0x48* element;
};

struct UnkMorphBall_l94dba0 {
    u8 pad_0x0[0x128];
    UnkHandle_l94dba0* field_0x128;
    u8 pad_0x130[0x24];
    UnkAxis_l94dba0 rest_axis;  // 0x154
    u8 pad_0x160[0x2428 - 0x160];
    UnkTouched_l94dba0 touched;  // 0x2428
    s32 field_0x2438;
    u8 pad_0x243c[0x2480 - 0x243c];
    u32 field_0x2480;
    u8 pad_0x2484[8];
    UnkAxis_l94dba0 axis;  // 0x248c
};

void fn_7100943f3c(UnkMorphBall_l94dba0* self, UnkContact_l94dba0* contact, bool a);
void fn_710097a664(UnkMorphBall_l94dba0* self, UnkContact_l94dba0* contact);

// The shape when it is of kind 0x10.
static inline UnkShape_l94dba0* AsKind10(UnkShape_l94dba0* shape) {
    return shape->Kind() == 0x10 ? shape : nullptr;
}

void fn_710094dba0(UnkMorphBall_l94dba0* self, UnkContact_l94dba0* contact) {
    u64 result = 0;
    if (fn_71010d9638(reinterpret_cast<UnkMove_b152*>(self), reinterpret_cast<UnkHitInfo_b152*>(contact),
                      &result)) {
        fn_710093ed64(reinterpret_cast<UnkMorphBall_eb64*>(self));
        return;
    }
    if (contact->other && contact->other->target) {
        if (UnkShape_l94dba0* shape = AsKind10(contact->other->target->shape)) {
            self->touched.shape = shape;
            UnkStruct_0x48* element =
                fn_710012431c(reinterpret_cast<UnkPool_0x48*>(shape->elements), contact->element);
            if (self->touched.element != element) {
                if (self->field_0x128 && self->field_0x128->target) {
                    const UnkAxis_l94dba0& v = self->axis;
                    f32 along = -contact->normal_y * v.x + contact->normal_x * v.y + 0.0f * v.z;
                    if (along == 0.0f) {
                        self->axis.x = self->rest_axis.x;
                        self->axis.y = self->rest_axis.y;
                        self->axis.z = self->rest_axis.z;
                    }
                }
                self->touched.element = element;
                self->field_0x2438 = -1;
                fn_7100943f3c(self, contact, true);
            }
        } else {
            self->field_0x2480 = 0;
            self->touched.shape = nullptr;
            self->touched.element = nullptr;
        }
    }
    fn_710097a664(self, contact);
}
