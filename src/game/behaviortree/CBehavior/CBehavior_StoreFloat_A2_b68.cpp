// behaviortree::CBehavior's unit (unit 500 of tools/tu_map.py, the name from reflection): stores a float under
// `name` in the blackboard section g_unk_71726b7cb8 of the AI component (+0x240) of the behaviour's actor (+0x18),
// unless the blackboard (+0x1260) is locked (the setter as in src/game/CAIComponent/CAIComponent_StoreFloat_b49.cpp).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkTUProperty.h"
#include "types.h"

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

extern base::global::CStrId g_unk_71726b7cb8;

struct UnkBehaviorAI_A2_b68 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;
};

struct UnkBehaviorActor_A2_b68 {
    u8 pad_0x0[0x240];
    UnkBehaviorAI_A2_b68* ai;
};

struct UnkBehavior_A2_b68 {
    u8 pad_0x0[0x18];
    UnkBehaviorActor_A2_b68* actor;
};

// Signature as src/game/behaviortree/CStopWatchAction/fn_7100392704.cpp declares it.
struct UnkSelf;

void fn_71003954fc(UnkSelf* behavior, void* record, f32 value) {
    UnkBehavior_A2_b68* self = reinterpret_cast<UnkBehavior_A2_b68*>(behavior);
    UnkPropertyName* name = static_cast<UnkPropertyName*>(record);
    if (UnkBehaviorActor_A2_b68* actor = self->actor) {
        if (UnkBehaviorAI_A2_b68* ai = actor->ai) {
            CBlackboard* blackboard = &ai->blackboard;
            if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
                if (!blackboard->field_0xe8) {
                    UnkTypedValue v;
                    v.type = fn_710000abbc();
                    v.value = &value;
                    fn_7100332c80(s, name, &v);
                }
            }
        }
    }
}

// The same for a bool (fn_7100395474).
void fn_7100395474(UnkSelf* behavior, void* record, bool value) {
    UnkBehavior_A2_b68* self = reinterpret_cast<UnkBehavior_A2_b68*>(behavior);
    UnkPropertyName* name = static_cast<UnkPropertyName*>(record);
    if (UnkBehaviorActor_A2_b68* actor = self->actor) {
        if (UnkBehaviorAI_A2_b68* ai = actor->ai) {
            CBlackboard* blackboard = &ai->blackboard;
            if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
                if (!blackboard->field_0xe8) {
                    UnkTypedValue v;
                    v.type = fn_710000d228();
                    v.value = &value;
                    fn_7100332c80(s, name, &v);
                }
            }
        }
    }
}
