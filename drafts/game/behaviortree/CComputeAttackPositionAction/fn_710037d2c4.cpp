// behaviortree::CComputeAttackPositionAction's unit: computes the attack position. The AI component's flag
// decides whether one is wanted; when the enemy movement is in mode 1, a side search from the actor toward
// its target gives the position, and the result goes to the blackboard records at +0x58 and +0x50.
// Names and layouts are placeholders.
#include <functional>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "types.h"

using base::global::UnkStringData;
using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkSelf;
struct UnkAIPathFollower;

struct UnkAIPathSearchOut;

// What a side search fills (0x14 bytes, four-byte aligned here).
struct UnkSideSearchOut_l37d2c4 {
    f32 field_0x0[4];
    f32 distance;

    UnkSideSearchOut_l37d2c4() : field_0x0{}, distance(-1.0f) {}
};

bool fn_710035e8d8(UnkAIPathFollower* self, const CVector2D* from, const CVector2D* to, const CVector2D* dir_a,
                   const CVector2D* dir_b, bool flag, CVector2D* point, u32* out_0x7, f32 limit, bool* blocked,
                   f32* out_a, f32* out_b, u32* out_c, UnkAIPathSearchOut* out, std::function<void()> filter);
void fn_7100395474(UnkSelf* behavior, void* record, bool value);
void fn_7100395600(UnkSelf* behavior, void* record, const CVector3D* value);

struct UnkAttackPositionAI_l37d2c4 {
    u8 pad_0x0[0x3f];
    u8 wants_position;  // 0x3f
    u8 pad_0x40[0x28];
    u8 follower[0x30];  // 0x68
};

struct UnkAttackPositionMovement_l37d2c4 {
    u8 pad_0x0[0x58];
    CVector3D pos;  // 0x58
    u8 pad_0x64[0x971 - 0x64];
    u8 field_0x971;
    u8 pad_0x972[0xa0c - 0x972];
    u8 field_0xa0c;
};

struct UnkAttackPositionActor_l37d2c4 {
    void** vtable;
    u8 pad_0x8[0x30];
    u8 field_0x38[0xc];  // the transform
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0xc];
    f32 field_0x64;
    u8 pad_0x68[0x188];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkAttackPositionAI_l37d2c4* ai;  // 0x240
};
static_assert(__builtin_offsetof(UnkAttackPositionActor_l37d2c4, movement) == 0x1f0, "layout");
static_assert(__builtin_offsetof(UnkAttackPositionActor_l37d2c4, ai) == 0x240, "layout");

struct UnkComputeAttackPosition_l37d2c4 {
    u8 pad_0x0[0x18];
    UnkAttackPositionActor_l37d2c4* actor;  // 0x18
    u8 pad_0x20[0xc];
    u8 found;  // 0x2c
    u8 pad_0x2d[3];
    CVector3D position;  // 0x30
    u8 pad_0x3c[0xc];
    bool field_0x48;
    u8 pad_0x49[7];
    UnkStringData* position_record;  // 0x50
    UnkStringData* found_record;     // 0x58
};

static inline CVector3D Translation(const UnkAttackPositionActor_l37d2c4* actor) {
    CVector3D v;
    v.x = actor->field_0x44;
    v.y = actor->field_0x54;
    v.z = actor->field_0x64;
    return v;
}

// Whether a position is wanted, and when the movement is in mode 1, the position itself.
inline void ComputePosition(UnkComputeAttackPosition_l37d2c4* self) {
    UnkAttackPositionActor_l37d2c4* actor = self->actor;
    self->found = false;
    UnkAttackPositionAI_l37d2c4* ai = actor->ai;
    if (!ai)
        return;
    self->found = ai->wants_position;
    UnkReflObject* m = actor->movement;
    UnkReflObject* cast = m && fn_710009d50c(m->vf_0x0(), fn_71010c74e0()) ? m : nullptr;
    auto* movement = reinterpret_cast<UnkAttackPositionMovement_l37d2c4*>(cast);
    if (fn_71010cc3a4(cast) != 1)
        return;
    bool flip = (movement->field_0xa0c != 0) ^ (movement->field_0x971 != 0);
    CVector2D point;
    point.x = 0.0f;
    point.y = 0.0f;
    u32 out7;
    bool blocked;
    s32 side_a = -1;
    s32 side_b = -1;
    f32 distance = -1.0f;
    UnkSideSearchOut_l37d2c4 out;
    UnkAttackPositionActor_l37d2c4* owner = self->actor;
    void* follower = owner->ai->follower;
    CVector2D from;
    CVector3D from_pos = Translation(owner);
    fn_710007b430(&from, &from_pos);
    UnkAttackPositionActor_l37d2c4* target = reinterpret_cast<UnkAttackPositionActor_l37d2c4*>(
        fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor)));
    CVector2D to;
    CVector3D to_pos = Translation(target);
    fn_710007b430(&to, &to_pos);
    CVector2D toward;
    fn_710007b430(&toward, &movement->pos);
    const UnkMatrix44& mtx = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->actor->field_0x38));
    CVector2D dir;
    CVector3D forward = {mtx.m[2], mtx.m[6], mtx.m[10]};
    fn_710007b430(&dir, &forward);
    fn_710035e8d8(static_cast<UnkAIPathFollower*>(follower), &from, &to, &toward, &dir, flip, &point, &out7,
                  0.0f, &blocked, reinterpret_cast<f32*>(&side_a), reinterpret_cast<f32*>(&side_b),
                  reinterpret_cast<u32*>(&distance),
                  reinterpret_cast<UnkAIPathSearchOut*>(&out), std::function<void()>());
    self->position.x = point.x;
    self->position.y = point.y;
    self->position.z = 0.0f;
}

u32 fn_710037d2c4(UnkComputeAttackPosition_l37d2c4* self) {
    ComputePosition(self);
    if (!self->found)
        return 6;
    if (self->found_record != &g_unk_7101d47108->empty)
        fn_7100395474(reinterpret_cast<UnkSelf*>(self), &self->found_record, self->field_0x48);
    if (self->position_record != &g_unk_7101d47108->empty)
        fn_7100395600(reinterpret_cast<UnkSelf*>(self), &self->position_record, &self->position);
    return 5;
}
