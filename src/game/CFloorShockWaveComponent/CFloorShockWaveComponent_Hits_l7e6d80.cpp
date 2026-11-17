// CFloorShockWaveComponent's unit: whether the floor wave hits an actor. Past the base test
// (fn_71009f4d18), an actor with a body (+0x1f0) must lie on the floor line through the owner, as wide as
// the wave's radius plus half the actor's box, within the height tolerance (+0x240); then only actors on
// the side the wave travels (+0x16c, its sign) count. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU2523.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkOwner;
struct UnkVec3f;
struct UnkShockWaveHitTest_ovb0123;
bool fn_71009f4d18(UnkShockWaveHitTest_ovb0123* self, UnkOwner* actor, const UnkVec3f* direction, bool force);

struct UnkEntityBox {
    base::math::CVector3D min;
    base::math::CVector3D max;
};

// A position returned in memory (x8): 16 bytes, not trivially copyable.
struct UnkActorPosition {
    f32 x, y, z, w;
    UnkActorPosition(const UnkActorPosition& other);
};

struct UnkFloorShockWaveBody_l7e6d80 {
    u8 pad_0x0[0xf9];
    bool airborne;  // 0xf9
};

struct UnkFloorShockWaveActor_l7e6d80 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x188];
    UnkFloorShockWaveBody_l7e6d80* body;  // 0x1f0
};
static_assert(__builtin_offsetof(UnkFloorShockWaveActor_l7e6d80, body) == 0x1f0, "layout");

struct UnkFloorShockWave_l7e6d80 {
    u8 pad_0x0[0x30];
    UnkFloorShockWaveActor_l7e6d80* owner;  // 0x30
    u8 pad_0x38[0x134];
    f32 travel;  // 0x16c
    u8 pad_0x170[0xcc];
    bool hits_airborne;  // 0x23c
    u8 pad_0x23d[3];
    f32 height_tolerance;  // 0x240
    u8 pad_0x244[0x40];
    f32 radius;  // 0x284
};
static_assert(__builtin_offsetof(UnkFloorShockWave_l7e6d80, radius) == 0x284, "layout");

inline CVector2D Minus(const CVector2D& a, const CVector2D& b) { return {a.x - b.x, a.y - b.y}; }
inline CVector2D Plus(const CVector2D& a, const CVector2D& b) { return {a.x + b.x, a.y + b.y}; }

bool fn_71007e6d80(UnkFloorShockWave_l7e6d80* self, UnkOwner* target, const UnkVec3f* direction, bool force) {
    bool hit = fn_71009f4d18(reinterpret_cast<UnkShockWaveHitTest_ovb0123*>(self), target, direction, force);
    UnkFloorShockWaveActor_l7e6d80* actor = reinterpret_cast<UnkFloorShockWaveActor_l7e6d80*>(target);
    if (actor && actor->body) {
        if (!hit)
            return false;
        if (!self->hits_airborne && actor->body->airborne)
            return false;
        UnkEntityBox box = fn_7100d7bc70(reinterpret_cast<const UnkEntityBounds*>(actor));
        const UnkFloorShockWaveActor_l7e6d80* owner = self->owner;
        CVector2D center;
        {
            f32 x = owner->x, y = owner->y, z = owner->z;
            CVector3D position = {x, y, z};
            fn_710007b430(&center, &position);
        }
        CVector2D point;
        {
            UnkActorPosition position = fn_7100d7b7a8(reinterpret_cast<UnkActor*>(actor));
            fn_710007b430(&point, reinterpret_cast<const CVector3D*>(&position));
        }
        f32 reach = self->radius + (box.max.x - box.min.x) * 0.5f;
        CVector2D half = {reach, 0.0f};
        UnkSegment2D floor(Minus(center, half), Plus(center, half));
        CVector2D closest = {0.0f, 0.0f};
        if (!fn_7100126018(&floor, &point, &closest, 0.0f))
            return false;
        if (!(__builtin_fabsf(point.y - closest.y) <= self->height_tolerance))
            return false;
    } else if (!hit) {
        return false;
    }
    if (self->travel > 0.0f) {
        if (actor->x < self->owner->x)
            return false;
    } else if (self->travel < 0.0f) {
        if (actor->x > self->owner->x)
            return false;
    }
    return true;
}
