// CObsydomithonAIComponent unit (tools/tu_map.py, the name from reflection). fn_710062e6c4: after the AI
// component's hit handler (fn_7101105c94), when the hit actor is the owner's target, the owner is not frozen and
// the hit collider's name is not g_unk_71726f9438, the owner's timeline (+0x2e8) gets the id g_unk_710164a338.
// fn_710062cd74: the facing angle at +0x1475c set from the given angle (clamped to [0, 90]) and the movement
// object's direction, wrapped into [0, 360); the angle to the target written next to it (as fn_710062f6e4 in
// CObsydomithonAIComponent_Angle_b69a4.cpp); the facing sent to the controller's property bag.
// fn_710062f490: a track event handler: turn tracks set and clear the turning flag (+0x14764), and an
// attack-fail track on the animation resets the facing to 90 and stops the attack at +0x146c8.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

using base::global::CStrId;

struct UnkWarLotusAI;
struct UnkCollisionContainer;
void fn_7101105c94(UnkWarLotusAI* self);
CStrId* fn_7100864018(UnkCollisionContainer* self, void* object);
bool fn_71007f9f20(void* obj);
extern CStrId g_unk_71726f9438 __attribute__((visibility("hidden")));
extern u64 g_unk_710164a338;

struct UnkObsydomithonOwner_b0129a {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // 0x2a0
    u8 pad_0x2a8[0x20];
    void* frost;  // 0x2c8
    u8 pad_0x2d0[0x18];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkObsydomithonAI_b0129a {
    u8 pad_0x0[0x30];
    UnkObsydomithonOwner_b0129a* owner;
};

struct UnkObsydomithonHit_b0129a {
    u8 pad_0x0[0x98];
    void* object;
};

// The owner has a collision container and a timeline, and there is an actor.
static inline bool Ready(UnkObsydomithonOwner_b0129a& owner, UnkActor* actor) {
    return !((owner.timeline == nullptr) | (actor == nullptr) | (owner.collision == nullptr));
}

void fn_710062e6c4(UnkObsydomithonAI_b0129a* self, UnkActor* actor, UnkObsydomithonHit_b0129a* hit) {
    fn_7101105c94(reinterpret_cast<UnkWarLotusAI*>(self));
    UnkObsydomithonOwner_b0129a* owner = self->owner;
    UnkCollisionContainer* collision = owner->collision;
    UnkTimelineComponent* timeline = owner->timeline;
    if (!Ready(*owner, actor))
        return;
    void* frost = owner->frost;
    if (fn_7100d7c990(reinterpret_cast<UnkActor*>(owner)) != actor)
        return;
    if (frost && fn_71007f9f20(frost))
        return;
    const CStrId* id = hit->object ? fn_7100864018(collision, hit->object) : &g_unk_7101d470e0;
    if (id->data == g_unk_71726f9438.data)
        return;
    alignas(8) u32 mask = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
    fn_7100a2cb7c(timeline, &g_unk_710164a338, &mask, false, -1.0f);
}

extern "C" f32 atan2f(f32 y, f32 x);
extern base::global::CStrId g_unk_71726f9428 __attribute__((visibility("hidden")));

struct UnkObsyVec_b0129b {
    f32 x, y, z;
    UnkObsyVec_b0129b(const UnkObsyVec_b0129b& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkObsyMover_b0129b {
    void** vtable;
    UnkObsyVec_b0129b Get() {
        using Fn = UnkObsyVec_b0129b (*)(UnkObsyMover_b0129b*);
        return reinterpret_cast<Fn>(vtable[0x210 / 8])(this);
    }
};

struct UnkObsyCtrl_b0129b {
    u8 pad_0x0[0x80];
    unk_prop_bag::UnkPropBag bag;
};

struct UnkObsyOwner_b0129b {
    u8 pad_0x0[0x1f0];
    UnkObsyMover_b0129b* mover;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkObsyCtrl_b0129b* ctrl;  // 0x220
};

struct UnkVec2_b0129b {
    f32 x, y;
};

struct UnkObsyFacing_b0129b {
    f32 facing;        // 0x0
    f32 target_angle;  // 0x4
    u8 pad_0x8[0x38];
    UnkVec2_b0129b offset;  // 0x40
};

struct UnkObsydomithonAI_b0129b {
    u8 pad_0x0[0x30];
    UnkObsyOwner_b0129b* owner;
    u8 pad_0x38[0x14724];
    UnkObsyFacing_b0129b facing;  // 0x1475c
};
static_assert(__builtin_offsetof(UnkObsydomithonAI_b0129b, facing) == 0x1475c, "layout");

static inline __attribute__((always_inline)) void AngleToTarget_b0129b(UnkObsydomithonAI_b0129b* self,
                                                                       const UnkVec2_b0129b* offset, f32* angle) {
    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    f32 tx = target->field_0x44;
    f32 ty = target->field_0x54;
    f32 x = tx + offset->x;
    f32 y = ty + offset->y;
    UnkActor* actor = reinterpret_cast<UnkActor*>(self->owner);
    f32 ax = actor->field_0x44;
    f32 ay = actor->field_0x54;
    f32 dx = x - ax;
    f32 dy = y - ay;
    *angle = atan2f(dy, dx) * 57.29578f;
    if (*angle >= 360.0f)
        *angle += -360.0f;
    else if (*angle < -0.001f)
        *angle += 360.0f;
    if (*angle >= -0.001f && *angle <= 0.001f)
        *angle = 0.001f;
}

void fn_710062cd74(UnkObsydomithonAI_b0129b* self, f32 angle) {
    UnkObsyFacing_b0129b* facing = &self->facing;
    f32 v = (angle >= 0.0f && angle <= 90.0f) ? angle : 0.0f;
    if (UnkObsyMover_b0129b* mover = self->owner->mover) {
        UnkObsyVec_b0129b dir = mover->Get();
        if (dir.x > 0.001f)
            facing->facing = 0.0f - v;
        else if (dir.x < -0.001f)
            facing->facing = v + 180.0f;
        if (facing->facing < -0.001f)
            facing->facing += 360.0f;
        else if (facing->facing >= 360.0f)
            facing->facing += -360.0f;
    }
    if (fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)))
        AngleToTarget_b0129b(self, &facing->offset, &facing->target_angle);
    else
        facing->target_angle = facing->facing;
    if (UnkObsyCtrl_b0129b* ctrl = self->owner->ctrl)
        unk_prop_bag::fn_710033187c(&ctrl->bag, g_unk_71726f9428, facing->facing);
}

extern CStrId g_unk_71726f9428 __attribute__((visibility("hidden")));
struct UnkObsyAI_A3_b75;
void fn_710062cf68(UnkObsyAI_A3_b75* self);
void fn_710062a268(void* attack, bool value);
bool fn_7100c978dc(void* track, const base::reflection::CClass* cls);

struct UnkObsyCtrl_b0129c {
    u8 pad_0x0[0x80];
    unk_prop_bag::UnkPropBag bag;
};

struct UnkObsyOwner_b0129c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkObsyFacing_b0129c {
    f32 facing;  // 0x0
    f32 target_angle;
    bool turning;  // 0x8
};

struct UnkObsydomithonAI_b0129c {
    u8 pad_0x0[0x30];
    UnkObsyOwner_b0129c* owner;
    u8 pad_0x38[0x146c8 - 0x38];
    u8 attack[0x1475c - 0x146c8];  // 0x146c8
    UnkObsyFacing_b0129c facing;  // 0x1475c
};
static_assert(__builtin_offsetof(UnkObsydomithonAI_b0129c, facing) == 0x1475c, "layout");

struct UnkTrackData_b0129c {
    u8 pad_0x0[0x38];
    CStrId name;
};

struct UnkTrack_b0129c {
    u8 pad_0x0[0x10];
    UnkTrackData_b0129c* data;
};

struct UnkTrackEvent_b0129c {
    s32 kind;
    UnkTrack_b0129c* track;
};

struct UnkHashData_b0129c {
    u8 pad_0x0[0x30];
    u64 hash;
};

bool fn_710062f490(UnkObsydomithonAI_b0129c* self, UnkTrackEvent_b0129c* event) {
    UnkAnimationComponent* animation = self->owner ? self->owner->animation : nullptr;
    UnkTrack_b0129c* track = event->track;
    const CStrId* name = track && track->data ? &track->data->name : &g_unk_7101d470e0;
    if (!animation || name->data == &g_unk_7101d47108->empty)
        return true;
    UnkObsyFacing_b0129c* facing = &self->facing;
    if (event->kind == 3 && fn_7100c978dc(track, fn_7100d2b3a8()))
        facing->turning = true;
    if (event->kind == 1 && fn_7100c978dc(event->track, fn_7100d2b3a8())) {
        facing->turning = false;
        fn_710062cf68(reinterpret_cast<UnkObsyAI_A3_b75*>(self));
    }
    if (event->kind == 1) {
        if (reinterpret_cast<UnkHashData_b0129c*>(name->data)->hash == 0xcfe8c2748f6c9936) {
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 2;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0x9acfaf2b2d6d677f, 0xfc45ff84f571ba40, &mode, &g_unk_710164b958,
                          &g_unk_7172769740, &mask);
        }
        if (fn_710117b348(animation, fn_7100c0d1d8())) {
            facing->facing = 90.0f;
            if (auto* ctrl = reinterpret_cast<UnkObsyCtrl_b0129c*>(self->owner->animation))
                unk_prop_bag::fn_710033187c(&ctrl->bag, g_unk_71726f9428, 90.0f);
            fn_710062a268(self->attack, false);
        }
    }
    return true;
}
