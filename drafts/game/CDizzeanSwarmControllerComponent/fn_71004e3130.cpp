// CDizzeanSwarmControllerComponent's unit (tools/tu_map.py unit 770): moves one group's members. A member
// whose hop is over starts a new one towards a random point around the group, with its speed, the hop's
// effect and timing; every member then advances along its hop, and the group's centre becomes the
// members' mean position. Names and layouts are placeholders.
#include <math.h>
#include <stdlib.h>

#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/UnkTU1159.h"
#include "game/UnkTU1161.h"
#include "game/UnkTU2560.h"
#include "types.h"

using base::reflection::CClass;

// As src/game_lo/fn_71007455dc.cpp defines it.
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
u8 fn_71007452a4(UnkTarget* target);
void fn_71007456fc(UnkTarget* target, u8 value);
void fn_7100745f10(UnkParent_71007455dc* parent, f32 radius);

extern u8 g_unk_7101649c60[];
extern u8 g_unk_7101649c68[];

// A member of a group.
struct UnkDizzeanMember_l4e3130 {
    u8 pad_0x0[0x28];
    u32 field_0x28;
    UnkVec3Result position;   // 0x2c
    UnkVec3Result direction;  // 0x38
    f32 field_0x44;           // the share of the group's radius it circles at
    f32 field_0x48;           // its angle around the group
    u8 pad_0x4c[0x64 - 0x4c];
    f32 field_0x64;
    u8 pad_0x68[0x158 - 0x68];
    f32 duration;   // 0x158
    f32 timer;      // 0x15c
    f32 speed;      // 0x160
    f32 slowdown;   // 0x164
    u8 pad_0x168[0x170 - 0x168];
    u8 field_0x170;  // an angle, wrapped to (-128, 128]
    u8 pad_0x171[3];
    UnkVec3Result start;  // 0x174
};

// A group of members (0x148 bytes).
struct UnkDizzeanGroup_l4e3130 {
    u8 pad_0x0[0x44];
    f32 radius;  // 0x44
    u8 pad_0x48[0xa0 - 0x48];
    UnkVec3Result center;  // 0xa0
    u8 pad_0xac[0xb8 - 0xac];
    UnkDizzeanMember_l4e3130** members;  // 0xb8
    u32 memberCount;                     // 0xc0
    u8 pad_0xc4[0x148 - 0xc4];
};

struct UnkKeyHolder_l4e3130 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkBillboard_l4e3130 {
    void** vtable;
    u8 pad_0x8[0xc8];
    UnkKeyHolder_l4e3130* field_0xd0;
};

struct UnkDizzeanSwarm_l4e3130 {
    u8 pad_0x0[0x10];
    UnkBillboard_l4e3130* field_0x10;
    u8 pad_0x18[0x50 - 0x18];
    UnkDizzeanGroup_l4e3130* groups;  // 0x50
    u8 pad_0x58[0x110 - 0x58];
    s32 count;  // 0x110

    // The group at `i`, or null past the end.
    UnkDizzeanGroup_l4e3130* At(s32 i) {
        if (i < count && groups)
            return &groups[i];
        return nullptr;
    }
};

struct UnkDizzeanData_l4e3130 {
    u8 pad_0x0[0x10];
    UnkDizzeanSwarm_l4e3130* swarm;  // 0x10
};

struct UnkDizzeanController_l4e3130 {
    u8 pad_0x0[0xe8];
    UnkDizzeanData_l4e3130* data;  // 0xe8
};

// The hop's effect: its frame count and play rate.
struct UnkHopEffect_l4e3130 {
    u8 pad_0x0[0x8];
    u32 frames;  // 0x8
    u8 pad_0xc[0x20 - 0xc];
    f32 rate;  // 0x20
};

