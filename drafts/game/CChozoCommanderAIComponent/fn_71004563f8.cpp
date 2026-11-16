// CChozoCommanderAIComponent unit (tools/tu_map.py): fills the movement step's velocity. On a hyper-dash
// speed track it scales the dash direction (updated by the attack when it is a CChozoCommanderAttack) by
// the track's speed and the step time, capped once by the speed limit; otherwise, in the animation
// named by g_unk_71726c9508, it scales the current velocity. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CAttackComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkVec3_l4563f8 {
    UnkVec3_l4563f8(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    f32 x;
    f32 y;
    f32 z;
};

struct UnkSpeedTrack_l4563f8 {
    u8 pad_0x0[0x18];
    f32 speed;  // 0x18
};

struct UnkCommanderOwner_l4563f8 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x30];
    void* attack;  // 0x258
};

struct UnkCommanderAttack_l4563f8 : UnkReflObject {
    void UpdateDirection(UnkVec3_l4563f8* direction) {
        reinterpret_cast<void (*)(UnkCommanderAttack_l4563f8*, UnkVec3_l4563f8*)>(
            (*reinterpret_cast<void***>(this))[0xf8 / 8])(this, direction);
    }
};

struct UnkCommanderAI_l4563f8 {
    u8 pad_0x0[0x30];
    UnkCommanderOwner_l4563f8* owner;  // 0x30
    u8 pad_0x38[0x1110];
    UnkReflObject* attack;  // 0x1148
    u8 pad_0x1150[0x14d84];
    UnkVec3_l4563f8 direction;  // 0x15ed4
    f32 max_speed;              // 0x15ee0
    f32 scale;                  // 0x15ee4
};
static_assert(__builtin_offsetof(UnkCommanderAI_l4563f8, direction) == 0x15ed4, "layout");

struct UnkMoveStep_l4563f8 {
    f32 dt;
    u8 pad_0x4[0x5c];
    UnkVec3_l4563f8 velocity;  // 0x60
    UnkVec3_l4563f8 result;    // 0x6c
};

extern __attribute__((visibility("hidden"))) UnkCls_710117af10* g_unk_71726c9508;

static inline UnkVec3_l4563f8 operator*(f32 s, const UnkVec3_l4563f8& v) {
    return UnkVec3_l4563f8(s * v.x, s * v.y, s * v.z);
}

static inline UnkVec3_l4563f8 operator*(const UnkVec3_l4563f8& v, f32 s) {
    return UnkVec3_l4563f8(v.x * s, v.y * s, v.z * s);
}

static inline f32 Length(const UnkVec3_l4563f8& v) {
    return __builtin_sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

void fn_71004563f8(UnkCommanderAI_l4563f8* self, UnkMoveStep_l4563f8* step) {
    UnkAnimationComponent* animation = self->owner->animation;
    void* attack_component = self->owner->attack;
    if (!animation || !attack_component)
        return;
    UnkTrackQuery query;
    query.cls = fn_7100c44f68();
    query.result = nullptr;
    query.value = -1.0f;
    fn_710117b08c(animation, &query);
    if (!query.result) {
        UnkAnimationComponent* current = self->owner->animation;
        if (!current)
            return;
        if (!fn_710117af10(reinterpret_cast<UnkStruct_710117af10*>(current), &g_unk_71726c9508, false))
            return;
        step->result = self->scale * step->velocity;
        return;
    }
    f32 speed = reinterpret_cast<UnkSpeedTrack_l4563f8*>(query.result)->speed;
    if (fn_7101136308(attack_component)) {
        UnkReflObject* attack = self->attack;
        if (attack && fn_710009d50c(attack->vf_0x0(), fn_7100426484()))
            static_cast<UnkCommanderAttack_l4563f8*>(attack)->UpdateDirection(&self->direction);
    }
    step->velocity = speed * self->direction * step->dt;
    if (Length(step->velocity) > self->max_speed) {
        step->velocity = self->max_speed * self->direction;
        self->max_speed = __FLT_MAX__;
    }
    step->result = step->velocity;
}
