// behaviortree::CSabotoruIsFarFromCornerCondition unit: whether a point is far from the corner the
// owner's enemy movement is on, by a side search of the AI path follower from the movement's point
// toward the given position. Names and layouts are placeholders.
#include <functional>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkAIPathFollower;

struct UnkAIPathSearchOut;

// What the side search fills here: 0x14 bytes, four-byte aligned.
struct UnkSabotoruSearchOut {
    u32 a[2];
    u32 b[2];
    f32 distance;

    UnkSabotoruSearchOut() : a{0, 0}, b{0, 0}, distance(-1.0f) {}
};

bool fn_710035e8d8(UnkAIPathFollower* self, const CVector2D* from, const CVector2D* to, const CVector2D* dir_a,
                   const CVector2D* dir_b, bool flag, CVector2D* point, u32* out_0x7, f32 limit, bool* blocked,
                   f32* out_a, f32* out_b, u32* out_c, UnkAIPathSearchOut* out, std::function<void()> filter);

struct UnkSabotoruCorner {
    u8 pad_0x0[0x89];
    u8 flag;  // 0x971 of the movement
    u8 pad_0x8a[2];
    CVector2D point;  // 0x974 of the movement
};

struct UnkSabotoruMovement {
    void** vtable;
    u8 pad_0x8[0x50];
    CVector3D target;  // 0x58
    u8 pad_0x64[0x884];
    UnkSabotoruCorner corner;  // 0x8e8
    u8 pad_0x97c[0x90];
    u8 reversed;  // 0xa0c

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkSabotoruMovement*)>(vtable[0])(this);
    }
};
static_assert(__builtin_offsetof(UnkSabotoruMovement, corner) == 0x8e8, "corner");
static_assert(__builtin_offsetof(UnkSabotoruMovement, reversed) == 0xa0c, "reversed");

struct UnkSabotoruAI {
    u8 pad_0x0[0x68];
    UnkAIPathFollower* follower() { return reinterpret_cast<UnkAIPathFollower*>(pad_0x0 + 0x68); }
};

struct UnkSabotoruOwner {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x178];
    UnkSabotoruMovement* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkSabotoruAI* ai;  // 0x240
};
static_assert(__builtin_offsetof(UnkSabotoruOwner, movement) == 0x1f0, "movement");
static_assert(__builtin_offsetof(UnkSabotoruOwner, ai) == 0x240, "ai");

struct UnkSabotoruCondition {
    u8 pad_0x0[0x18];
    UnkSabotoruOwner* owner;  // 0x18
};

bool fn_7100675d54(UnkSabotoruCondition* self, const CVector3D* pos) {
    UnkSabotoruMovement* movement = self->owner->movement;
    if (!movement || !fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()) || fn_71010cc3a4(movement) != 1)
        return true;

    UnkSabotoruCorner& corner = movement->corner;
    bool reversed = (movement->reversed != 0) ^ (corner.flag != 0);
    UnkAIPathFollower* follower = self->owner->ai->follower();
    const CVector2D* from = &corner.point;
    CVector2D point = {0.0f, 0.0f};
    u8 hit_side;
    bool blocked;
    s32 index_a = -1;
    s32 index_b = -1;
    f32 side_distance = -1.0f;
    UnkSabotoruSearchOut out;
    CVector2D to;
    fn_710007b430(&to, pos);
    CVector2D dir;
    fn_710007b430(&dir, &movement->target);
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    CVector2D front;
    fn_710007b430(&front, &static_cast<const CVector3D&>(CVector3D{m.m[2], m.m[6], m.m[10]}));
    fn_710035e8d8(follower, from, &to, &dir, &front, reversed, &point, reinterpret_cast<u32*>(&hit_side), 0.0f,
                  &blocked, reinterpret_cast<f32*>(&index_a), reinterpret_cast<f32*>(&index_b),
                  reinterpret_cast<u32*>(&side_distance), reinterpret_cast<UnkAIPathSearchOut*>(&out), std::function<void()>());
    if (reversed ^ (hit_side == 0))
        return true;
    return false;
}