inline UnkVec3Result operator+(const UnkVec3Result& a, const UnkVec3Result& b) {
    return UnkVec3Result(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline UnkVec3Result operator-(const UnkVec3Result& a, const UnkVec3Result& b) {
    return UnkVec3Result(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline UnkVec3Result operator*(const UnkVec3Result& a, f32 s) {
    return UnkVec3Result(a.x * s, a.y * s, a.z * s);
}

inline f32 Length(const UnkVec3Result& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline UnkVec3Result Normalized(const UnkVec3Result& v) {
    return v * (1.0f / (Length(v) + 0.0001f));
}

inline f32 Min(f32 a, f32 b) {
    return a < b ? a : b;
}

inline f32 Random01() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

inline f32 RandomRange(f32 lo, f32 hi) {
    return (hi - lo) * Random01() + lo;
}

// From `from` to `to` as `t` goes from 0 to 1, held at either end.
inline f32 Lerp(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return t * to + (1.0f - t) * from;
}

inline UnkTarget* AsTarget(UnkDizzeanMember_l4e3130* member) {
    return reinterpret_cast<UnkTarget*>(member);
}

inline UnkStruct_71007455dc* AsNode(UnkDizzeanMember_l4e3130* member) {
    return reinterpret_cast<UnkStruct_71007455dc*>(member);
}

inline UnkParent_71007455dc* AsParent(UnkDizzeanGroup_l4e3130* group) {
    return reinterpret_cast<UnkParent_71007455dc*>(group);
}

inline bool IsActive(UnkDizzeanMember_l4e3130* member) {
    return fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(member));
}

void fn_71004e3130(UnkDizzeanController_l4e3130* self, s32 index, f32 dt) {
    UnkDizzeanSwarm_l4e3130* swarm = self->data ? self->data->swarm : nullptr;
    UnkDizzeanGroup_l4e3130* group = swarm->At(index);
    if (group->memberCount == 0)
        return;

    const UnkVec3Result gravity(0.0f, -50.0f, 0.0f);
    f32 sumX = 0.0f;
    f32 sumY = 0.0f;
    f32 sumZ = 0.0f;
    f32 count = 0.0f;
    for (u64 i = 0; i < group->memberCount; ++i) {
        UnkDizzeanMember_l4e3130* member = group->members[i];
        if (!IsActive(member) || member->field_0x28 != 0)
            continue;
        UnkVec3Result position = member->position;
        UnkVec3Result home = fn_71007455dc(AsNode(member));

        if (member->timer <= 0.0f) {
            // A new hop towards a random point around the group.
            member->field_0x170 = fn_71007452a4(AsTarget(member));
            f32 duration = RandomRange(0.5f, 0.75f);
            member->duration = duration;
            member->timer = duration;
            UnkVec3Result fall = gravity * duration;
            f32 radius = group->radius + fall.y;
            f32 orbit = member->field_0x44 * radius;
            f32 angle = member->field_0x48;
            f32 height = RandomRange(-25.0f, 25.0f);
            UnkVec3Result around(cosf(angle) * orbit, fall.y * 0.25f + sinf(angle) * orbit, height);
            f32 spread = Min(radius - orbit, 50.0f);
            f32 distance = RandomRange(0.0f, spread);
            f32 turn = RandomRange(0.0f, 6.2831855f);
            UnkVec3Result point = around + UnkVec3Result(cosf(turn) * distance, sinf(turn) * distance, 0.0f);
            // The point's heading, computed and not used, as in the original.
            f32 heading = acosf(point.x / Length(point));
            (void)heading;

            UnkVec3Result delta = point + fn_7100745e24(AsParent(group)) - home;
            if (delta.y > -0.75f)
                delta = delta - fall;
            f32 speed = Min(Length(delta), 100.0f) * 2.0f / duration;
            member->speed = speed;
            UnkVec3Result direction = Normalized(delta);
            member->direction = direction;
            member->slowdown = -speed / duration;
            member->start = home;

            UnkBillboard_l4e3130* obj = self->data->swarm->field_0x10;
            UnkBillboard_l4e3130* billboard =
                obj && fn_710009d50c(reinterpret_cast<const CClass* (*)(UnkBillboard_l4e3130*)>(obj->vtable[0])(obj),
                                     fn_710072be34())
                    ? obj
                    : nullptr;
            void* manager = g_unk_71726da048;
            if (!manager) {
                manager = fn_71000839a0(0xe0, 0x3f);
                fn_7100e1bca4(manager);
                g_unk_71726da048 = manager;
            }
            u64 key = billboard->field_0xd0->field_0x30;
            UnkHopEffect_l4e3130* effect = static_cast<UnkHopEffect_l4e3130*>(
                fn_7100e1cb50(manager, &key, direction.y > -0.75f ? g_unk_7101649c60 : g_unk_7101649c68));
            f32 rate = (f32)effect->frames / (duration * effect->rate * 30.0f);
            fn_7100745440(AsTarget(member), effect, direction.y > -0.75f ? 9 : 0);
            member->field_0x64 = rate;
        }

        f32 duration = member->duration;
        f32 timer = member->timer - dt;
        member->timer = timer;
        f32 t = duration - timer;
        f32 progress = t / duration;
        s32 from = member->field_0x170;
        if (from > 128)
            from -= 256;
        UnkVec3Result direction = member->direction;
        UnkVec3Result pos = member->start;
        f32 tilt;
        if (direction.y > -0.75f) {
            // Thrown: decelerating along the direction, pulled down by gravity.
            f32 travelled = member->speed * t + 0.5f * member->slowdown * t * t;
            pos = member->start + direction * travelled + gravity * t * 0.5f;
            if (progress > 0.5f)
                tilt = Lerp(direction.x * -22.0f, 0.0f, (progress - 0.5f) * 2.0f);
            else
                tilt = Lerp((f32)from, direction.x * -22.0f, progress * 2.0f);
        } else {
            // Dropping: at half the speed.
            pos = member->start + direction * member->speed * t * 0.5f;
            tilt = Lerp((f32)from, direction.x * -22.0f, progress);
        }
        fn_710074566c(AsNode(member), &pos.x);
        fn_71007456fc(AsTarget(member), (s32)tilt);
        sumX += position.x;
        sumY += position.y;
        sumZ += position.z;
        count += 1.0f;
    }

    if (count > 0.0f) {
        UnkVec3Result mean(sumX / count, sumY / count, sumZ / count);
        f32 farthest = 0.0f;
        for (u64 i = 0; i < group->memberCount; ++i) {
            UnkDizzeanMember_l4e3130* member = group->members[i];
            if (!IsActive(member))
                continue;
            UnkVec3Result d = member->position - mean;
            f32 distance = d.x * d.x + d.y * d.y + d.z * d.z;
            farthest = farthest > distance ? farthest : distance;
        }
        group->center = mean + fn_7100745e24(AsParent(group));
        fn_7100745f10(AsParent(group), sqrtf(farthest));
    }
}
