// behaviortree::CEmmyChaseAction unit (tools/tu_map.py unit 801): the chase's update. Advances the action's
// timers, lets the EMMY's AI follow its target (or give up on it), and once the chase is lost with no id
// left it stores "lost" in the AI's blackboard and leaves with status 5; otherwise status 2. Names and
// layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CBlackboard.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2547.h"
#include "game/UnkTU811.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CEmmyChaseAction.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkEmmyChaseAction3_ovb0001;
bool fn_710051327c(UnkEmmyChaseAction3_ovb0001* self);
void fn_710051270c(UnkEmmyChaseAction3_ovb0001* self, f32 value);
struct UnkEmmyAI3_ovb0001;
void fn_7100546654(UnkEmmyAI3_ovb0001* ai);
struct UnkEmmy811;
void fn_71005480dc(UnkEmmy811* self, s32 phase);

extern char g_unk_71726df1a0[];
extern UnkPropertyName g_unk_71726df1b8;

// The EMMY's AI component (CEmmyAIComponent).
struct UnkEmmyAI_l512e60 : UnkReflObject {
    u8 pad_0x8[0x8];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x1260 - 0x18];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x146c8 - 0x1350];
    u64 chase_id;  // 0x146c8
    u8 pad_0x146d0[0x14b00 - 0x146d0];
    f32 field_0x14b00;

    // vtable slot 0x808: how long the chase may run.
    f32 vf_0x808() {
        return reinterpret_cast<f32 (*)(UnkEmmyAI_l512e60*)>((*reinterpret_cast<void***>(this))[0x808 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkEmmyAI_l512e60, blackboard) == 0x1260, "layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l512e60, chase_id) == 0x146c8, "layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l512e60, field_0x14b00) == 0x14b00, "layout");

struct UnkChasePath_l512e60;
UnkChasePath_l512e60* fn_710054abc4(UnkEmmyAI_l512e60* self, UnkActor* target);
bool fn_710054b390(UnkEmmyAI_l512e60* self, UnkChasePath_l512e60* path);

struct UnkChaseTargetInfo_l512e60 {
    u8 pad_0x0[0x7b9];
    bool field_0x7b9;
};

// The target actor, as far as this function reads it.
struct UnkChaseTarget_l512e60 {
    u8 pad_0x0[0x200];
    UnkChaseTargetInfo_l512e60* info;  // 0x200
};

struct UnkCentralState_l512e60 {
    u8 pad_0x0[0x10];
    s32 id;  // 0x10
};

struct UnkCentralUnit_l512e60 {
    u8 pad_0x0[0x40];
    UnkCentralState_l512e60* state;  // 0x40
};

struct UnkEmmyChaseAction_l512e60 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x2c - 0x20];
    f32 field_0x2c;
    u8 pad_0x30[0x58 - 0x30];
    f32 field_0x58;
    f32 field_0x5c;
};

bool fn_7100513174(UnkEmmyChaseAction_l512e60* self);
void fn_71005134a8(UnkEmmyChaseAction_l512e60* self);

template <class T>
static inline void SetProperty(CBlackboard* blackboard, UnkPropertyName* name, T* value) {
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = base::reflection::UnkReflTypeOf<T>::Get();
            v.value = value;
            fn_7100332c80(section, name, &v);
        }
    }
}

// The lazily created registry's central unit.
static inline UnkCentralUnit_l512e60* CentralUnit() {
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    return reinterpret_cast<UnkCentralUnit_l512e60*>(fn_7100ddb768(reinterpret_cast<UnkUnit2547*>(reg)));
}

// The AI and its character class component, both null without an AI.
struct UnkChaseParts_l512e60 {
    UnkEmmyAI_l512e60* ai;
    UnkReflObject* char_class;
};

static inline UnkChaseParts_l512e60 Parts(UnkEmmyAI_l512e60* emmy) {
    UnkChaseParts_l512e60 parts = {nullptr, nullptr};
    if (!emmy)
        return parts;
    parts.ai = emmy;
    UnkReflObject* char_class = emmy->char_class;
    if (char_class && fn_710009d50c(char_class->vf_0x0(), fn_710051e010()))
        parts.char_class = char_class;
    return parts;
}

s32 fn_7100512e60(UnkEmmyChaseAction_l512e60* self, f32 dt) {
    // The AI component's blackboard is reached through the cast's own result.
    UnkEmmyAI_l512e60* emmy = static_cast<UnkEmmyAI_l512e60*>(self->owner->field_0x240);
    if (emmy && !fn_710009d50c(emmy->vf_0x0(), fn_710053b934()))
        emmy = nullptr;
    UnkChaseParts_l512e60 parts = Parts(emmy);
    UnkEmmyAI_l512e60* ai = parts.ai;
    UnkReflObject* char_class = parts.char_class;

    if (fn_7100d7bea0(self->owner, 1)) {
        self->field_0x58 = 0.0f;
        self->field_0x5c += dt;
    } else {
        self->field_0x58 += dt;
    }
    fn_710051270c(reinterpret_cast<UnkEmmyChaseAction3_ovb0001*>(self), dt);

    if (ai->chase_id != ~0ull) {
        if (UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220)) {
            if (!fn_710117ae38(animation, ai->chase_id, false))
                fn_7100546654(reinterpret_cast<UnkEmmyAI3_ovb0001*>(ai));
        }
    }

    UnkActor* target = fn_7100d7c990(self->owner);
    if (!char_class || !target)
        return 2;

    UnkChasePath_l512e60* path = fn_710054abc4(ai, target);
    bool in_view = fn_710054b390(ai, path);
    bool timed_out = false;
    if (ai->field_0x14b00 < 15000.0f)
        timed_out = self->field_0x58 >= ai->vf_0x808();

    if (!path || (fn_7100513174(self) && (timed_out || !in_view))) {
        fn_71005480dc(reinterpret_cast<UnkEmmy811*>(ai), 1);
    } else {
        UnkChaseTargetInfo_l512e60* info = reinterpret_cast<UnkChaseTarget_l512e60*>(target)->info;
        if (info && !info->field_0x7b9) {
            UnkCentralState_l512e60* state = CentralUnit()->state;
            if ((state && (state->id == 2 || state->id == 6)) || fn_710009d50c(ai->vf_0x0(), fn_71004fa488()))
                fn_7100547428(reinterpret_cast<UnkEmmyActor_b55*>(ai), 2, target, in_view);
        }
    }

    if (!fn_710051327c(reinterpret_cast<UnkEmmyChaseAction3_ovb0001*>(self)) && ai->chase_id == ~0ull) {
        bool lost = true;
        SetProperty(&emmy->blackboard, &g_unk_71726df1b8, &lost);
        fn_7100544a34(reinterpret_cast<UnkEmmyActor_b55*>(ai), g_unk_71726df1a0, true);
        return 5;
    }
    fn_71005134a8(self);
    self->field_0x2c += dt;
    return 2;
}
