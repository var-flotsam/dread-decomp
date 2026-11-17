// CEmmyGrabSamusAttack's unit: starts the grab. Tells the scenario script (CurrentScenario.OnEmmyGrabStart),
// notes whether the E.M.M.I. can be countered and rolls its two chances, picks the grab's first state (3 for
// the one-kind grab when the E.M.M.I. allows it, else 2), tells the player and the central-unit registry.
// Names and layouts are placeholders.
#include <stdlib.h>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CBlackboard.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkAttack.h"
#include "game/UnkEmmyGrabSamusAttack.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2547.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/behaviortree/CBehavior.h"
#include "game/lua_lib.h"
#include "game_s2/UnkChargeComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;

void fn_7100ddbabc(UnkUnit2547* self, u32 state, f32 value);
void fn_710116a150(void* self, CStrId* id);
extern CStrId g_unk_71726de5c8 __attribute__((visibility("hidden")));
// The blackboard property holding the grab's kind.
extern char g_unk_71726debe0[];

// The E.M.M.I.'s character class tunables (what its vf_0xd8 returns).
struct UnkEmmyChances_l506370 {
    u8 pad_0x0[0x100];
    f32 counter_chance;  // 0x100
    u8 pad_0x104[0x10];
    f32 second_chance;   // 0x114
};

struct UnkEmmyCharClass_l506370;

struct UnkEmmyCharClassVtable_l506370 {
    const CClass* (*GetClass)(const UnkEmmyCharClass_l506370* self);
    void* slots[0x1a];
    UnkEmmyChances_l506370* (*Chances)(UnkEmmyCharClass_l506370* self);  // 0xd8
};

struct UnkEmmyCharClass_l506370 {
    UnkEmmyCharClassVtable_l506370* vtable;
};

struct UnkEmmyAI_l506370;

struct UnkEmmyAIVtable_l506370 {
    const CClass* (*GetClass)(const UnkEmmyAI_l506370* self);
    void* slots[0x102];
    bool (*CanBeCountered)(UnkEmmyAI_l506370* self);  // 0x818
};

struct UnkEmmyAI_l506370 {
    UnkEmmyAIVtable_l506370* vtable;
    u8 pad_0x8[0x8];
    UnkEmmyCharClass_l506370* char_class;  // 0x10
    u8 pad_0x18[0x1260 - 0x18];
    CBlackboard blackboard;  // 0x1260
};

// The fields of the E.M.M.I. AI component read here, past its blackboard.
struct UnkEmmyAIState_l506370 {
    u8 pad_0x0[0x27720];
    bool field_0x27720;
    u8 pad_0x27721[0x29ef0 - 0x27721];
    s32 field_0x29ef0;
};

struct UnkModeHolder_l506370;

struct UnkModeHolderVtable_l506370 {
    void* slots[0xd];
    bool (*vf_0x68)(UnkModeHolder_l506370* self);
};

struct UnkModeHolder_l506370 {
    UnkModeHolderVtable_l506370* vtable;
};

struct UnkGrabPlayer_l506370 {
    u8 pad_0x0[0x1c0];
    void* field_0x1c0;
};

struct UnkEmmyGrabAttack_l506370 : UnkAttackBase {
    u8 pad_0x20[0x8];
    UnkGrabStateMachine_l507d24 state_machine;  // 0x28
    u8 pad_0xd8[0x790 - 0xd8];
    bool can_counter;  // 0x790
    u8 pad_0x791[0x7b1 - 0x791];
    bool counter_roll;  // 0x7b1
    bool second_roll;   // 0x7b2
};
static_assert(__builtin_offsetof(UnkEmmyGrabAttack_l506370, can_counter) == 0x790, "layout");
static_assert(__builtin_offsetof(UnkEmmyGrabAttack_l506370, second_roll) == 0x7b2, "layout");

static inline f32 Random01() { return (f32)((rand() & 0x7fff) * (1.0 / 32767.0)); }

static inline f32 RandomRange(f32 lo, f32 hi) { return Random01() * (hi - lo) + lo; }

