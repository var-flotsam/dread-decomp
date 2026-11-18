// behaviortree::CRotateToPlayer's unit: runs the action. The unit direction from the actor (+0x18) to the
// actor it acts against (fn_7100d7c990), along x only when +0x34 is set, goes as the desired direction to
// the actor's target object (+0x298, fn_71006f4b0c) or else to its effect sink (+0x218, fn_71010c7e6c).
// Returns 2 when +0x2c is set, 5 otherwise. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector3D;

template <class T>
struct UnkOpt_l38f50c {
    T value;
    bool has;

    // Clears the value and the flag together (the original zeroes both in one store run).
    void Clear() { __builtin_memset(this, 0, sizeof(T) + 1); }
};

// The query sent to the target object (vtable g_unk_7101a24b58).
struct UnkRotateQuery_l38f50c {
    void** vtable;
    UnkOpt_l38f50c<CVector3D> dir;  // 0x8
    UnkOpt_l38f50c<u32> field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    bool field_0x30;
    u32 field_0x34;

    UnkRotateQuery_l38f50c() {
        vtable = g_unk_7101a24b58 + 2;
        dir.Clear();
        field_0x18.has = false;
        field_0x20 = 0;
        field_0x28 = 0;
        field_0x30 = false;
        field_0x34 = 0;
    }
};
static_assert(sizeof(UnkRotateQuery_l38f50c) == 0x38, "layout");

// The block sent to the effect sink (vtable g_unk_7101a24b80, kind 0).
struct UnkRotateSinkParams_l38f50c {
    void** vtable;
    UnkOpt_l38f50c<u32> field_0x8;
    UnkOpt_l38f50c<f32> field_0x10;
    UnkOpt_l38f50c<CVector3D> dir;  // 0x18
    UnkOpt_l38f50c<CVector3D> field_0x28;
    u32 kind;  // 0x38

    UnkRotateSinkParams_l38f50c() {
        vtable = g_unk_7101a24b80 + 2;
        field_0x8.has = false;
        field_0x10.has = false;
        kind = 0;
        dir.Clear();
        field_0x28.Clear();
    }
};
static_assert(__builtin_offsetof(UnkRotateSinkParams_l38f50c, kind) == 0x38, "layout");

struct UnkRotateToPlayer_l38f50c {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
    u8 pad_0x20[0xc];
    bool done;  // 0x2c
    u8 pad_0x2d[7];
    bool horizontal;  // 0x34
};

s32 fn_710038f50c(UnkRotateToPlayer_l38f50c* self) {
    UnkActor* player = fn_7100d7c990(self->actor);
    UnkActor* actor = self->actor;
    CVector3D to = {player->field_0x44, player->field_0x54, player->field_0x64};
    CVector3D from = {actor->field_0x44, actor->field_0x54, actor->field_0x64};
    CVector3D d = {to.x - from.x, to.y - from.y, to.z - from.z};
    if (self->horizontal) {
        d.y = 0.0f;
        d.z = 0.0f;
    }
    f32 inv = 1.0f / (sqrtf(d.x * d.x + d.y * d.y + d.z * d.z) + 0.0001f);
    CVector3D dir = {d.x * inv, d.y * inv, d.z * inv};
    if (void* target = self->actor->field_0x298) {
        UnkRotateQuery_l38f50c query;
        query.dir.value = dir;
        query.dir.has = true;
        fn_71006f4b0c(target, &query, 0);
    } else if (UnkEffectSink* sink = static_cast<UnkEffectSink*>(self->actor->field_0x218)) {
        UnkRotateSinkParams_l38f50c params;
        params.dir.value = dir;
        params.dir.has = true;
        fn_71010c7e6c(sink, &params, 0);
    }
    return self->done ? 2 : 5;
}
