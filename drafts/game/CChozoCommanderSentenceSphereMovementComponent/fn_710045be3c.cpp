// CChozoCommanderSentenceSphereMovementComponent's unit: the sphere's per-frame update (fn_710045be3c). While it is
// active it grows its radius toward the definition's maximum, then turns its velocity in the horizontal plane
// toward the target point (kept over a blocking actor), at most the definition's turn rate. Names and layouts are
// placeholders.
#include <math.h>

#include "game/CProjectileMovement.h"
#include "game/UnkVec3Bits.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkClamper;
struct UnkSentenceSphereMovement_ovb0135;
UnkVec3Bits fn_710045bd10(UnkClamper* self);
void CChozoCommanderSentenceSphereMovementComponent_ApplyRadius(UnkSentenceSphereMovement_ovb0135* self);
UnkVec3f fn_710007a390(const UnkVec3f& v, f32 angle);

struct UnkSphereDef_l45be3c {
    u8 pad_0x0[0x138];
    f32 turn_speed;    // 0x138
    f32 min_radius;    // 0x13c
    f32 mid_radius;    // 0x140
    f32 max_radius;    // 0x144
    f32 grow_time;     // 0x148
    f32 finish_time;   // 0x14c
};

struct UnkSpherePosed_l45be3c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkSphereBlockerHolder_l45be3c {
    u8 pad_0x0[0x10];
    UnkSpherePosed_l45be3c* actor;  // 0x10
};

struct UnkSentenceSphere_l45be3c {
    u8 pad_0x0[0x30];
    UnkSpherePosed_l45be3c* owner;  // 0x30
    u8 pad_0x38[0x230 - 0x38];
    UnkSphereBlockerHolder_l45be3c* blocker;  // 0x230
    u8 pad_0x238[0x4b8 - 0x238];
    f32 velocity_x;  // 0x4b8
    f32 velocity_y;  // 0x4bc
    f32 velocity_z;  // 0x4c0
    u8 pad_0x4c4[0x5d0 - 0x4c4];
    UnkSphereDef_l45be3c* def;  // 0x5d0
    s32 state;                  // 0x5d8
    f32 radius;                 // 0x5dc
    u8 pad_0x5e0[4];
    f32 time;  // 0x5e4
};

static inline f32 ClampUnit(f32 v) {
    return v <= -1.0f ? -1.0f : (v > 1.0f ? 1.0f : v);
}

void fn_710045be3c(UnkSentenceSphere_l45be3c* self, f32 dt) {
    fn_7100ac40fc(reinterpret_cast<UnkChargeComponent*>(self), dt);
    if (self->state != 1)
        return;

    UnkSphereDef_l45be3c* def = self->def;
    self->time += dt;
    f32& radius = self->radius;
    if (radius < def->max_radius) {
        f32 range;
        f32 duration;
        if (radius < def->mid_radius) {
            range = def->mid_radius - def->min_radius;
            duration = def->grow_time;
        } else {
            range = def->max_radius - def->mid_radius;
            duration = def->finish_time;
        }
        radius += range * dt / duration;
        radius = radius < def->max_radius ? radius : def->max_radius;
        CChozoCommanderSentenceSphereMovementComponent_ApplyRadius(reinterpret_cast<UnkSentenceSphereMovement_ovb0135*>(self));
    }

    UnkVec3Bits target = fn_710045bd10(reinterpret_cast<UnkClamper*>(self));
    UnkSpherePosed_l45be3c* owner = self->owner;
    f32 dx = target.x - owner->x;
    f32 dy = target.y - owner->y;
    f32 dz = target.z - owner->z;
    f32 distance = sqrtf(dx * dx + dy * dy + dz * dz);

    f32 speed = sqrtf(self->velocity_x * self->velocity_x + self->velocity_y * self->velocity_y +
                      self->velocity_z * self->velocity_z);
    f32 inv_speed = 1.0f / speed;
    UnkVec3f dir(inv_speed * self->velocity_x, inv_speed * self->velocity_y, inv_speed * self->velocity_z);

    owner = self->owner;
    f32 origin_x = owner->x;
    f32 origin_y = owner->y;
    target.z = 0.0f;
    if (UnkSphereBlockerHolder_l45be3c* holder = self->blocker) {
        if (UnkSpherePosed_l45be3c* blocker = holder->actor) {
            f32 blocker_x = blocker->x;
            f32 blocker_y = blocker->y;
            bool crosses = (origin_x > blocker_x && target.x < blocker_x) ||
                           (origin_x < blocker_x && target.x > blocker_x);
            if (crosses && target.y < blocker_y + 410.0f + self->radius) {
                target.x = blocker_x;
                target.y = self->radius + 410.0f + blocker_y;
                target.z = 0.0f;
            }
        }
    }

    // The direction to the target in the horizontal plane.
    f32 to_x = target.x - origin_x;
    f32 to_y = target.y - origin_y;
    f32 to_z = 0.0f;
    f32 inv_length = 1.0f / (sqrtf(to_x * to_x + to_y * to_y + to_z * to_z) + 0.0001f);
    to_x *= inv_length;
    to_y *= inv_length;
    to_z = inv_length * to_z;

    f32 angle = acosf(ClampUnit(dir.x * to_x + dir.y * to_y + dir.z * to_z));

    f32 turn_speed = self->def->turn_speed;
    f32 ramped = turn_speed * self->time * 0.5f;
    f32 limit = distance < 500.0f ? turn_speed : ramped;
    limit = limit < turn_speed ? limit : turn_speed;
    f32 step = angle > limit * dt ? limit * dt : angle;

    // Which side of the velocity the target is on: cross(dir, down) against the direction.
    f32 up_x = 0.0f;
    f32 up_y = 0.0f;
    f32 up_z = 1.0f;
    f32 down_x = -up_x;
    f32 down_y = -up_y;
    f32 down_z = -up_z;
    f32 side_x = dir.y * down_z - dir.z * down_y;
    f32 side_y = dir.z * down_x - dir.x * down_z;
    f32 side_z = dir.x * down_y - dir.y * down_x;
    f32 side = to_x * side_x + to_y * side_y + to_z * side_z;

    UnkVec3f turned = fn_710007a390(dir, side < 0.0f ? -step : step);
    self->velocity_x = speed * turned.x;
    self->velocity_y = speed * turned.y;
    self->velocity_z = speed * 0.0f;
}
