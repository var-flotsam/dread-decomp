// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py unit 977): scatters a group's members. The point the
// group stands at is found by walking the tuned distance (+0x46c of the tuning) back along the path from the
// group's node, blending the nodes' values on the way (fn_7100745f10); the members are then spread on a ring of
// the group's radius around it, facing away from the walk, each with random speed, phase and height. Names and
// layouts are placeholders.
#include <math.h>
#include <stdlib.h>

#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

// As src/game/CDaivoSwarmControllerComponent/CDaivoSwarmControllerComponent_Swarm_ovb0012.cpp declares it.
struct UnkDaivoFade_b0012;
struct UnkFadeGroup_b0012;

// As src/game_s1/fn_7100078c9c.cpp defines it.
UnkVec3f fn_710007a390(const UnkVec3f& v, f32 angle);

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_Start_b55.cpp declares it.
struct UnkTarget;
void fn_71007456fc(UnkTarget* target, u8 value);

// A node of the path: its area and index on it.
struct UnkPathKey_l64f42c {
    u32 area;
    u32 index;
};

struct UnkPathNode_l64f42c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CVector3D* GetPos();
    u8 pad_0x8[0x14 - 0x8];
    f32 value;  // 0x14
};

struct UnkScatterMember_l64f42c {
    u8 pad_0x0[0x2c];
    CVector3D position;  // 0x2c
    u8 pad_0x38[0x44 - 0x38];
    f32 speed;  // 0x44
    f32 phase;  // 0x48
    u8 pad_0x4c[0x6c - 0x4c];
    f32 field_0x6c;
    f32 field_0x70;
    u8 pad_0x74[0x168 - 0x74];
    f32 sectorValue;  // 0x168
    u8 pad_0x16c[0x170 - 0x16c];
    u8 sector;  // 0x170
    u8 field_0x171;
};

struct UnkScatterGroup_l64f42c {
    u8 pad_0x0[0x40];
    f32 radius;  // 0x40
    u8 pad_0x44[0x48 - 0x44];
    UnkPathKey_l64f42c prevKey;  // 0x48
    UnkPathKey_l64f42c key;  // 0x50
    u8 pad_0x58[0x64 - 0x58];
    f32 distance;  // 0x64
    u8 pad_0x68[0x88 - 0x68];
    CVector3D dir;  // 0x88
    u8 pad_0x94[0xb8 - 0x94];
    UnkScatterMember_l64f42c** members;  // 0xb8
    u32 memberCount;  // 0xc0

    UnkScatterMember_l64f42c* At(u64 i) { return i < memberCount ? members[i] : nullptr; }
};

struct UnkScatterTuning_l64f42c {
    u8 pad_0x0[0x468];
    f32 field_0x468;
    f32 spread;  // 0x46c
};

struct UnkScatterSwarm_l64f42c {
    u8 pad_0x0[0x560];
    UnkScatterTuning_l64f42c* tuning;  // 0x560
};

void fn_71007397a0(UnkScatterSwarm_l64f42c* self, UnkScatterGroup_l64f42c* group);
UnkPathNode_l64f42c* fn_7100749e44(UnkScatterSwarm_l64f42c* self, const UnkPathKey_l64f42c* key);
void fn_7100745f10(UnkScatterGroup_l64f42c* group, f32 value);

static inline f32 Random01() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandomRange(f32 min, f32 max) { return min + (max - min) * Random01(); }

// The angle between two directions.
static inline f32 Angle(const CVector3D& a, const CVector3D& b) {
    f32 c = Dot(Normalized(a), Normalized(b));
    c = !(c >= 1.0f) ? c : 1.0f;
    c = !(c < -1.0f) ? c : -1.0f;
    return acosf(c);
}

static inline bool Near(f32 a, f32 b) { return a - 0.001f <= b && a + 0.001f >= b; }

// The value between two nodes' values at `t` (1 is the first node's).
static inline f32 Blend(f32 t, f32 first, const UnkPathNode_l64f42c* second) {
    if (t >= 1.0f)
        return first;
    if (t < 0.0f)
        return second->value;
    return t * first + (1.0f - t) * second->value;
}

