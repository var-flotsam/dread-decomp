// CRedenkiSwarmControllerComponent's unit: one movement step of a swarm member. Turns its heading (0 to 255
// around the circle) towards its direction by at most the turn rate, speeds it up within 1.1 times the
// controller's speed, moves it along its old heading, and restarts it once its time runs out. Names and
// layouts are placeholders.
#include <math.h>

#include "game/UnkXformNode.h"
#include "types.h"

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

UnkVec3f RotateVec3AboutZ(const UnkVec3f& v, f32 angle);
struct UnkStruct_71007455dc;
UnkVec3Result fn_71007455dc(UnkStruct_71007455dc* self);
void fn_710074566c(UnkStruct_71007455dc* self, const f32* pos);
struct UnkTarget;
void fn_71007456fc(UnkTarget* target, u8 value);
struct UnkRedenkiController_b55;
void fn_7100655280(UnkRedenkiController_b55* self, UnkTarget* target);

__attribute__((visibility("hidden"))) extern const UnkVec3f g_unk_710164a3fc;
extern f32 g_unk_7101a5aed4;  // the turn rate

struct UnkSwarmMember_l65a514 {
    u8 pad_0x0[0x38];
    f32 direction[3];  // 0x38
    u8 pad_0x44[0x118];
    f32 time;          // 0x15c
    f32 speed;         // 0x160
    f32 acceleration;  // 0x164
    f32 heading;       // 0x168
    f32 jerk;          // 0x16c
};
static_assert(__builtin_offsetof(UnkSwarmMember_l65a514, jerk) == 0x16c, "member layout");

struct UnkSwarmController_l65a514;

struct UnkSwarmControllerVtable_l65a514 {
    void* pad_0x0[0x208 / 8];
    f32 (*GetSpeed)(UnkSwarmController_l65a514* self);  // 0x208
};

struct UnkSwarmController_l65a514 {
    UnkSwarmControllerVtable_l65a514* vtable;
};

static inline f32 SafeAcos_l65a514(f32 c) {
    if (c >= 1.0f)
        return 0.0f;
    if (c <= -1.0f)
        return 3.1415927f;
    return acosf(c);
}

// `from` moved `distance` along `dir`.
static inline UnkVec3Result Advanced_l65a514(const UnkVec3Result& from, const UnkVec3f& dir, f32 distance) {
    return UnkVec3Result(distance * dir.x + from.x, distance * dir.y + from.y, distance * dir.z + from.z);
}

void fn_710065a514(UnkSwarmController_l65a514* self, UnkSwarmMember_l65a514* member, f32 dt) {
    f32 heading = member->heading;
    UnkVec3f forward = RotateVec3AboutZ(g_unk_710164a3fc, heading * 6.2831855f / 256.0f);

    f32 x = member->direction[0];
    f32 y = member->direction[1];
    f32 z = member->direction[2];
    UnkVec3f right(1.0f, 0.0f, 0.0f);
    f32 dot = x * -right.x + y * -right.y + z * -right.z;
    f32 angle = SafeAcos_l65a514(dot / sqrtf(x * x + y * y + z * z));
    if (y > 0.0f)
        angle = 6.2831855f - angle;
    f32 target = angle * 256.0f / 6.2831855f;

    f32 maxTurn = fabsf(g_unk_7101a5aed4 * dt);
    f32 diff = target - heading;
    if (diff > 128.0f)
        diff -= 256.0f;
    else if (diff < -128.0f)
        diff += 256.0f;
    if (!(fabsf(diff) < maxTurn)) {
        int sign = (diff > 0.0f) - (diff < 0.0f);
        target = heading + maxTurn * (f32)sign;
        if (target < 0.0f)
            target += 256.0f;
        if (target > 256.0f)
            target -= 256.0f;
    }
    fn_71007456fc(reinterpret_cast<UnkTarget*>(member), target);
    f32 acceleration = member->acceleration + member->jerk * dt;
    member->heading = target;
    member->acceleration = acceleration;

    f32 speed = member->speed + member->acceleration * dt;
    f32 time = member->time;
    f32 maxSpeed = self->vtable->GetSpeed(self) * 1.1f;
    if (speed > maxSpeed)
        speed = maxSpeed;
    else if (speed < -maxSpeed)
        speed = -maxSpeed;
    member->speed = speed;

    f32 step = speed * dt;
    UnkVec3Result pos = Advanced_l65a514(fn_71007455dc(reinterpret_cast<UnkStruct_71007455dc*>(member)), forward, step);
    fn_710074566c(reinterpret_cast<UnkStruct_71007455dc*>(member), &pos.x);

    if (time <= 0.0f)
        fn_7100655280(reinterpret_cast<UnkRedenkiController_b55*>(self), reinterpret_cast<UnkTarget*>(member));
}
