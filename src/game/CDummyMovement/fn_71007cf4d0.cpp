// CDummyMovement's unit (tools/tu_map.py, unit 1232): the update. After the base step it records the owner's
// position and angles, takes the move (vf_0x280 with a target at +0x230, vf_0x288 without), adds the animation's
// root motion turned into world space, keeps the result above the floor at +0x254 and moves the owner there.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CAnimationComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

bool fn_7101179808(UnkAnimationComponent* self);

struct UnkDummyOwner_l7cf4d0 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x3b0 - 0x228];
    bool moved;  // 0x3b0

    // A direction in the owner's space turned into world space.
    UnkVec3f ToWorld(const UnkVec3f& v) {
        return TransformDirectionByMatrix3x3(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(&xform)), v);
    }
};
static_assert(__builtin_offsetof(UnkDummyOwner_l7cf4d0, moved) == 0x3b0, "layout");

struct UnkDummyTarget_l7cf4d0 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkDummySettings_l7cf4d0 {
    u8 pad_0x0[0x64];
    bool no_floor;  // 0x64
};

struct UnkDummyMovement_l7cf4d0 {
    virtual void vf_0x0();
    u8 pad_0x8[0x28];
    UnkDummyOwner_l7cf4d0* owner;  // 0x30
    u8 pad_0x38[0x84 - 0x38];
    UnkVec3f position;  // 0x84
    UnkVec3f angle;     // 0x90
    u8 pad_0x9c[0x230 - 0x9c];
    UnkDummyTarget_l7cf4d0* target;  // 0x230
    u8 pad_0x238[0x248 - 0x238];
    UnkDummySettings_l7cf4d0* settings;  // 0x248
    u8 pad_0x250[0x4];
    f32 floor_y;  // 0x254
};
static_assert(__builtin_offsetof(UnkDummyMovement_l7cf4d0, floor_y) == 0x254, "layout");

struct UnkDummyMovementV_l7cf4d0 {
    u8 pad_0x0[0x280];
    UnkVec3f (*vf_0x280)(UnkDummyMovement_l7cf4d0* self, f32 dt);
    UnkVec3f (*vf_0x288)(UnkDummyMovement_l7cf4d0* self, f32 dt);
};

static inline const UnkDummyMovementV_l7cf4d0* VTable(UnkDummyMovement_l7cf4d0* self) {
    return *reinterpret_cast<UnkDummyMovementV_l7cf4d0**>(self);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71007cf4d0(UnkDummyMovement_l7cf4d0* self, f32 dt) {
    fn_71010d1b60(reinterpret_cast<UnkSteeringMovement_ovb0023*>(self), dt);
    UnkDummyOwner_l7cf4d0* owner = self->owner;
    self->position = owner->xform.GetTranslation();
    self->angle = owner->xform.GetAngle();
    UnkVec3f move;
    if (self->target && self->target->field_0x10)
        move = VTable(self)->vf_0x280(self, dt);
    else
        move = VTable(self)->vf_0x288(self, dt);

    UnkVec3f root(0.0f, 0.0f, 0.0f);
    if (UnkAnimationComponent* animation = self->owner->animation) {
        if (fn_7101179808(animation)) {
            UnkStruct_71007142e8 motion;
            fn_71007142e8(&motion);
            fn_7101179d08(self->owner->animation, &motion, &g_unk_717270ec18);
            root = self->owner->ToWorld(UnkVec3f(motion.m[3], motion.m[7], motion.m[11]));
        }
    }

    UnkVec3f pos(move.x + root.x, move.y + root.y, move.z + root.z);
    if (!self->settings->no_floor && pos.y < self->floor_y) {
        if (!(self->position.y <= self->floor_y))
            pos.y = self->floor_y;
        else
            pos = self->position;
    }
    owner = self->owner;
    owner->xform.SetTranslation(pos);
    owner->moved = true;
}
