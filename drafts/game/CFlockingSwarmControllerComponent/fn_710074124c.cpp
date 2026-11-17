// CFlockingSwarmControllerComponent unit (tools/tu_map.py unit 1158): the flock's steering, one vector
// per member: its heading at cruise speed, away from close flock-mates or else towards their centre,
// away from obstacles and turned off a wall it is about to cross. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU1161.h"
#include "game/UnkTU83.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

// Returned through x8, so the original's result type is not trivially copyable (as
// src/game/CFlockingSwarmControllerComponent/fn_7100739c64.cpp defines it).
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

// As src/game/CCutsceneComponent/CCutsceneComponent_ReflPlace_A5_b70.cpp defines it.
struct UnkVec3_cs {
    f32 x, y, z;
};

// A flock member (0x148 bytes).
struct UnkFlockMember_l74124c {
    u8 pad_0x0[0x24];
    bool active;  // +0x24
    u8 pad_0x25[0x1b];
    f32 field_0x40;
    f32 size;   // +0x44
    u32 group;  // +0x48
    u8 pad_0x4c[0x10];
    f32 field_0x5c;
    f32 field_0x60;
    u8 pad_0x64[0x7];
    bool field_0x6b;
    bool field_0x6c;
    u8 pad_0x6d[0x1b];
    UnkVec3f heading;  // +0x88
    UnkVec3f normal;   // +0x94: the wall it is near
    u8 pad_0xa0[0xa8];
};
static_assert(sizeof(UnkFlockMember_l74124c) == 0x148, "UnkFlockMember_l74124c placeholder layout");

struct UnkFlockMembers_l74124c {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* data;  // +0x10: the class data
    u8 pad_0x18[0x38];
    UnkFlockMember_l74124c* members;  // +0x50
    u8 pad_0x58[0xb8];
    s32 count;  // +0x110

    UnkFlockMember_l74124c* At(s32 i) {
        if (i >= count)
            return nullptr;
        if (!members)
            return nullptr;
        return &members[i];
    }
};

struct UnkFlockMembersOwner_l74124c {
    u8 pad_0x0[0x10];
    UnkFlockMembers_l74124c* members;
};

// The points of the members, one per member (data, count).
struct UnkFlockPoints_l74124c {
    CVector3D* data;
    u32 count;
};

// The behaviours the flock steers by.
struct UnkFlockFlags_l74124c {
    u32 cruise : 1;
    u32 separate : 1;
    u32 gather : 1;
    u32 avoid_obstacles : 1;
    u32 avoid_walls : 1;
};

struct UnkFlockSwarm_l74124c;
struct UnkFlockSwarmVtbl_l74124c {
    u8 pad_0x0[0x2a8];
    f32 (*vf_0x2a8)(UnkFlockSwarm_l74124c* self);  // the cruise speed
    f32 (*vf_0x2b0)(UnkFlockSwarm_l74124c* self);  // the weight towards the centre
    f32 (*vf_0x2b8)(UnkFlockSwarm_l74124c* self);  // the weight away from close flock-mates
};

struct UnkFlockSwarm_l74124c {
    UnkFlockSwarmVtbl_l74124c* vtable;
    u8 pad_0x8[0x88];
    UnkFlockPoints_l74124c obstacles;  // +0x90
    u8 pad_0xa0[0x44];
    f32 obstacle_weight;  // +0xe4
    UnkFlockMembersOwner_l74124c* field_0xe8;
    u8 pad_0xf0[0x248];
    UnkFlockFlags_l74124c flags;  // +0x338

    UnkFlockMembers_l74124c* Members() { return field_0xe8 ? field_0xe8->members : nullptr; }
    UnkFlockMember_l74124c* Member(s32 i) { return &field_0xe8->members->members[i]; }
};
static_assert(__builtin_offsetof(UnkFlockSwarm_l74124c, flags) == 0x338, "UnkFlockSwarm_l74124c placeholder layout");

