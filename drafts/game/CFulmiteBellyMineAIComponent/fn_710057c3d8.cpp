// CFulmiteBellyMineAIComponent unit (tools/tu_map.py unit 826): fn_710057c3d8 starts the mine after the base
// update: resets its state block (+0x146c8), turns the owner, picks the facing from the carrier's (+0x146f0)
// transform, and sends the facing to the owner's +0x298 and its effect sink. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTU101.h"
#include "game/UnkTimelineCalls.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkMineAI_A3_b70;
bool fn_710057c778(UnkMineAI_A3_b70* self);

struct UnkBellyMineKey_l57c3d8;
extern UnkBellyMineKey_l57c3d8 g_unk_7101649f68;  // reached through the GOT

// The kind-5 block handed to the owner's +0x298 (vtable g_unk_7101a25b10; game_s2/UnkParams.h).
struct UnkBellyMineParams_l57c3d8 {
    void** vtable;
    CVector3D dir;  // 0x8
    bool has_dir;   // 0x14
    f32 scale;      // 0x18
    bool has_scale; // 0x1c
    u64 field_0x20;
    u64 field_0x28;
    bool field_0x30;
    u32 kind;  // 0x34
    u8 pad_0x38;
    bool field_0x39;
    u8 pad_0x3a;
    bool field_0x3b;
    u8 pad_0x3c;
    bool field_0x3d;
};
static_assert(__builtin_offsetof(UnkBellyMineParams_l57c3d8, kind) == 0x34, "layout");

// The block sent to the effect sink (vtable g_unk_7101a24b80, kind 0).
struct UnkBellyMineSinkParams_l57c3d8 {
    void** vtable;
    u32 field_0x8;
    bool has_0x8;
    f32 field_0x10;
    bool has_0x10;
    CVector3D field_0x18;
    bool has_0x18;
    CVector3D up;  // 0x28
    bool has_up;
    u32 kind;  // 0x38
};
static_assert(__builtin_offsetof(UnkBellyMineSinkParams_l57c3d8, kind) == 0x38, "layout");

struct UnkBellyMineSink_l57c3d8 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool active;  // 0xf9

    void vf_0x260() { reinterpret_cast<void (*)(UnkBellyMineSink_l57c3d8*)>(vtable[0x260 / 8])(this); }
};

struct UnkBellyMineDef_l57c3d8 {
    u8 pad_0x0[0x50c];
    u32 field_0x50c;
    u8 pad_0x510[0x50];
    u32 field_0x560;
};

// The sound handle (as CFulmiteBellyMineAIComponent_Sound_ovb0132.cpp sees it).
struct UnkBellyMineSound_l57c3d8 {
    void* ref;
    s64 serial;
};

struct UnkBellyMineState_l57c3d8 {
    UnkBellyMineSink_l57c3d8* sink;  // 0x146c8
    UnkBellyMineDef_l57c3d8* def;    // 0x146d0
    UnkBellyMineSound_l57c3d8 sound; // 0x146d8
    u32 field_0x146e8;
    u32 field_0x146ec;
    UnkActor* carrier;  // 0x146f0
    bool done;          // 0x146f8
    bool field_0x146f9;
    bool field_0x146fa;
    bool field_0x146fb;
    bool field_0x146fc;
    u8 pad_0x146fd[0xf];
    bool field_0x1470c;
    CVector2D dir;  // 0x14710
    u32 field_0x14718;
    bool field_0x1471c;
    u8 pad_0x1471d[0xb];
    bool field_0x14728;
    u8 pad_0x14729[0xb];
    u32 field_0x14734;
    CVector3D home;  // 0x14738
    CVector3D target;  // 0x14744
    bool has_target;   // 0x14750
    bool in_shape;     // 0x14751
    u32 field_0x14754;
};
static_assert(sizeof(UnkBellyMineState_l57c3d8) == 0x90, "layout");

struct UnkBellyMineAI_l57c3d8 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    UnkBellyMineState_l57c3d8 state;  // 0x146c8
};

// Slot 0xd8 of the actor's vtable: sets its rotation.
static inline void SetRotation(UnkActor* actor, const CVector3D* angles, bool now) {
    reinterpret_cast<void (*)(UnkActor*, const CVector3D*, bool)>(actor->vtable[0xd8 / 8])(actor, angles, now);
}

