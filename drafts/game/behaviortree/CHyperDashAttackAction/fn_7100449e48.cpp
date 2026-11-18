// behaviortree::CHyperDashAttackAction's unit (tools/tu_map.py, unit 662): the node's run. While the dash
// track plays, steps the dash by its phase (+0x6c); before the dash has started, tries to start it and
// otherwise gives up the attack order. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAttackComponent.h"
#include "game/CEnemyMovement.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBufferStringPool.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "types.h"

#include <math.h>

// Signatures as other files declare them.
struct UnkAttackActor_ovb0128;
struct UnkCancelOwner_ovb0128;
struct UnkSelf;
struct UnkEffectSink;
void* fn_71003729a8(UnkAttackActor_ovb0128* actor);
void fn_7100395474(UnkSelf* behavior, void* record, bool value);

// The blackboard record the run clears when the dash cannot start.
extern u8 g_unk_71726c8f08[];

template <class T>
struct UnkOpt_l449e48 {
    T value;
    bool has;
};

struct UnkVec3_l449e48 {
    f32 x, y, z;
};

// The block sent to the effect sink (vtable g_unk_7101a24b80, kind 0).
struct UnkSinkParams_l449e48 {
    void** vtable;
    UnkOpt_l449e48<u32> field_0x8;
    UnkOpt_l449e48<f32> field_0x10;
    UnkOpt_l449e48<UnkVec3_l449e48> field_0x18;
    UnkOpt_l449e48<UnkVec3_l449e48> field_0x28;
    u32 kind;  // 0x38
};

struct UnkDashActor_l449e48 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0x218 - 0x48];
    UnkEffectSink* sink;  // 0x218
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkHyperDashAttackAction_l449e48 {
    u8 pad_0x0[0x18];
    UnkDashActor_l449e48* actor;  // 0x18
    u8 pad_0x20[0x69 - 0x20];
    bool started;  // 0x69
    u8 pad_0x6a[2];
    s32 phase;  // 0x6c
    bool field_0x70;
    u8 pad_0x71[3];
    f32 target_x;  // 0x74
    u8 pad_0x78[8];
    s32 direction;  // 0x80
};

void* fn_7100449868(UnkHyperDashAttackAction_l449e48* self);
bool fn_7100449bdc(UnkHyperDashAttackAction_l449e48* self, void* target, bool flag);
void fn_710044a010(UnkHyperDashAttackAction_l449e48* self);
void fn_710044ae1c(UnkHyperDashAttackAction_l449e48* self);

u32 fn_7100449e48(UnkHyperDashAttackAction_l449e48* self) {
    if (!self->started && fn_710117b348(self->actor->animation, fn_7100c25518())) {
        switch (self->phase) {
        case 1:
        case 2:
        case 3:
            fn_710044a010(self);
            return 2;
        case 0:
            break;
        default:
            return 2;
        }
        UnkDashActor_l449e48* actor = self->actor;
        f32 x = actor->x;
        f32 delta = self->target_x - x;
        s32 side = (delta > 0.0f) - (delta < 0.0f);
        if ((f32)side != (f32)self->direction)
            delta = 0.0f;
        if (UnkEffectSink* sink = actor->sink) {
            UnkSinkParams_l449e48 params;
            params.kind = 0;
            __builtin_memset(&params.field_0x28, 0, 13);
            params.vtable = g_unk_7101a24b80 + 2;
            __builtin_memset(&params.field_0x18, 0, 13);
            params.field_0x8.has = false;
            params.field_0x10.value = fabsf(delta);
            params.field_0x10.has = true;
            fn_71010c7e6c(sink, &params, 0);
        }
        if (fabsf(delta) < 10.0f)
            fn_710044ae1c(self);
        return 2;
    }
    if (!self->started) {
        void* target = fn_7100449868(self);
        self->started = fn_7100449bdc(self, target, self->field_0x70);
        if (self->started)
            return 2;
        char* instance = g_unk_71726b2398;
        if (!instance) {
            instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
            fn_71011a664c(instance);
            g_unk_71726b2398 = instance;
        }
        fn_71011a75f0(instance, reinterpret_cast<UnkCancelOwner_ovb0128*>(self->actor));
        fn_7100395474(reinterpret_cast<UnkSelf*>(self), g_unk_71726c8f08, false);
        return 6;
    }
    if (self->actor) {
        void* player = fn_71003729a8(reinterpret_cast<UnkAttackActor_ovb0128*>(self->actor));
        if (player && fn_7101136308(player))
            return 2;
    }
    return 5;
}
