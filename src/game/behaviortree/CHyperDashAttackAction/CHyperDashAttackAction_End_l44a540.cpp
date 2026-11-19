// behaviortree::CHyperDashAttackAction's unit (tools/tu_map.py, unit 662): the node's end. Clears the dash's
// blackboard record and the commander's flag; a dash that was under way resets the owner's effect sink
// and drops the weak handle at +0xa0, and unless the node ended with status 5 the owner's timeline gets
// its id with an all-bits mask. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CEnemyMovement.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CHyperDashAttackAction.h"
#include "game/behaviortree/CSetAnimDependsOnDirAction.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::CClass;

// Signatures as other files declare them.
struct UnkSelf;
struct UnkEffectSink;

// The blackboard record the dash sets.
extern const u64 g_unk_71016496a8;  // a timeline id, reached through the GOT

// A small flag value whose constructors are not constexpr, so its statics are guarded.
struct UnkFlags_l44a540 {
    u32 value;
    UnkFlags_l44a540() { value = 0; }
};

struct UnkMask_l44a540 {
    u32 value;
    explicit UnkMask_l44a540(const UnkFlags_l44a540& f) { value = (u8)~f.value; }
};

inline const UnkFlags_l44a540& UnkNone_l44a540() {
    static UnkFlags_l44a540 s_value;
    return s_value;
}

inline const UnkMask_l44a540& UnkAll_l44a540() {
    static UnkMask_l44a540 s_value(UnkNone_l44a540());
    return s_value;
}

// The block sent to the effect sink (vtable g_unk_7101a24db8).
struct UnkSinkParams_l44a540 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
    u8 pad_0x3e;
    u8 field_0x3f;
    u8 pad_0x40;
    u8 field_0x41;
    u8 pad_0x42[2];
    u8 field_0x44[13];
    u8 pad_0x51;
    u8 field_0x52;
};

struct UnkDashAI_l44a540 {
    void** vtable;
    u8 pad_0x8[0x1140 - 0x8];
    s32 mode;  // 0x1140
    u8 pad_0x1144[0x15ed0 - 0x1144];
    bool field_0x15ed0;

    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkDashAI_l44a540*)>(vtable[0])(this); }
};

struct UnkDashActor_l44a540 {
    u8 pad_0x0[0x218];
    UnkEffectSink* sink;  // 0x218
    u8 pad_0x220[0x20];
    UnkDashAI_l44a540* ai;  // 0x240
    u8 pad_0x248[0x2e8 - 0x248];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkHyperDashAttackAction_l44a540 {
    u8 pad_0x0[0x18];
    UnkDashActor_l44a540* actor;  // 0x18
    u8 pad_0x20[0x70 - 0x20];
    bool dashing;  // 0x70
    u8 pad_0x71[0x88 - 0x71];
    u8 link[0x18];  // 0x88
    UnkWeakHandle* handle;  // 0xa0
};

void fn_710044a540(UnkHyperDashAttackAction_l44a540* self, s32 status) {
    fn_7100394bd4(self);
    fn_7100395474(reinterpret_cast<UnkSelf*>(self), g_unk_71726c8f08, false);
    UnkDashAI_l44a540* commander = nullptr;
    if (self->actor) {
        UnkDashAI_l44a540* ai = self->actor->ai;
        if (ai && fn_710009d50c(ai->vf_0x0(), fn_710044e708()))
            commander = ai;
    }
    if (commander)
        commander->field_0x15ed0 = false;
    if (!self->dashing)
        return;
    self->dashing = false;
    if (commander && commander->mode != 1) {
        if (UnkEffectSink* sink = self->actor->sink) {
            UnkSinkParams_l44a540 params;
            params.field_0xc = 0;
            params.field_0x14 = 0;
            __builtin_memset(params.field_0x18, 0, 13);
            __builtin_memset(params.field_0x28, 0, 13);
            params.vtable = g_unk_7101a24db8 + 2;
            params.field_0x3d = 0;
            params.field_0x3f = 0;
            params.field_0x41 = 0;
            params.field_0x52 = 0;
            __builtin_memset(params.field_0x44, 0, 13);
            params.field_0x38 = 1;
            fn_71010c7e6c(sink, &params, 1);
        }
    }
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->link);
        ResetRef(&self->handle);
    }
    if (status == 5)
        return;
    if (UnkTimelineComponent* timeline = self->actor->timeline) {
        if (!fn_7100a2be94(timeline, &g_unk_71016496a8)) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_l44a540().value;
            fn_7100a2cb7c(timeline, &g_unk_71016496a8, &mask.id, false, -1.0f);
        }
    }
}
