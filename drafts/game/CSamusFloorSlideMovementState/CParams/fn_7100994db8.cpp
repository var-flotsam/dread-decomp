// CSamusFloorSlideMovementState (unit 1459): pushes the slide on. The push is the movement's velocity with x set
// to the tunables' speed (none while a dash melee track plays) in the direction the owner faces, scaled by the
// speed booster's factor while it runs. After fitting the body (fn_710099230c), a movement that keeps sliding
// (+0x51a4) takes the push and the state updates (slot 0x118); otherwise the state asks for the next movement
// state with the push, under the game id table's id at +0xfd0 unless `keep` is set. Names and layouts are
// placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAbilitySpeedBooster.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::math::CVector3D;

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

struct UnkDesc16_ovb0004 {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_ovb0004 {
    s64 id;
    UnkDesc16_ovb0004 desc;
    UnkTrackBlend blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_ovb0004(UnkDesc16_ovb0004 d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_ovb0004()
        : UnkMoveTransition_ovb0004(*reinterpret_cast<const UnkDesc16_ovb0004*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_ovb0004) == 0x30, "layout");

extern base::reflection::UnkTunable g_unk_71726b0fc0;

// The speed booster's tunables: the slide speed factor at +0x30.
struct UnkBoosterTunable_l994db8 {
    u8 pad_0x0[0x30];
    f32 slide_factor;
};

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8) asking for the state of g_unk_7101a22ce8, with a push.
struct UnkDashParams_l994db8 {
    void* vtable;
    UnkMoveTransition_ovb0004 transition;  // 0x8
    u32 mode;                              // 0x38
    CVector3D push;                        // 0x3c

    UnkDashParams_l994db8() : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        mode = 3;
        push.x = 0.0f;
        push.y = 0.0f;
        push.z = 0.0f;
        vtable = g_unk_7101a22ce8 + 0x10;
    }
};
static_assert(sizeof(UnkDashParams_l994db8) == 0x48, "layout");

struct UnkDashOwner_l994db8 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1a8];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkDashMovement_l994db8 {
    u8 pad_0x0[0x1d8];
    CVector3D velocity;  // 0x1d8
    u8 pad_0x1e4[0x4fc0];
    bool sliding;  // 0x51a4
};
static_assert(__builtin_offsetof(UnkDashMovement_l994db8, sliding) == 0x51a4, "layout");

struct UnkDashTunables_l994db8 {
    u8 pad_0x0[0xd0];
    f32 speed;  // 0xd0
};

struct UnkDashState_l994db8 {
    void** vtable;
    UnkAirTarget_B71a6* target;  // 0x8
    u8 pad_0x10[0x20];
    UnkDashOwner_l994db8* owner;        // 0x30
    UnkDashMovement_l994db8* movement;  // 0x38
    UnkDashTunables_l994db8* tunables;  // 0x40

    void vf_0x118() { reinterpret_cast<void (*)(UnkDashState_l994db8*)>(vtable[0x118 / 8])(this); }
};

struct UnkSlideState_ovb0004;
void fn_710099230c(UnkSlideState_ovb0004* self);

static inline s32 Sign(f32 v) {
    return (v > 0.0f) - (v < 0.0f);
}

void fn_7100994db8(UnkSlideState_ovb0004* state, bool keep) {
    auto* self = reinterpret_cast<UnkDashState_l994db8*>(state);
    bool melee = fn_710117b348(self->owner->animation, fn_7100cf4438());
    CVector3D push = self->movement->velocity;
    f32 speed = melee ? 0.0f : self->tunables->speed;
    push.x = speed * Sign(fn_710002614c(&self->owner->transform).m[2]);
    if (UnkAbility* booster = fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310))) {
        if (fn_710033cf20(booster))
            push.x *= reinterpret_cast<const UnkBoosterTunable_l994db8*>(&g_unk_71726b0fc0)->slide_factor;
    }
    fn_710099230c(state);
    if (self->movement->sliding) {
        fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(self->movement),
                      reinterpret_cast<const UnkMovementVec_71010d08d4*>(&push), true);
        self->vf_0x118();
        return;
    }
    UnkDashParams_l994db8 params;
    if (!keep)
        params.transition.id = GameManagerIdTable().field_0xfd0.data->Key();
    params.push = push;
    params.mode = 1;
    fn_710098ae04(self->target, reinterpret_cast<UnkAirRequest_B71a6*>(&params));
}
