// CFlockingSwarmControllerComponent unit (unit 1158 of tools/tu_map.py): steers one swarm member's velocity
// towards its desired velocity, turning by at most a tunable rate per second, scales it to the swarm's
// speed and moves the member by it. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkTU1161.h"
#include "game/UnkTU83.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

struct UnkBillboardEntry_be;

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkVec3_cs {
    f32 x, y, z;
};

UnkVec3f RotateVec3AboutZ(const UnkVec3f& v, f32 angle);
void fn_7100745e74(UnkBillboardEntry_be* self, const CVector3D* target, bool keep_items);

struct UnkSwarmMember_l741cc8 {
    u8 pad_0x0[0x24];
    bool active;  // 0x24
    u8 pad_0x25[0x46];
    bool fast;  // 0x6b
    bool slow;  // 0x6c
    u8 pad_0x6d[3];
    UnkVec3f velocity;  // 0x70
    u8 pad_0x7c[0xc];
    f32 forward_x;  // 0x88
    f32 forward_y;  // 0x8c
    f32 forward_z;  // 0x90
    u8 pad_0x94[0xb4];
};
static_assert(sizeof(UnkSwarmMember_l741cc8) == 0x148, "member size");

struct UnkSwarm_l741cc8 {
    u8 pad_0x0[0x50];
    UnkSwarmMember_l741cc8* members;  // 0x50
    u8 pad_0x58[0xb8];
    s32 count;  // 0x110
};

struct UnkSwarmHolder_l741cc8 {
    u8 pad_0x0[0x10];
    UnkSwarm_l741cc8* swarm;  // 0x10
};

struct UnkSwarmTunables_l741cc8 {
    u8 pad_0x0[0x438];
    f32 turn_rate;  // 0x438
    u8 pad_0x43c[0x14];
    bool check_reverse;  // 0x450
};

struct UnkFlockingSwarm_l741cc8 {
    u8 pad_0x0[0xe8];
    UnkSwarmHolder_l741cc8* holder;  // 0xe8
    u8 pad_0xf0[0xe0];
    bool has_leaders;  // 0x1d0
    u8 pad_0x1d1[3];
    s32 leader;  // 0x1d4
    s32 follower;  // 0x1d8
    u8 pad_0x1dc[0xe0];
    f32 speed;  // 0x2bc
    u8 pad_0x2c0[0x68];
    UnkSwarmTunables_l741cc8* tunables;  // 0x328
    u8 pad_0x330[4];
    f32 field_0x334;
    u8 pad_0x338[0x30];
    u8 desired[0x28];  // 0x368
    u8 pad_0x390[4];
    bool constrained;  // 0x394
};

void fn_7100742290(UnkFlockingSwarm_l741cc8* self, UnkSwarmMember_l741cc8* member, UnkVec3Result* pos);

// Whether v is outside [-0.001, 0.001] (or not a number).
static inline bool NotNearZero(f32 v) {
    return !(v + 0.001f >= 0.0f) || !(v - 0.001f <= 0.0f);
}

static inline f32 LengthSq(const UnkVec3f& v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

void fn_7100741cc8(UnkFlockingSwarm_l741cc8* self, s32 index, f32 dt) {
    // As the unit's other functions read it: a null holder is taken as the swarm itself.
    UnkSwarm_l741cc8* swarm = reinterpret_cast<UnkSwarm_l741cc8*>(self->holder);
    if (swarm)
        swarm = self->holder->swarm;
    UnkSwarmMember_l741cc8* member =
        index < swarm->count && swarm->members ? &swarm->members[index] : nullptr;
    if (member == nullptr || !member->active)
        return;

    const UnkVec3_cs* desired = fn_71000cc374(self->desired, index);
    UnkVec3f vel = member->velocity;
    f32 dx = desired->x - vel.x;
    f32 dy = desired->y - vel.y;
    f32 dz = desired->z - vel.z;
    if (NotNearZero(dx) || NotNearZero(dy) || NotNearZero(dz)) {
        f32 max_turn = self->tunables->turn_rate;
        f32 fx = member->forward_x;
        f32 fy = member->forward_y;
        f32 fz = member->forward_z;
        f32 forward_len = sqrtf(fx * fx + fy * fy + fz * fz);
        max_turn *= dt;
        if (self->tunables->check_reverse) {
            f32 inv = 1.0f / (forward_len + 0.0001f);
            f32 cos_forward = (fx * inv * vel.x + fy * inv * vel.y + fz * inv * vel.z) / sqrtf(LengthSq(vel));
            if (cos_forward < -0.5f) {
                max_turn *= 9.0f;
                self->field_0x334 = 0.1f;
            }
        }

        f32 dot = dx * vel.x + dy * vel.y + dz * vel.z;
        f32 diff_len_sq = dz * dz + (dx * dx + dy * dy);
        f32 cos_angle = dot / (sqrtf(diff_len_sq) * sqrtf(LengthSq(vel)));
        f32 angle;
        if (cos_angle >= 1.0f)
            angle = 0.0f;
        else if (cos_angle <= -1.0f)
            angle = 3.14159265f;
        else
            angle = acosf(cos_angle);

        f32 inv = 1.0f / (sqrtf(diff_len_sq) + 0.0001f);
        vel.x = dx * inv * 10.0f * dt + vel.x;
        vel.y = dy * inv * 10.0f * dt + vel.y;
        vel.z = dz * inv * 10.0f * dt + vel.z;
        if (!(angle < max_turn)) {
            // Turn the other way when the difference lies on the right of the velocity (about z).
            f32 side = dx * (vel.y - vel.z * 0.0f) + dy * (vel.z * 0.0f - vel.x) + dz * (vel.x * 0.0f - vel.y * 0.0f);
            if (side > 0.0f)
                vel = RotateVec3AboutZ(vel, -max_turn);
            else
                vel = RotateVec3AboutZ(vel, max_turn);
        }

        f32 speed = self->speed;
        if (member->fast)
            speed *= 1.2f;
        else if (member->slow)
            speed *= 0.9f;
        if (self->has_leaders) {
            if (self->leader == index)
                speed *= 1.2f;
            else if (self->follower == index)
                speed *= 0.8f;
        }

        f32 scale = 1.0f / (sqrtf(LengthSq(vel)) + 0.0001f);
        f32 x = speed * (vel.x * scale);
        f32 y = speed * (scale * vel.y);
        f32 z = speed * (scale * vel.z);
        vel.x = x;
        vel.y = y;
        vel.z = z;
        member->velocity = vel;
    }

    UnkVec3Result pos = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(member));
    pos.x = vel.x * dt + pos.x;
    pos.y = vel.y * dt + pos.y;
    pos.z = 0.0f;
    if (self->constrained)
        fn_7100742290(self, member, &pos);
    fn_7100745e74(reinterpret_cast<UnkBillboardEntry_be*>(member), reinterpret_cast<const CVector3D*>(&pos), false);
}
