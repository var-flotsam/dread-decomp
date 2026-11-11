// CCharClassSetDesiredViewDirToPlayerEvent's unit (tools/tu_map.py, the name from reflection): runs the
// event on the context's actor: with a component at +0x1f0 and an actor it acts against
// (fn_7100d7c990), the view direction is +1 or -1 along x, towards that actor. A direction away from
// zero is stored in the component (+0x70, as a vector), and the actor's effect sink (+0x218) gets a
// parameter block with the direction set (as src/game/UnkUnit1123/UnkUnit1123_b38.cpp builds one).
// Names and layouts are placeholders.
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "types.h"

struct UnkVec3_b49 {
    f32 x;
    f32 y;
    f32 z;
};

template <class T>
struct UnkOpt_b49 {
    T value;
    bool has;
};

// The block sent to the effect sink (vtable g_unk_7101a24b80, kind 0).
struct UnkSinkParams_b49 {
    void** vtable;
    UnkOpt_b49<u32> field_0x8;
    UnkOpt_b49<f32> field_0x10;
    UnkOpt_b49<UnkVec3_b49> field_0x18;
    UnkOpt_b49<UnkVec3_b49> field_0x28;
    u32 kind;  // 0x38
};

struct UnkEffectSink;

struct UnkViewDirComponent {
    u8 pad_0x0[0x70];
    UnkVec3_b49 desired_dir;  // 0x70
};

struct UnkViewDirContext {
    UnkActor* actor;
};

static inline UnkViewDirComponent* ViewDir(UnkActor* actor) {
    return *reinterpret_cast<UnkViewDirComponent**>(reinterpret_cast<char*>(actor) + 0x1f0);
}

void fn_7100bd305c(void* self, UnkViewDirContext* context) {
    UnkActor* actor = context->actor;
    if (!actor)
        return;
    UnkViewDirComponent* component = ViewDir(actor);
    UnkActor* other = fn_7100d7c990(actor);
    if (!component || !other)
        return;
    f32 dir = other->field_0x44 > context->actor->field_0x44 ? 1.0f : -1.0f;
    if (!(dir + 0.001f >= 0.0f && dir - 0.001f <= 0.0f))
        component->desired_dir = {dir, 0.0f, 0.0f};
    if (UnkEffectSink* sink = static_cast<UnkEffectSink*>(context->actor->field_0x218)) {
        UnkSinkParams_b49 params;
        params.vtable = g_unk_7101a24b80 + 2;
        params.field_0x8.has = false;
        params.field_0x10.has = false;
        params.kind = 0;
        __builtin_memset(&params.field_0x18, 0, 13);
        __builtin_memset(&params.field_0x28, 0, 13);
        params.field_0x18.value = {dir, 0.0f, 0.0f};
        params.field_0x18.has = true;
        fn_71010c7e6c(sink, &params, 0);
    }
}
