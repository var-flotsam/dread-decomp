// CBatalloonAIComponent's unit (tools/tu_map.py unit 583): after the base routine fills `out`, stops the
// movement (both vectors zeroed) when another Batalloon of the spawn group lies ahead in the direction of
// travel, within range, and nearer the target than this one is. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CBatalloonAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector3D;

struct UnkNavComponent;
struct UnkNavGroupInfo;
struct UnkSwifterMember_ovb0114;

// As src/game/CBatalloonAIComponent/CBatalloonAIComponent_Sync_l3dd8b0.cpp defines them.
struct UnkBatalloonMember_l3dd8b0 {
    u8 pad_0x0[0x40];
};

struct UnkBatalloonGroup_l3dd8b0 {
    u8 pad_0x0[0x68];
    UnkBatalloonMember_l3dd8b0* members;  // +0x68
    u32 memberCount;                    // +0x70
};

struct UnkBatalloonGroupSource_l3dd8b0 {
    u8 pad_0x0[0x60];
    u8 part[1];  // +0x60
};

struct UnkBatalloonGroupHandle_l3dd8b0 {
    u8 pad_0x0[0x10];
    void* target;  // +0x10
};

UnkBatalloonGroupSource_l3dd8b0* fn_71003ddc44(UnkNavGroupInfo* info);
UnkBatalloonGroup_l3dd8b0* fn_71003ddd20(void* target);

struct UnkBatalloonMemberData_l3ded50 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // +0x10
};

struct UnkBatalloonMemberEntity_l3ded50 {
    u8 pad_0x0[0x40];
    UnkBatalloonMemberData_l3ded50* data;  // +0x40
};

struct UnkBatalloonMemberHandle_l3ded50 {
    u8 pad_0x0[0x10];
    UnkBatalloonMemberEntity_l3ded50* entity;  // +0x10
};

struct UnkBatalloonRange_l3ded50 {
    u8 pad_0x0[0x534];
    f32 radius;  // +0x534
};

struct UnkBatalloonAI_l3ded50 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // +0x30
    u8 pad_0x38[0x14698];
    UnkBatalloonRange_l3ded50* range;  // +0x146d0
};

struct UnkBatalloonMove_l3ded50 {
    u8 pad_0x0[0x60];
    CVector3D direction;  // +0x60
    CVector3D velocity;   // +0x6c
};

void fn_71005a25b0(UnkBatalloonAI_l3ded50* self, UnkBatalloonMove_l3ded50* out);

struct UnkVec_l3ded50 {
    f32 x, y, z;
};

static inline UnkVec_l3ded50 operator-(const UnkVec_l3ded50& a, const UnkVec_l3ded50& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline UnkVec_l3ded50 operator*(const UnkVec_l3ded50& v, f32 s) {
    return {v.x * s, v.y * s, v.z * s};
}

static inline f32 Dot(const UnkVec_l3ded50& a, const UnkVec_l3ded50& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline f32 Length(const UnkVec_l3ded50& v) {
    return sqrtf(Dot(v, v));
}

static inline f32 Length(const CVector3D& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

static inline UnkVec_l3ded50 Normalized(const UnkVec_l3ded50& v) {
    return v * (1.0f / (Length(v) + 0.0001f));
}

static inline UnkVec_l3ded50 Pos(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

static inline f32 DistanceSq(const UnkActor* a, const UnkActor* b) {
    UnkVec_l3ded50 d = Pos(a);
    d.x -= b->field_0x44;
    d.y -= b->field_0x54;
    d.z -= b->field_0x64;
    return Dot(d, d);
}

void fn_71003ded50(UnkBatalloonAI_l3ded50* self, UnkBatalloonMove_l3ded50* out) {
    fn_71005a25b0(self, out);
    void* animation = self->owner->field_0x220;
    if (animation && fn_7101178e6c(static_cast<UnkTakumakuComponent*>(animation), 6))
        return;
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(animation), fn_7100d253f8()))
        return;

    f32 speed = Length(out->direction);
    UnkVec_l3ded50 direction{out->direction.x, out->direction.y, out->direction.z};

    UnkNavGroupInfo* info = fn_710110c3d8(reinterpret_cast<UnkNavComponent*>(self));
    if (!info)
        return;
    UnkBatalloonGroupSource_l3dd8b0* source = fn_71003ddc44(info);
    if (!source)
        return;
    UnkBatalloonGroupHandle_l3dd8b0* handle = reinterpret_cast<UnkBatalloonGroupHandle_l3dd8b0*>(
        *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(source->part)));
    if (!handle || !handle->target)
        return;
    UnkBatalloonGroup_l3dd8b0* group = fn_71003ddd20(handle->target);
    if (!group)
        return;

    f32 rangeSq = 0.0f;
    if (UnkBatalloonRange_l3ded50* range = self->range)
        rangeSq = range->radius * range->radius;
    UnkActor* target = fn_7100d7c990(self->owner);
    f32 targetDistSq = 0.0f;
    if (target)
        targetDistSq = DistanceSq(target, self->owner);

    for (UnkBatalloonMember_l3dd8b0 *it = group->members, *end = it + group->memberCount; it != end; ++it) {
        UnkBatalloonMemberHandle_l3ded50* member = reinterpret_cast<UnkBatalloonMemberHandle_l3ded50*>(
            *fn_71003df2d0(reinterpret_cast<UnkSwifterMember_ovb0114*>(it)));
        if (!member || !member->entity || !member->entity->data)
            continue;
        UnkActor* other = member->entity->data->actor;
        if (!other || other == self->owner)
            continue;
        UnkVec_l3ded50 toOther = Normalized(Pos(other) - Pos(self->owner));
        if (!(Dot(direction * (1.0f / (speed + 0.0001f)), toOther) >= 0.0f))
            continue;
        if (!(DistanceSq(other, self->owner) < rangeSq))
            continue;
        f32 otherDistSq = target ? DistanceSq(target, other) : 0.0f;
        if (!(otherDistSq < targetDistSq))
            continue;
        const CVector3D& zero = g_unk_710164779c;
        out->direction.x = zero.x;
        out->direction.y = zero.y;
        out->direction.z = zero.z;
        out->velocity.x = zero.x;
        out->velocity.y = zero.y;
        out->velocity.z = zero.z;
        return;
    }
}
