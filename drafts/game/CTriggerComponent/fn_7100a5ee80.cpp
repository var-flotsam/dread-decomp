// CTriggerComponent unit (tools/tu_map.py, unit 1578): sends an event of a kind about an actor. Each of the
// component's conditions of that kind (kind 3 with a delay: once the delay and two more times have passed
// the game time) that is enabled and not yet done, and whose class name (when it has one) is the actor's
// char class's, hands the actor to its handlers; a condition that does not repeat is then done and, with
// +0x278 set, writes "ConditionDone" to the scenario's blackboard section. Last, the owner's script gets
// the kind's function with the actor's name, the direction and whether any condition matched. Names and
// layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkSubId_7100fc4b34.h"
#include "game/UnkTU2553.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkTriggerComponent_ovb0067;
struct UnkTriggerActor_ovb0067;
struct UnkVec3f;

// As src/game/CTriggerComponent/CTriggerComponent_b47.cpp declares them.
struct UnkTriggerListElement {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(void* owner);
};
UnkTriggerListElement** fn_71007a804c(void* list, u32 index);

// As src/game/CTriggerComponent/fn_7100a64bb0.cpp declares it: the "ConditionDone" property.
extern UnkProperty g_unk_717274d6f0;
// The script function of each event kind.
extern const char* g_unk_7101a8f598[];

struct UnkTriggerActor_la5ee80;

// What a condition hands the actor to.
struct UnkTriggerHandler_la5ee80 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkTriggerActor_la5ee80* actor);
};

struct UnkTriggerCondition_la5ee80 {
    CStrId name;
    s32 kind;  // 0x8
    CStrId class_name;  // 0x10
    bool enabled;  // 0x18
    bool repeat;  // 0x19
    f32 delay;  // 0x1c
    u8 pad_0x20[4];
    f32 field_0x24;
    f32 field_0x28;
    u8 pad_0x2c[4];
    u8 handlers[8];  // 0x30
    u32 handler_count;  // 0x38
    u8 pad_0x3c[0x1c];
    bool done;  // 0x58: "ConditionDone"
};

struct UnkTriggerCharClass_la5ee80 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x90];
    CStrId name;  // 0x98
};

struct UnkTriggerActor_la5ee80 {
    u8 pad_0x0[0x28];
    UnkTriggerCharClass_la5ee80* char_class;  // 0x28
    CStrId name;  // 0x30
};

struct UnkTriggerScript_la5ee80 {
    u8 pad_0x0[0x98];
    const bool* field_0x98;
};

struct UnkTriggerOwner_la5ee80 {
    u8 pad_0x0[0x1e0];
    UnkTriggerScript_la5ee80* script;  // 0x1e0
};

struct UnkTriggerComponent_la5ee80 {
    u8 pad_0x0[0x30];
    UnkTriggerOwner_la5ee80* owner;  // 0x30
    u8 pad_0x38[5];
    bool field_0x3d;
    u8 pad_0x3e[0xc];
    bool calls_script[6];  // 0x4a, by event kind
    u8 pad_0x50[0x200];
    base::global::CRntVector<UnkTriggerCondition_la5ee80*> conditions;  // 0x250
    bool field_0x278;
    u8 pad_0x279[0xf];
    CStrId prefix;  // 0x288
    u8 pad_0x290[0x28];
    UnkQualifiedName qualified_name;  // 0x2b8
};
static_assert(__builtin_offsetof(UnkTriggerComponent_la5ee80, conditions) == 0x250, "layout");
static_assert(__builtin_offsetof(UnkTriggerComponent_la5ee80, qualified_name) == 0x2b8, "layout");

static inline bool IsEmpty(const CStrId& id) {
    return &g_unk_7101d47108->empty == id.data;
}

// The string of an id, counting the read.
static inline const CRntString& StringOf(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// The game time (+0x2444 of the game manager).
static inline f32 GameTime() {
    return g_unk_7101d47310->field_0x2444;
}

// Whether `condition` answers an event of `kind`: one of kind 3 with a delay only once the delay and
// the two times after it have passed.
static inline bool Answers(const UnkTriggerCondition_la5ee80* condition, s32 kind) {
    if (condition->kind == 3 && condition->delay >= 0.0f)
        return kind == 3 && condition->delay + condition->field_0x24 + condition->field_0x28 < GameTime();
    return condition->kind == kind;
}

// Writes the condition's "ConditionDone" to the scenario's blackboard section.
static inline void WriteDone(UnkTriggerComponent_la5ee80* self, UnkTriggerCondition_la5ee80* condition,
                             UnkGameManager* manager, const CStrId& scenario) {
    UnkQualifiedName* name = &self->qualified_name;
    if (!name->str || !name->str[0]) {
        CStrId prefix = self->prefix;
        fn_7100ae9d7c(name, reinterpret_cast<UnkIdSource*>(&g_unk_717274d6f0), prefix);
    }
    const UnkSubId_7100fc4b34& id = fn_71003327a4(name, &condition->name);
    CBlackboard::CSection* section = fn_71010472e0(&manager->field_0x968, scenario);
    if (section && !manager->field_0x968.field_0xe8) {
        UnkTypedValue value = {fn_710000d228(), &condition->done};
        fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(const_cast<UnkSubId_7100fc4b34*>(&id)), &value);
    }
}

void fn_7100a5ee80(UnkTriggerComponent_ovb0067* component, s32 kind, UnkTriggerActor_ovb0067* who,
                   const UnkVec3f* dir) {
    UnkTriggerComponent_la5ee80* self = reinterpret_cast<UnkTriggerComponent_la5ee80*>(component);
    UnkTriggerActor_la5ee80* actor = reinterpret_cast<UnkTriggerActor_la5ee80*>(who);
    CStrId scenario = fn_7100de9c30(g_unk_7101d47310);
    bool matched = false;
    UnkGameManager* manager = g_unk_7101d47310;
    for (UnkTriggerCondition_la5ee80** it = self->conditions.data;
         it != self->conditions.data + self->conditions.count; ++it) {
        UnkTriggerCondition_la5ee80* condition = *it;
        if (!Answers(condition, kind) || !condition->enabled || (!condition->repeat && condition->done))
            continue;
        matched = true;

        if (!IsEmpty(condition->class_name)) {
            UnkTriggerCharClass_la5ee80* char_class = actor->char_class;
            if (!char_class || !fn_710009d50c(char_class->vf_0x0(), fn_71011abbd4()) ||
                StringOf(char_class->name).key != StringOf(condition->class_name).key)
                continue;
        }
        for (u32 i = 0; i < condition->handler_count; ++i) {
            UnkTriggerListElement* element = *fn_71007a804c(condition->handlers, i);
            reinterpret_cast<UnkTriggerHandler_la5ee80*>(element)->vf_0x20(actor);
        }
        if (condition->repeat)
            continue;

        condition->done = true;
        if (self->field_0x278)
            WriteDone(self, condition, manager, scenario);
    }

    UnkTriggerScript_la5ee80* script = self->owner->script;
    if (script && self->field_0x3d && script->field_0x98 && *script->field_0x98 &&
        self->calls_script[static_cast<u32>(kind)]) {
        self->calls_script[static_cast<u32>(kind)] =
            fn_7100305088(reinterpret_cast<UnkScriptCaller*>(script), g_unk_7101a8f598[static_cast<u32>(kind)],
                          "svb", actor->name.Text(), dir, matched);
    }
}