static inline UnkEmmyAI_l506370* EmmyAI(UnkActor* owner) {
    return static_cast<UnkEmmyAI_l506370*>(owner->field_0x240);
}

static inline bool IsEmmyAI(UnkEmmyAI_l506370* ai) {
    return fn_710009d50c(ai->vtable->GetClass(ai), fn_710053b934());
}

static inline bool IsEmmyProto(UnkEmmyAI_l506370* ai) {
    return ai && fn_710009d50c(ai->vtable->GetClass(ai), fn_71004fa488());
}

static inline UnkEmmyAIState_l506370* EmmyState(UnkActor* owner) {
    UnkEmmyAI_l506370* ai = EmmyAI(owner);
    return reinterpret_cast<UnkEmmyAIState_l506370*>(IsEmmyAI(ai) ? ai : nullptr);
}

// Whether the character class plays CCharClassEmmyGrabOnlyImpactTrack.
static inline bool IsGrabOnlyImpact(UnkActor* owner) {
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(owner->field_0x220);
    return animation && fn_710117b348(animation, fn_7100c15fd8());
}

// The grab tunables (g_unk_71726e0aa8) as far as read here.
struct UnkGrabTunable_l506370 {
    u8 pad_0x0[0xf8];
    f32 registry_time;  // 0xf8
};

static inline UnkUnit2547* Registry() {
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    return reinterpret_cast<UnkUnit2547*>(reg);
}

void fn_7100506370(UnkEmmyGrabAttack_l506370* self) {
    fn_71003608ac(self);
    if (fn_71010c0bb0(g_unk_7172a01d18, "CurrentScenario.OnEmmyGrabStart")) {
        fn_7100fbc4e0(g_unk_7172a01d18, self->field_0x8);
        char error[0x100];
        fn_71010c0d80(g_unk_7172a01d18, 1, error, sizeof(error), nullptr, nullptr);
    }

    UnkEmmyAI_l506370* ai = EmmyAI(self->field_0x8);
    self->can_counter = ai && IsEmmyAI(ai) && ai->vtable->CanBeCountered(ai);
    self->counter_roll = false;
    self->second_roll = false;
    if (UnkEmmyAI_l506370* emmy = EmmyAI(self->field_0x8)) {
        UnkEmmyCharClass_l506370* char_class = emmy->char_class;
        if (char_class && fn_710009d50c(char_class->vtable->GetClass(char_class), fn_710051e010())) {
            self->counter_roll = RandomRange(0.0f, 100.0f) <= char_class->vtable->Chances(char_class)->counter_chance;
            self->second_roll = RandomRange(0.0f, 100.0f) <= char_class->vtable->Chances(char_class)->second_chance;
        }
    }

    s32 kind = 0;
    if (CBlackboard::CSection* section = fn_7101047270(&EmmyAI(self->field_0x8)->blackboard, g_unk_71726b7cb8))
        fn_7100395a30(section, g_unk_71726debe0, &kind);

    UnkModeHolder_l506370* mode = static_cast<UnkModeHolder_l506370*>(g_unk_7101d47310->field_0x498);
    if (mode->vtable->vf_0x68(mode)) {
        self->state_machine.Request(2);
    } else if (IsGrabOnlyImpact(self->field_0x8)) {
        self->state_machine.Request(2);
    } else if (EmmyState(self->field_0x8)->field_0x27720) {
        self->state_machine.Request(2);
    } else if (kind == 1 && EmmyState(self->field_0x8)->field_0x29ef0 == 1 &&
               (fn_71010cc3a4(self->field_0x8->field_0x218) == 1 || IsEmmyProto(EmmyAI(self->field_0x8)))) {
        self->state_machine.Request(3);
    } else {
        self->state_machine.Request(2);
    }

    if (auto* player = reinterpret_cast<UnkGrabPlayer_l506370*>(fn_7100dec788(g_unk_7101d47310))) {
        if (player->field_0x1c0)
            fn_710116a150(player->field_0x1c0, &g_unk_71726de5c8);
    }
    fn_7100ddbabc(Registry(), 0, reinterpret_cast<const UnkGrabTunable_l506370&>(g_unk_71726e0aa8).registry_time);
}
