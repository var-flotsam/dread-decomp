// animtree::CLookAtPosControl: the per-frame update. The current point eases toward the target (+0x94 per
// second, snapping when within a unit), is kept inside the control's yaw limits and turned by its roll
// about the node's facing, and the node's look-at matrix from the bone's position to it is applied.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

#include <math.h>

struct UnkAnimatedNode_ovb0138;
struct UnkNodeOwner_ovb0138;
UnkNodeOwner_ovb0138* fn_71002dbb7c(UnkAnimatedNode_ovb0138* self);

UnkVec3f fn_710007a390(const UnkVec3f& v, f32 angle);

struct UnkLookAtPosControl_l2e681c;

// The look-at matrix from `eye` toward `target`, the two axes chosen by `forward` and `up`.
void fn_7100077ce8(UnkMatrix44* out, const UnkVec3f* eye, const UnkVec3f* target, u32 forward, u32 up);
void fn_71002e28d0(UnkLookAtPosControl_l2e681c* self, const UnkMatrix44* matrix);

struct UnkLookAtOwner_l2e681c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x228 - 0x94];
    UnkEntity* entity;  // 0x228
};

struct UnkLookAtPosControl_l2e681c {
    u8 pad_0x0[0x50];
    u64 bone;  // 0x50
    UnkVec3f target;   // 0x58
    UnkVec3f current;  // 0x64
    u32 forward;  // 0x70
    u32 up;       // 0x74
    u8 pad_0x78[0x84 - 0x78];
    f32 roll;     // 0x84
    f32 yaw_min;  // 0x88
    f32 yaw_max;  // 0x8c
    u8 pad_0x90[0x94 - 0x90];
    f32 speed;  // 0x94
};
static_assert(__builtin_offsetof(UnkLookAtPosControl_l2e681c, speed) == 0x94, "layout");

static inline UnkLookAtOwner_l2e681c* OwnerOf(UnkLookAtPosControl_l2e681c* self) {
    return reinterpret_cast<UnkLookAtOwner_l2e681c*>(
        fn_71002dbb7c(reinterpret_cast<UnkAnimatedNode_ovb0138*>(self)));
}

void fn_71002e681c(UnkLookAtPosControl_l2e681c* self, f32 dt) {
    UnkMatrix44 bone = {};
    bone.m[15] = 1.0f;
    bone.m[10] = 1.0f;
    bone.m[5] = 1.0f;
    bone.m[0] = 1.0f;
    {
        UnkEntity* entity = OwnerOf(self)->entity;
        u64 key = self->bone;
        fn_71011735e8(entity, &key, &bone, 0);
    }

    if (!(self->speed <= 0.0f)) {
        f32 dx = self->target.x - self->current.x;
        f32 dy = self->target.y - self->current.y;
        f32 dz = self->target.z - self->current.z;
        if (dx * dx + dy * dy + dz * dz < 1.0f) {
            self->current = self->target;
        } else {
            f32 t = self->speed * dt;
            f32 k = !(t <= 0.0f) ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
            self->current = UnkVec3f(self->current.x + k * dx, self->current.y + k * dy, self->current.z + k * dz);
        }
    } else {
        self->current = self->target;
    }
    UnkVec3f pos = self->current;

    f32 roll = self->roll;
    if (self->yaw_min < self->yaw_max || roll != 0.0f) {
        f32 facing = OwnerOf(self)->xform.GetAngle().y > 0.0f ? 1.0f : -1.0f;
        f32 ox = bone.m[3];
        f32 oy = bone.m[7];
        f32 oz = bone.m[11];
        UnkVec3f dir(pos.x - ox, pos.y - oy, pos.z - oz);
        f32 len = __builtin_sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        if (len != 0.0f) {
            dir.x /= len;
            dir.y /= len;
            dir.z /= len;
        }
        f32 dist = !(len < 800.0f) ? len : 800.0f;
        if (self->yaw_min < self->yaw_max) {
            f32 c = acosf(facing * dir.x + dir.y * 0.0f + dir.z * 0.0f);
            f32 yaw = dir.x * 0.0f + dir.y + dir.z * 0.0f > 0.0f ? -c : c;
            if (yaw < self->yaw_min || yaw > self->yaw_max) {
                f32 limit = yaw < self->yaw_min ? self->yaw_min : self->yaw_max;
                dir = fn_710007a390(UnkVec3f(facing, 0.0f, 0.0f), facing < 0.0f ? limit : -limit);
                pos = UnkVec3f(ox + dist * dir.x, oy + dist * dir.y, oz + dist * dir.z);
            }
        }
        if (roll != 0.0f) {
            dir = fn_710007a390(dir, facing * self->roll);
            pos = UnkVec3f(ox + dist * dir.x, oy + dist * dir.y, oz + dist * dir.z);
        }
    }

    UnkVec3f eye(bone.m[3], bone.m[7], bone.m[11]);
    fn_7100077ce8(&bone, &eye, &pos, self->forward, self->up);
    fn_71002e28d0(self, &bone);
}
