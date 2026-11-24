// CEnemyMovement unit: the rotation angles one step of turning the movement's angles (+0x90) toward its
// direction (+0x70), limited by the character class's turn speed for this frame's time.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CMovementComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

struct UnkEnemyMovement_b45a2;
f32 CCharClassEnemyMovement_GetAngularSpeed(UnkEnemyMovement_b45a2* self, UnkActor* actor);
void* fn_71007ea32c(void* actor);
void fn_71000769f0(UnkMatrix44* out, const void* a, const void* b, const void* c, const void* d);
extern const CVector3D g_unk_710164ceac __attribute__((visibility("hidden")));

struct UnkEnemyClass_l0cd7ec {
    u8 pad_0x0[0x168];
    bool turn_track;  // 0x168
};

struct UnkEnemyMovement_l0cd7ec {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x38];
    CVector3D direction;  // 0x70
    u8 pad_0x7c[0x14];
    UnkVec3f angles;  // 0x90
    u8 pad_0x9c[0x1c18 - 0x9c];
    UnkEnemyClass_l0cd7ec* char_class;  // 0x1c18
};

UnkVec3f fn_71010d2cf4(UnkEnemyMovement_l0cd7ec* self, const UnkVec3f* current, f32 dt);

static inline UnkVec3f Cross(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

static inline f32 Dot(const UnkVec3f& a, const CVector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline f32 Dot(const CVector3D& a, const CVector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline f32 Length(const CVector3D& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

static inline f32 Length(const UnkVec3f& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

// The angle between two directions, in radians.
static inline f32 AngleBetween(const UnkVec3f& a, const CVector3D& b) {
    f32 cosine = Dot(a, b) / (Length(b) * Length(a));
    if (cosine >= 1.0f)
        return 0.0f;
    if (cosine <= -1.0f)
        return 3.14159265f;
    return acosf(cosine);
}

static inline bool Differs(f32 a, f32 b, f32 epsilon) {
    return !(a + epsilon >= b) || !(a + -epsilon <= b);
}

static inline UnkVec3f Column(const UnkMatrix44& m, int i) {
    return UnkVec3f(m.m[i], m.m[4 + i], m.m[8 + i]);
}


UnkVec3f fn_71010cd7ec(UnkEnemyMovement_l0cd7ec* self, const UnkVec3f* current, f32 dt) {
    if (fn_71007ea32c(self->owner) && !fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(self)))
        return fn_71010d2cf4(self, current, dt);

    UnkMatrix44 rotation = fn_7100078fc8(reinterpret_cast<const UnkMatrix44&>(self->angles));
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (self->char_class->turn_track && animation && !fn_710117b348(animation, fn_7100d11fa8()))
        return self->angles;

    UnkVec3f forward = reinterpret_cast<const UnkVec3f&>(self->direction);
    UnkVec3f up;
    up = Column(rotation, 1);
    UnkVec3f right = Cross(up, forward);

    const UnkMatrix44& world = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    f32 angle = AngleBetween(Column(world, 2), self->direction);
    f32 step = CCharClassEnemyMovement_GetAngularSpeed(reinterpret_cast<UnkEnemyMovement_b45a2*>(self->char_class), self->owner) * dt / angle;
    step = step < 1.0f ? step : 1.0f;

    if (Differs(fabsf(Dot(Column(rotation, 1), self->direction)), 1.0f, 0.001f)) {
        up = Cross(forward, right);
        if (fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(self)) && up.y < -0.01f) {
            up.x = -up.x;
            up.y = -up.y;
            up.z = -up.z;
        }
        up.z = 0.0f;
        f32 scale = 1.0f / (Length(up) + 0.0001f);
        up.x *= scale;
        up.y *= scale;
        up.z *= scale;
    } else {
        up = Cross(forward, Column(rotation, 0));
    }
    right = Cross(up, forward);

    UnkMatrix44 target;
    fn_71000769f0(&target, &right, &up, &forward, &g_unk_710164ceac);
    UnkMatrix44 blended = fn_7100078834(reinterpret_cast<const UnkKeyedEntry*>(&rotation), &target, step);
    return fn_7100079b18(reinterpret_cast<UnkXform*>(&blended), current->z);
}
