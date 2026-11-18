// behaviortree::CSabotoruIsFarFromCornerCondition's unit (unit 997): fn_7100675a28 is true when the
// Sabotoru, walking a path, is far enough along it from its corner (its home or spawn point) that the
// remaining distance (fn_710011958c) runs out. Names and layouts are placeholders.
#include <functional>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkAIPathFollower;
struct UnkAIPathSearchOut;

bool fn_710035e8d8(UnkAIPathFollower* self, const CVector2D* from, const CVector2D* to, const CVector2D* dir_a,
                   const CVector2D* dir_b, bool flag, CVector2D* point, u32* out_0x7, f32 limit, bool* blocked,
                   f32* out_a, f32* out_b, u32* out_c, UnkAIPathSearchOut* out, std::function<void()> filter);

// What a side search fills (0x14 bytes).
struct UnkAIPathSearchOut {
    u64 a;
    u64 b;
    f32 distance;

    UnkAIPathSearchOut() : a(0), b(0), distance(-1.0f) {}
};

struct UnkVec2f_A5 {
    f32 x, y;
    UnkVec2f_A5(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkVec2f_A5(const UnkVec2f_A5& o) : x(o.x), y(o.y) {}
};

// The Sabotoru's corner tunables (at +0x146dc of its AI).
struct UnkSabotoruCorner_l675a28 {
    f32 radius_near;     // 0x0
    f32 radius_far;      // 0x4
    u8 pad_0x8[0x14];
    CVector3D corner;    // 0x1c
    CVector3D home;      // 0x28
};

struct UnkSabotoruAI_l675a28 {
    u8 pad_0x0[0x68];
    u8 follower[0x30];  // 0x68, the path follower
    u8 pad_0x98[0x146dc - 0x98];
    UnkSabotoruCorner_l675a28 corner;  // 0x146dc
};

// The movement's path state (at +0x8e8 of the movement, kind 1).
struct UnkSabotoruPathState_l675a28 {
    u8 pad_0x0[0x89];
    u8 flag;        // 0x89
    u8 pad_0x8a[2];
    CVector2D pos;  // 0x8c
    u8 pad_0x94[8];
    u32 edge;       // 0x9c
};

struct UnkSabotoruMovement_l675a28 {
    u8 pad_0x0[0x58];
    CVector3D dir;  // 0x58
    u8 pad_0x64[0x8e8 - 0x64];
    UnkSabotoruPathState_l675a28 state;  // 0x8e8
    u8 pad_0x988[0x9e4 - 0x988];
    CVector2D facing;  // 0x9e4
    u8 pad_0x9ec[0xa0c - 0x9ec];
    u8 reversed;  // 0xa0c
};

struct UnkSabotoruSpawnHolder_l675a28 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // 0x10
};

struct UnkSabotoruOwner_l675a28 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1f0 - 0x78];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x37c - 0x248];
    f32 reach;  // 0x37c
    u8 pad_0x380[0x18];
    UnkSabotoruSpawnHolder_l675a28* spawn;  // 0x398
};

struct UnkFarFromCornerCondition_l675a28 {
    u8 pad_0x0[0x18];
    UnkSabotoruOwner_l675a28* owner;  // 0x18
};

bool fn_7100675d54(UnkFarFromCornerCondition_l675a28* self, const CVector3D* corner);
void fn_710011958c(UnkAIPathFollower* follower, u32 edge, const CVector2D* from, bool forward,
                   const CVector3D* to, void* a, void* b, void* c, f32 distance, f32 limit, f32* remaining,
                   const CVector2D* facing);

// Component-wise copies, as the game's vectors copy.
static inline void Assign(CVector2D* out, const CVector2D& v) {
    out->x = v.x;
    out->y = v.y;
}

static inline void Assign(CVector3D* out, const CVector3D& v) {
    out->x = v.x;
    out->y = v.y;
    out->z = v.z;
}

// The third column of a transform: the direction it faces.
static inline CVector3D UnkAxisZ(const UnkMatrix44& m) {
    CVector3D v;
    v.x = m.m[2];
    v.y = m.m[6];
    v.z = m.m[10];
    return v;
}

bool fn_7100675a28(UnkFarFromCornerCondition_l675a28* self) {
    if (!self->owner)
        return false;
    UnkReflObject* ai_obj = self->owner->ai;
    if (!ai_obj || !fn_710009d50c(ai_obj->vf_0x0(), fn_7100677dd4()))
        return false;
    UnkReflObject* movement_obj = self->owner->movement;
    if (!movement_obj || !fn_710009d50c(movement_obj->vf_0x0(), fn_71010c74e0()))
        return false;
    if (fn_71010cc3a4(movement_obj) != 1)
        return false;

    UnkSabotoruAI_l675a28* ai = reinterpret_cast<UnkSabotoruAI_l675a28*>(ai_obj);
    UnkSabotoruMovement_l675a28* movement = reinterpret_cast<UnkSabotoruMovement_l675a28*>(movement_obj);
    UnkSabotoruPathState_l675a28* state = &movement->state;
    const UnkSabotoruCorner_l675a28* tunable = &ai->corner;

    u32 edge = state->edge;
    CVector2D from;
    Assign(&from, state->pos);
    CVector3D corner;
    Assign(&corner, tunable->corner);
    {
        CVector2D at;
        fn_710007b430(&at, &corner);
        if (at.x + 1.0f >= state->pos.x && at.x - 1.0f <= state->pos.x && at.y + 1.0f >= state->pos.y &&
            at.y - 1.0f <= state->pos.y)
            return false;
    }

    UnkAIPathFollower* follower = reinterpret_cast<UnkAIPathFollower*>(ai->follower);
    UnkSabotoruSpawnHolder_l675a28* spawn = self->owner->spawn;
    UnkReflObject* point = spawn ? spawn->component : nullptr;
    if (!point || !fn_710009d50c(point->vf_0x0(), fn_7100a1b7b4()))
        Assign(&corner, tunable->home);

    f32 radius = tunable->radius_near;
    CVector3D query;
    Assign(&query, corner);
    if (!fn_7100675d54(self, &query))
        radius = tunable->radius_far;
    f32 remaining = radius + self->owner->reach;
    CVector2D facing;
    Assign(&facing, movement->facing);

    UnkVec2f_A5 nearest(0.0f, 0.0f);
    bool forward;
    bool blocked;
    s32 edge_from = -1;
    s32 edge_to = -1;
    f32 distance = -1.0f;
    UnkAIPathSearchOut out;
    bool flag = (movement->reversed != 0) != (state->flag != 0);

    CVector2D to;
    fn_710007b430(&to, &corner);
    CVector2D dir;
    fn_710007b430(&dir, &movement->dir);
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    CVector2D ahead;
    CVector3D axis = UnkAxisZ(m);
    fn_710007b430(&ahead, &axis);

    fn_710035e8d8(follower, &state->pos, &to, &dir, &ahead, flag, reinterpret_cast<CVector2D*>(&nearest),
                  reinterpret_cast<u32*>(&forward), 0.0f, &blocked, reinterpret_cast<f32*>(&edge_from),
                  reinterpret_cast<f32*>(&edge_to), reinterpret_cast<u32*>(&distance), &out,
                  std::function<void()>());

    fn_710011958c(follower, edge, &from, forward, &corner, nullptr, nullptr, nullptr, remaining, -1.0f,
                  &remaining, &facing);
    return remaining <= 0.0f;
}
