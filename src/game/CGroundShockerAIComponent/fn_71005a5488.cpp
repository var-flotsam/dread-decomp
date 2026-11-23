// CGroundShockerAIComponent's unit (tools/tu_map.py unit 861): after the base step (fn_7101108124), restores the
// timer at +0x14738 from the tunable (+0x38) when it ran out while +0x14730 is set, faces the actor toward the
// target (+-0.9999 along x) through its effect sink (+0x218) and +0x298, turns it (vf_0x348) when it looks the
// other way, and resets the state at +0x14678 with the new direction. Names and layouts are placeholders.
#include "base/reflection/ReflectedAttributes.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "game_s2/UnkParams.h"
#include "game/CCharClassGroundShockerAttack.h"
#include "types.h"

using base::reflection::UnkTunable;


// The tunable as this function reads it.
struct UnkGroundShockerTunable_l5a5488 {
    u8 pad_0x0[0x38];
    f32 field_0x38;
};

template <class T>
struct UnkOpt_l5a5488 {
    T value;
    bool has;
};

struct UnkDir_l5a5488 {
    f32 x, y, z;
};

// The block sent to the effect sink (vtable g_unk_7101a24b80, kind 0).
struct UnkSinkParams_l5a5488 {
    void** vtable;
    UnkOpt_l5a5488<u32> field_0x8;
    UnkOpt_l5a5488<f32> field_0x10;
    UnkOpt_l5a5488<UnkDir_l5a5488> field_0x18;
    UnkOpt_l5a5488<UnkDir_l5a5488> field_0x28;
    u32 kind;  // 0x38
};

// The facing query handed to +0x298 (vtable g_unk_7101a24b58).
struct UnkFaceQuery_l5a5488 {
    void** vtable;
    UnkOpt_l5a5488<UnkDir_l5a5488> dir;  // 0x8
    u8 pad_0x18[4];
    bool field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    bool field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
};
static_assert(sizeof(UnkFaceQuery_l5a5488) == 0x38, "layout");

struct UnkGroundShockerSink_l5a5488 {
    void** vtable;

    UnkVec3f vf_0x370() {
        return reinterpret_cast<UnkVec3f (*)(UnkGroundShockerSink_l5a5488*)>(vtable[0x370 / 8])(this);
    }
    void vf_0x348() { reinterpret_cast<void (*)(UnkGroundShockerSink_l5a5488*)>(vtable[0x348 / 8])(this); }
};

struct UnkGroundShockerFaceState_l5a5488 {
    s32 field_0x0;
    f32 dir;  // 0x4
    u64 field_0x8;
    u8 pad_0x10[0xa8];
    void* field_0xb8;  // 0x14730
    f32 timer;         // 0x14738
};

struct UnkGroundShockerFaceAI_l5a5488 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x14678 - 0x38];
    UnkGroundShockerFaceState_l5a5488 state;  // 0x14678
};
static_assert(__builtin_offsetof(UnkGroundShockerFaceAI_l5a5488, state.timer) == 0x14738, "layout");

void fn_71005a5488(UnkGroundShockerFaceAI_l5a5488* self, UnkActor* target) {
    UnkGroundShockerFaceState_l5a5488* st = &self->state;
    fn_7101108124(reinterpret_cast<UnkQuetzoaAIComponent*>(self), target);
    if (st->field_0xb8 && st->timer < 0.001f)
        st->timer = reinterpret_cast<UnkGroundShockerTunable_l5a5488*>(&g_unk_71726e7be0)->field_0x38;

    f32 dir = target->field_0x44 - self->owner->field_0x44 > 0.0f ? 0.9999f : -0.9999f;

    UnkSinkParams_l5a5488 params;
    params.vtable = g_unk_7101a24b80 + 2;
    params.field_0x8.has = false;
    params.field_0x10.has = false;
    params.kind = 0;
    __builtin_memset(&params.field_0x18, 0, 13);
    __builtin_memset(&params.field_0x28, 0, 13);
    params.field_0x18.value = {dir, 0.0f, 0.0f};
    params.field_0x18.has = true;
    fn_71010c7e6c(static_cast<UnkEffectSink*>(self->owner->field_0x218), &params, 0);

    UnkFaceQuery_l5a5488 query;
    query.vtable = g_unk_7101a24b58 + 2;
    __builtin_memset(&query.dir, 0, 13);
    query.field_0x1c = false;
    query.field_0x20 = 0;
    query.field_0x28 = 0;
    query.field_0x30 = false;
    query.field_0x34 = 0;
    query.dir.value = {dir, 0.0f, 0.0f};
    query.dir.has = true;
    fn_71006f4b0c(self->owner->field_0x298, &query, 0);

    UnkVec3f facing = static_cast<UnkGroundShockerSink_l5a5488*>(self->owner->field_0x218)->vf_0x370();
    if (dir * facing.x + facing.y * 0.0f + facing.z * 0.0f < 0.0f)
        static_cast<UnkGroundShockerSink_l5a5488*>(self->owner->field_0x218)->vf_0x348();

    st->dir = dir;
    st->field_0x8 = 0;
    st->field_0x0 = -1;
}