template <class A, class B>
static inline f32 Dot(const A& a, const B& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
template <class V>
static inline f32 Length(const V& v) { return sqrtf(Dot(v, v)); }
template <class V>
static inline V Scaled(const V& v, f32 s) {
    return V{v.x * s, v.y * s, v.z * s};
}
static inline CVector3D Normalized(const CVector3D& v) { return Scaled(v, 1.0f / (Length(v) + 0.0001f)); }
template <class V>
static inline void Add(CVector3D* v, const V& d) {
    v->x += d.x;
    v->y += d.y;
    v->z += d.z;
}

// The angle of a cosine, clamped to [0, pi].
static inline f32 SafeAcos(f32 c) {
    if (c >= 1.0f)
        return 0.0f;
    if (c > -1.0f)
        return acosf(c);
    return 3.1415927f;
}

void fn_710074124c(UnkFlockSwarm_l74124c* self, const UnkFlockPoints_l74124c* positions,
                   UnkFlockPoints_l74124c* steering) {
    base::reflection::UnkReflObject* data = self->field_0xe8->members->data;
    if (!data || !fn_710009d50c(data->vf_0x0(), fn_710072be34()))
        return;
    for (u64 i = 0; i < steering->count; ++i) {
        UnkFlockMember_l74124c* member = self->Members()->At(i);
        if (!member->active)
            continue;
        member->field_0x6b = false;
        member->field_0x6c = false;
        f32 radius = member->size * 0.7f;
        const CVector3D& pos = positions->data[i];
        f32 radius_sq = radius * radius;

        // The close flock-mates' offsets and all flock-mates' centre.
        f32 offset_x = 0.0f;
        f32 offset_y = 0.0f;
        f32 sum_x = 0.0f;
        f32 sum_y = 0.0f;
        u32 close = 0;
        u32 mates = 0;
        for (u64 j = 0; j < positions->count; ++j) {
            if (i == j)
                continue;
            UnkFlockMember_l74124c* other = self->Member(j);
            if (!other->active || member->group != other->group)
                continue;
            const CVector3D& p = positions->data[j];
            sum_y += p.y;
            CVector3D d = {p.x - pos.x, p.y - pos.y, p.z - pos.z};
            sum_x += p.x;
            ++mates;
            if (Dot(d, d) < radius_sq) {
                offset_x += d.x;
                offset_y += d.y;
                ++close;
            }
        }

        UnkVec3f heading = member->heading;
        CVector3D steer = {0.0f, 0.0f, 0.0f};
        if (self->flags.cruise)
            Add(&steer, Scaled(heading, self->vtable->vf_0x2a8(self)));
        if (close != 0 && self->flags.separate) {
            f32 n = close;
            CVector3D away = Scaled(Normalized({offset_x / n, offset_y / n, 0.0f}), -1.0f);
            if (Dot(heading, away) > 0.0f)
                Add(&steer, Scaled(away, self->vtable->vf_0x2b8(self)));
        } else if (mates != 0 && self->flags.gather) {
            f32 n = mates;
            CVector3D center = {sum_x / n, sum_y / n, 0.0f};
            CVector3D inward = Normalized({center.x - pos.x, center.y - pos.y, 0.0f});
            f32 spread = radius * n * 0.5f;
            UnkVec3Result target = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(member));
            f32 ratio = Length(CVector3D{center.x - target.x, center.y - target.y, center.z - target.z}) / spread;
            if (Dot(inward, heading) > 0.0f) {
                f32 weight = self->vtable->vf_0x2b0(self);
                member->field_0x6b = ratio > 0.7f;
                Add(&steer, Scaled(Scaled(inward, weight), ratio));
                member->field_0x6c = true;
            } else {
                member->field_0x6c = ratio > 0.7f;
                member->field_0x6b = true;
            }
        }
        if (self->flags.avoid_obstacles) {
            CVector3D push = {0.0f, 0.0f, 0.0f};
            for (u32 k = 0; k < self->obstacles.count; ++k) {
                const UnkVec3_cs* o = fn_71000cc374(&self->obstacles, k);
                CVector3D d = {o->x - pos.x, o->y - pos.y, o->z - pos.z};
                if (Dot(d, d) < radius_sq) {
                    const UnkVec3_cs* obstacle = fn_71000cc374(&self->obstacles, k);
                    Add(&push, CVector3D{pos.x - obstacle->x, pos.y - obstacle->y, pos.z - obstacle->z});
                }
            }
            Add(&steer, Scaled(push, self->obstacle_weight));
        }
        if (self->flags.avoid_walls) {
            f32 reach = member->field_0x5c + member->field_0x40 + 20.0f;
            UnkVec3f normal = member->normal;
            if (!(Dot(heading, normal) < 0.0f))
                normal = Scaled(normal, -1.0f);
            if (reach > member->field_0x60) {
                f32 into = Dot(steer, normal);
                if (into > 0.0f) {
                    f32 angle = SafeAcos(into / (Length(normal) * Length(steer)));
                    f32 c = fabsf(cosf(angle));
                    UnkVec3f bounce = Scaled(Scaled(Scaled(normal, 2.0f), Length(steer)), c);
                    steer = {steer.x - bounce.x, steer.y - bounce.y, steer.z - bounce.z};
                }
            }
        }
        UnkVec3_cs* out = fn_71000cc374(steering, i);
        out->x = steer.x;
        out->y = steer.y;
        out->z = steer.z;
    }
}
