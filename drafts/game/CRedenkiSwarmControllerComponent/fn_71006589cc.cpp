// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py, unit 977): steers one swarm member towards its
// place in the group: turns its heading (in 1/255 turns at +0x168) towards the place, at most the
// controller's turn rate, eases its speed (+0x160) and moves it. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkTU1159.h"
#include "game/UnkTU1161.h"
#include "game/UnkXformNode.h"
#include "types.h"

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkStruct_71007455dc;
UnkVec3Result fn_71007455dc(UnkStruct_71007455dc* self);
void fn_710074566c(UnkStruct_71007455dc* self, const f32* pos);
void fn_71007456fc(UnkTarget* target, u8 value);
UnkVec3f fn_710007a390(const UnkVec3f& v, f32 angle);

__attribute__((visibility("hidden"))) extern const UnkVec3f g_unk_710164a3fc;

struct UnkRedenkiGroup_l6589cc {
    u8 pad_0x0[0x44];
    f32 spread;  // 0x44
    u8 pad_0x48[0x88 - 0x48];
    UnkVec3f direction;  // 0x88
};

struct UnkRedenkiMember_l6589cc {
    u8 pad_0x0[0x10];
    UnkRedenkiGroup_l6589cc* group;  // 0x10
    u8 pad_0x18[0x38 - 0x18];
    f32 field_0x38;
    f32 field_0x3c;
    u8 pad_0x40[0x44 - 0x40];
    f32 field_0x44;
    u8 pad_0x48[0x64 - 0x48];
    f32 field_0x64;
    u8 pad_0x68[0x160 - 0x68];
    f32 speed;  // 0x160
    u8 pad_0x164[4];
    f32 heading;  // 0x168
};

struct UnkRedenkiTuning_l6589cc {
    u8 pad_0x0[0x46c];
    f32 farDistance;  // 0x46c
    u8 pad_0x470[0x494 - 0x470];
    f32 minMoveRatio;  // 0x494
    f32 maxMoveRatio;  // 0x498
};

struct UnkRedenkiController_l6589cc {
    void** vtable;
    u8 pad_0x8[0x560 - 0x8];
    UnkRedenkiTuning_l6589cc* tuning;  // 0x560

    f32 MaxSpeed() { return reinterpret_cast<f32 (*)(UnkRedenkiController_l6589cc*)>(vtable[0x208 / 8])(this); }
    f32 Acceleration() { return reinterpret_cast<f32 (*)(UnkRedenkiController_l6589cc*)>(vtable[0x210 / 8])(this); }
    f32 TurnRate(f32 angle) {
        return reinterpret_cast<f32 (*)(UnkRedenkiController_l6589cc*, f32)>(vtable[0x340 / 8])(this, angle);
    }
};
static_assert(__builtin_offsetof(UnkRedenkiController_l6589cc, tuning) == 0x560, "UnkRedenkiController_l6589cc layout");