static inline const UnkMatrix44& Matrix(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

static inline bool IsNearZero(f32 v, f32 eps) {
    return v - eps <= 0.0f && v + eps >= 0.0f;
}

void fn_710057c3d8(UnkBellyMineAI_l57c3d8* self) {
    UnkBellyMineState_l57c3d8& state = self->state;
    fn_7100369650(reinterpret_cast<char*>(self));
    UnkActor* owner = self->owner;
    state.sink = static_cast<UnkBellyMineSink_l57c3d8*>(owner->field_0x218);
    state.field_0x146e8 = state.def->field_0x50c;
    state.field_0x146ec = 0;
    state.field_0x14754 = state.def->field_0x560;
    state.done = false;
    state.field_0x146f9 = false;
    state.field_0x146fa = false;
    CVector3D home = {owner->field_0x44, owner->field_0x54, owner->field_0x64};
    state.field_0x14734 = 0;
    state.home = home;
    state.field_0x146fb = false;
    state.field_0x146fc = true;
    state.field_0x14718 = 0;
    state.field_0x1470c = false;
    state.field_0x1471c = false;
    state.field_0x14728 = false;
    state.target.x = 0.0f;
    state.target.y = 0.0f;
    state.target.z = 0.0f;
    state.has_target = false;
    {
        CVector3D angles;
        angles.x = 0.0f;
        angles.y = -1.5707964f;
        angles.z = 0.0f;
        SetRotation(owner, &angles, true);
    }
    state.in_shape = fn_710057c778(reinterpret_cast<UnkMineAI_A3_b70*>(self));

    // Faces right (+x) or left, by the carrier's orientation.
    const CVector2D right = {1.0f, 0.0f};
    const CVector2D left = {-1.0f, -0.0f};
    if (!IsNearZero(Matrix(state.carrier).m[2], 0.01f))
        state.dir = Matrix(state.carrier).m[2] < 0.0f ? right : left;
    else
        state.dir = Matrix(state.carrier).m[1] < 0.0f ? left : right;

    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649f68), &mask.id, false, -1.0f);
    }

    UnkBellyMineParams_l57c3d8 params;
    params.field_0x30 = false;
    params.field_0x20 = 0;
    params.field_0x28 = 0;
    params.vtable = g_unk_7101a25b10 + 2;
    params.kind = 5;
    params.dir.x = state.dir.x;
    params.dir.y = state.dir.y;
    params.field_0x39 = false;
    params.field_0x3b = false;
    params.field_0x3d = false;
    params.has_scale = true;
    params.dir.z = 0.0f;
    params.has_dir = true;
    params.scale = 1.0f;
    fn_71006f4b0c(self->owner->field_0x298, &params, 1);

    UnkBellyMineSinkParams_l57c3d8 sink_params;
    sink_params.has_0x8 = false;
    sink_params.has_0x10 = false;
    sink_params.vtable = g_unk_7101a24b80 + 2;
    sink_params.kind = 0;
    sink_params.up.x = 0.0f;
    sink_params.up.y = 0.0f;
    sink_params.up.z = 0.0f;
    sink_params.has_up = false;
    sink_params.field_0x18.x = 0.0f;
    sink_params.field_0x18.y = 0.0f;
    sink_params.field_0x18.z = 0.0f;
    sink_params.has_0x18 = false;
    const CVector3D down = {-0.0f, -1.0f, -0.0f};
    CVector3D up;
    if (IsNearZero(Matrix(state.carrier).m[5], 0.001f)) {
        up = down;
    } else {
        // The original reads the carrier's matrix once more here and does not use it.
        Matrix(state.carrier);
        up.x = state.dir.x;
        up.y = state.dir.y;
        up.z = 0.0f;
    }
    sink_params.up = up;
    sink_params.has_up = true;
    fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(state.sink), &sink_params, 1);
    UnkBellyMineSink_l57c3d8* sink = state.sink;
    if (sink->active != true) {
        sink->active = true;
        sink->vf_0x260();
    }
    state.sound = reinterpret_cast<const UnkBellyMineSound_l57c3d8&>(g_unk_7108b6c460);
}
