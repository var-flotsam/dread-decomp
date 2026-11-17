// CMovementComponent (unit 2688): this frame's root rotation, from the animation's two root poses or
// from turning the transform toward the angle the component wants, and the facing it leaves.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CAnimationComponent.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

bool fn_7101179d00(UnkAnimationComponent* self);

struct UnkRotationOwner_l0d25a0 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkRotationConfig_l0d25a0 {
    u8 pad_0x0[0x58];
    bool turn_to_target;  // 0x58
};

struct UnkRotationMovement_l0d25a0;

struct UnkRotationMovementVtable_l0d25a0 {
    u8 pad_0x0[0x170];
    bool (*vf_0x170)(UnkRotationMovement_l0d25a0* self);
    u8 pad_0x178[0x210 - 0x178];
    UnkVec3f (*vf_0x210)(UnkRotationMovement_l0d25a0* self);
    u8 pad_0x218[0x248 - 0x218];
    UnkVec3f (*vf_0x248)(UnkRotationMovement_l0d25a0* self, f32 dt, const UnkVec3f& angle);
};

struct UnkRotationMovement_l0d25a0 {
    UnkRotationMovementVtable_l0d25a0* vtable;
    u8 pad_0x8[0x28];
    UnkRotationOwner_l0d25a0* owner;  // 0x30
    u8 pad_0x38[0x1f8 - 0x38];
    UnkRotationConfig_l0d25a0* config;  // 0x1f8
};

// A root pose: its transform, and bit 2 of `flags` when it holds a rotation.
struct UnkRootPose_l0d25a0 {
    UnkMatrix44 matrix;
    u8 pad_0x40[0x1c];
    u8 flags;  // 0x5c
    u8 pad_0x5d[0x13];
};

struct UnkRootPoses_l0d25a0 {
    u8 pad_0x0[0x70];
    UnkRootPose_l0d25a0 first;   // 0x70
    UnkRootPose_l0d25a0 second;  // 0xe0
};

static const u64 kRootRotationKey = 0xf4061ff25a2163d6;

static inline const UnkMatrix44* PoseRotation(const UnkRootPose_l0d25a0& pose) {
    return (pose.flags & 4) ? &fn_710002614c(&pose.matrix) : nullptr;
}

static inline void SetIdentity(UnkMatrix44* m) {
    for (u32 i = 0; i < 16; i++)
        m->m[i] = 0.0f;
    m->m[15] = 1.0f;
    m->m[10] = 1.0f;
    m->m[5] = 1.0f;
    m->m[0] = 1.0f;
}

// Concatenates the rest of this frame's rotation onto `rotation`: the first root pose, or the turn
// from the transform's angle toward the one the component wants (identity when there is none to
// make). False when nothing was concatenated.
static inline bool TurnRotation(UnkRotationMovement_l0d25a0* self, f32 dt, const UnkMatrix44* first,
                                UnkAnimationComponent* animation, UnkXform* xform, UnkMatrix44* rotation) {
    UnkMatrix44 inverse;
    UnkMatrix44 turn;
    const UnkMatrix44* lhs;
    const UnkMatrix44* rhs;
    UnkAnimationComponent* current = self->owner->animation;
    bool held = current && fn_7101179c3c(current, kRootRotationKey) && fn_7101179d00(current);
    if (!held && first) {
        lhs = rotation;
        rhs = first;
    } else {
        if (self->vtable->vf_0x170(self))
            return false;
        if (animation && !held && fn_7101179870(reinterpret_cast<UnkStateSet_1441*>(animation)))
            return false;
        UnkVec3f angle = xform->GetAngle();
        UnkVec3f target = self->vtable->vf_0x248(self, dt, angle);
        bool unchanged = !UNK_CHANGED(target.x, angle.x) && !UNK_CHANGED(target.y, angle.y) &&
                         !UNK_CHANGED(target.z, angle.z);
        if (!self->config->turn_to_target || unchanged || held) {
            SetIdentity(rotation);
            return false;
        }
        UnkMatrix44 current_rotation;
        UnkCopyMatrix(&current_rotation, xform->GetMatrix());
        current_rotation.m[3] = 0.0f;
        current_rotation.m[7] = 0.0f;
        current_rotation.m[11] = 0.0f;
        inverse = fn_7100078314(&current_rotation);
        turn = fn_7100078fc8(reinterpret_cast<const UnkMatrix44&>(target));
        lhs = &inverse;
        rhs = &turn;
    }
    UnkCopyMatrix(rotation, fn_71000127c8(*lhs, *rhs));
    return true;
}

void fn_71010d25a0(UnkRotationMovement_l0d25a0* self, f32 dt, const UnkRootPoses_l0d25a0* poses,
                   UnkXform* xform, UnkMatrix44* rotation, UnkVec3f* facing) {
    const UnkMatrix44* second = PoseRotation(poses->second);
    const UnkMatrix44* first = PoseRotation(poses->first);
    UnkAnimationComponent* animation = self->owner->animation;

    bool compose = false;
    if (second && (self->vtable->vf_0x170(self) ||
                   (first && animation && !fn_7101179c3c(animation, kRootRotationKey)))) {
        UnkCopyMatrix(rotation, fn_71000127c8(*rotation, *second));
        compose = true;
    }

    if (TurnRotation(self, dt, first, animation, xform, rotation))
        compose = true;

    if (compose) {
        UnkXform world(fn_71000127c8(xform->GetMatrix(), *rotation));
        const UnkMatrix44& m = world.GetMatrix();
        *facing = UnkVec3f(m.m[2], m.m[6], m.m[10]);
    } else {
        *facing = self->vtable->vf_0x210(self);
    }
    rotation->m[3] = 0.0f;
    rotation->m[7] = 0.0f;
    rotation->m[11] = 0.0f;
}