// Where the group stands: the tuned distance walked back along the path from the group's node, blending the
// nodes' values on the way (fn_7100745f10). The zero vector when the path is too short.
static inline CVector3D WalkBack(UnkScatterSwarm_l64f42c* self, UnkScatterGroup_l64f42c* group) {
    f32 spread = self->tuning->spread;
    u32 last = group->prevKey.index;

    UnkPathKey_l64f42c key = group->key;
    const CVector3D* head = fn_7100749e44(self, &key)->GetPos();
    CVector3D offset = *head - group->dir * group->distance;
    key = group->key;
    const CVector3D* end = fn_7100749e44(self, &key)->GetPos();
    UnkPathKey_l64f42c prev = group->prevKey;
    const CVector3D* start = fn_7100749e44(self, &prev)->GetPos();
    f32 remain = Distance(*end, *start) - group->distance;
    if (spread <= remain) {
        offset = -(group->dir * spread);
        key = group->key;
        f32 first = fn_7100749e44(self, &key)->value;
        key = group->prevKey;
        UnkPathNode_l64f42c* second = fn_7100749e44(self, &key);
        f32 value = Blend(1.0f - spread / remain, first, second);
        fn_7100745f10(group, value);
    }

    f32 left = spread - remain;
    if (last != 0) {
        while (left > 0.0f) {
            key = UnkPathKey_l64f42c();
            key.index = last;
            CVector3D from = *fn_7100749e44(self, &key)->GetPos();
            prev = key;
            prev.index = key.index - 1;
            CVector3D segment = *fn_7100749e44(self, &prev)->GetPos() - from;
            f32 length = Length(segment);
            if (left <= length) {
                CVector3D point = {from.x + left * (segment.x / length), from.y + left * (segment.y / length),
                                   from.z + left * (segment.z / length)};
                f32 first = fn_7100749e44(self, &key)->value;
                UnkPathNode_l64f42c* second = fn_7100749e44(self, &prev);
                f32 value = Blend(1.0f - left / length, first, second);
                offset = point - offset;
                fn_7100745f10(group, value);
            }
            left -= length;
        }
    }
    if (left > 0.0f)
        offset = g_unk_710164779c;
    return offset;
}

void fn_710064f42c(UnkDaivoFade_b0012* fade, UnkFadeGroup_b0012* fadeGroup) {
    const f32 kTwoPi = 6.28318548f;
    UnkScatterSwarm_l64f42c* self = reinterpret_cast<UnkScatterSwarm_l64f42c*>(fade);
    UnkScatterGroup_l64f42c* group = reinterpret_cast<UnkScatterGroup_l64f42c*>(fadeGroup);
    fn_71007397a0(self, group);
    CVector3D offset = WalkBack(self, group);

    f32 radius = group->radius;
    CVector3D facing;
    if (Near(offset.x, g_unk_710164779c.x) && Near(offset.y, g_unk_710164779c.y) &&
        Near(offset.z, g_unk_710164779c.z))
        facing = group->dir;
    else
        facing = -offset;
    CVector3D direction = Normalized(facing);
    f32 angle = Angle(direction, {1.0f, 0.0f, 0.0f});
    if (direction.y < 0.0f)
        angle = kTwoPi - angle;

    u32 count = group->memberCount;
    if (count == 0)
        return;
    u32 sector = (s32)(angle * 256.0f / kTwoPi);
    f32 half = kTwoPi / ((f32)count + (f32)count);
    f32 base = RandomRange(0.0f, kTwoPi);
    for (u64 i = 0; i < count; i++) {
        UnkScatterMember_l64f42c* member = group->At(i);
        f32 memberAngle = base + RandomRange(-half, half);
        f32 speed = RandomRange(0.0f, 0.75f);
        member->speed = speed;
        member->phase = RandomRange(0.0f, kTwoPi);
        UnkVec3f p(radius * speed, 0.0f, 0.0f);
        p = fn_710007a390(p, memberAngle);
        f32 height = RandomRange(-25.0f, 0.0f);
        p.x = offset.x + p.x;
        p.y = offset.y + p.y;
        p.z = offset.z + height;
        member->position.x = p.x;
        member->position.y = p.y;
        member->position.z = p.z;
        member->field_0x6c = member->field_0x70 = self->tuning->field_0x468;
        fn_71007456fc(reinterpret_cast<UnkTarget*>(member), sector);
        base += half + half;
        member->sector = member->field_0x171 = sector;
        member->sectorValue = sector;
    }
}