template <class V>
static inline f32 Length(const V& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

template <class A, class B>
static inline f32 AngleBetween(const A& a, const B& b) {
    f32 ia = 1.0f / (Length(a) + 0.0001f);
    UnkVec3f na(a.x * ia, a.y * ia, a.z * ia);
    f32 ib = 1.0f / (Length(b) + 0.0001f);
    UnkVec3f nb(b.x * ib, b.y * ib, b.z * ib);
    f32 d = na.x * nb.x + na.y * nb.y + na.z * nb.z;
    d = !(d >= 1.0f) ? d : 1.0f;
    d = !(d < -1.0f) ? d : -1.0f;
    return acosf(d);
}

static inline s32 Sign(f32 v) {
    return (v > 0.0f) - (v < 0.0f);
}

void fn_71006589cc(UnkRedenkiController_l6589cc* self, UnkRedenkiMember_l6589cc* member, f32 dt) {
    f32 farDistance = self->tuning->farDistance;
    UnkRedenkiGroup_l6589cc* group = member->group;
    f32 spread = group->spread;
    UnkVec3Result pos = fn_71007455dc(reinterpret_cast<UnkStruct_71007455dc*>(member));
    UnkVec3Result center = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(group));
    f32 targetX = spread * member->field_0x38 * member->field_0x44 + center.x;
    f32 targetY = spread * member->field_0x3c * member->field_0x44 + center.y;
    f32 offset = fabsf(targetX - pos.x) + fabsf(targetY - pos.y);
    if (offset + 0.001f >= 0.0f && offset - 0.001f <= 0.0f)
        return;

    f32 dx = targetX - pos.x;
    f32 dy = targetY - pos.y;
    f32 current = member->heading;
    f32 facingAngle = current * 6.2831855f / 255.0f;
    UnkVec3f facing = fn_710007a390(g_unk_710164a3fc, facingAngle);
    UnkVec3f toTarget(dx, dy, 0.0f);
    f32 distance = Length(toTarget);
    f32 inv = 1.0f / distance;
    UnkVec3f dir(toTarget.x * inv, toTarget.y * inv, toTarget.z * inv);
    UnkVec3f groupDir = group->direction;
    f32 along = dir.x * groupDir.x + dir.y * groupDir.y + dir.z * groupDir.z;
    if (along < 0.0f) {
        f32 side = Sign(dir.x * groupDir.y - dir.y * groupDir.x);
        f32 angle = 3.1415927f - AngleBetween(dir, groupDir);
        angle = !(angle >= 0.7853982f) ? angle : 0.7853982f;
        dir = fn_710007a390(groupDir, -(angle * side));
    }

    f32 turnRate = self->TurnRate(AngleBetween(dir, facing));
    f32 heading = AngleBetween(dir, UnkVec3f(1.0f, 0.0f, 0.0f));
    if (dir.y < 0.0f)
        heading = 6.2831855f - heading;
    heading = heading * 255.0f / 6.2831855f;
    f32 turn = heading - current;
    if (turn > 128.0f)
        turn -= 256.0f;
    else if (turn < -128.0f)
        turn += 256.0f;
    f32 maxTurn = turnRate * dt;
    if (!(fabsf(turn) < fabsf(maxTurn))) {
        f32 turned = current + fabsf(maxTurn) * Sign(turn);
        turned = turned < 0.0f ? turned + 256.0f : turned;
        heading = turned > 256.0f ? turned - 256.0f : turned;
    }

    f32 factor = 0.5f;
    if (!(along < 0.0f)) {
        f32 ratio = distance / farDistance;
        if (ratio > factor)
            factor = !(ratio >= 1.0f) ? ratio : 1.0f;
    }
    f32 wanted = factor * self->MaxSpeed();
    f32 speed = member->speed;
    f32 maxChange = fabsf(self->Acceleration() * dt);
    if (!(maxChange > fabsf(wanted - speed)))
        wanted = speed + maxChange * Sign(wanted - speed);

    f32 move = wanted * dt;
    if (farDistance < distance) {
        f32 boost = along < 0.0f ? 0.9f : 1.1f;
        move *= boost;
    }
    UnkVec3f forward = fn_710007a390(g_unk_710164a3fc, facingAngle);
    if (move > distance)
        move = distance;
    pos.x = forward.x * move + pos.x;
    pos.y = forward.y * move + pos.y;
    pos.z = forward.z * move + pos.z;
    f32 maxMove = self->MaxSpeed() * dt;
    if (maxMove > 0.0f) {
        f32 ratio = move / maxMove;
        UnkRedenkiTuning_l6589cc* tuning = self->tuning;
        f32 high = tuning->maxMoveRatio;
        f32 low = tuning->minMoveRatio;
        f32 capped = ratio >= high ? high : ratio;
        member->field_0x64 = ratio > low ? capped : low;
    }
    fn_710074566c(reinterpret_cast<UnkStruct_71007455dc*>(member), &pos.x);
    member->heading = heading;
    member->speed = wanted;
    fn_71007456fc(reinterpret_cast<UnkTarget*>(member), heading);
}
