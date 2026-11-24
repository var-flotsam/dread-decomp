// CSideEnemyMovement unit (tools/tu_map.py unit 1471): after the base update (fn_71010c903c), keeps the
// owner's transform angles (+0x1c28), sets the direction (+0x70) from the side (+0x1c34) and, when the
// config allows turning, turns the owner round when it faces the other way; then sets one of the two
// flag bits the config names (+0x160, +0x164) and clears the other by which way the owner's forward axis
// points along that direction. Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/UnkShakernautId.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkShakernautCtrl_b53;

struct UnkSideEnemyOwner_l9b601c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x38 - sizeof(UnkXform)];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
    u8 pad_0x228[0x198];
    bool transform_changed;  // 0x3c0
};

struct UnkSideEnemyConfig_l9b601c {
    u8 pad_0x0[0x160];
    u32 bit_a;  // 0x160
    u32 bit_b;  // 0x164
    bool turns;  // 0x168
};

struct UnkSideEnemyMovement_l9b601c {
    u8 pad_0x0[0x30];
    UnkSideEnemyOwner_l9b601c* owner;  // 0x30
    u8 pad_0x38[0x38];
    UnkVec3f dir;  // 0x70
    u8 pad_0x7c[0x1c20 - 0x7c];
    UnkSideEnemyConfig_l9b601c* config;  // 0x1c20
    UnkVec3f angle;                      // 0x1c28
    s32 side;                            // 0x1c34
    s32 field_0x1c38;
};

void fn_71010c903c(UnkSideEnemyMovement_l9b601c* self);

// The direction the side faces along x (none for any other side).
static inline UnkVec3f SideDir(s32 side) {
    switch (side) {
    case 1:
        return UnkVec3f(1.0f, 0.0f, 0.0f);
    case 0:
        return UnkVec3f(-1.0f, 0.0f, 0.0f);
    default:
        return UnkVec3f(0.0f, 0.0f, 0.0f);
    }
}

static inline f32 Sign(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

static inline bool Near(f32 a, f32 b, f32 eps) { return a - eps <= b && a + eps >= b; }

void CSideEnemyMovement_ApplyInitialDir(UnkSideEnemyMovement_l9b601c* self) {
    // The four flag ids get stack slots of their own, as in fn_71009b6b6c: declared ahead of the body.
    goto start;
    UnkShakernautId_b53 clear_b;
    UnkShakernautId_b53 set_a;
    UnkShakernautId_b53 clear_a;
    UnkShakernautId_b53 set_b;
start:
    fn_71010c903c(self);
    UnkVec3f angle = self->owner->xform.GetAngle();
    self->angle = angle;
    UnkSideEnemyOwner_l9b601c* owner = self->owner;
    UnkShakernautCtrl_b53* ctrl;
    if (owner && (ctrl = owner->ctrl)) {
        UnkVec3f side_dir = SideDir(self->side);
        f32 facing = side_dir.x;
        if (!Near(facing, 0.0f, 0.001f)) {
            self->dir = side_dir;
            if (self->config->turns && !Near(Sign(angle.y), facing, 0.1f)) {
                self->angle.y = -angle.y;
                owner->xform.SetAngle(self->angle);
                owner->transform_changed = true;
            }
        }
        const UnkVec3f& dir = self->dir;
        f32 x = dir.x, y = dir.y, z = dir.z;
        const UnkMatrix44& m = self->owner->xform.GetMatrix();
        f32 fx = m.m[2], fy = m.m[6], fz = m.m[10];
        f32 along = x * fx + y * fy + z * fz;
        if (along > 0.0f) {
            clear_b.value = 1u << self->config->bit_b;
            fn_7101178564(ctrl, &clear_b, true, false);
            set_a.value = 1u << self->config->bit_a;
            fn_71011785a8(ctrl, &set_a, true, false);
        } else if (along < 0.0f) {
            clear_a.value = 1u << self->config->bit_a;
            fn_7101178564(ctrl, &clear_a, true, false);
            set_b.value = 1u << self->config->bit_b;
            fn_71011785a8(ctrl, &set_b, true, false);
        }
    }
    self->field_0x1c38 = 0x7fffffff;
}
