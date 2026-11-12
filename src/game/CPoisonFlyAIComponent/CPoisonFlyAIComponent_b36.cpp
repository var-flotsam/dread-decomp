// Functions in the CPoisonFlyAIComponent unit (tools/tu_map.py). Names and layouts are
// placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CPoisonFlyAIComponent.h"
#include "game/UnkTUProperty.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

// A typed value as the section setter takes it (as in src/game/CXParasiteAIComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkPoisonFlyAI {
    u8 pad_0x0[0x121e];
    bool field_0x121e;
    u8 pad_0x121f[0x41];
    CBlackboard field_0x1260;
};

void fn_7101108108(UnkPoisonFlyAI* self);  // the base method

// Stores a copy of `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
static inline void SetSectionBool(CBlackboard* blackboard, const base::global::CStrId& section,
                                  UnkPropertyName* name, bool value) {
    UnkTypedValue v;
    bool copy = value;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            v.type = fn_710000d228();
            v.value = &copy;
            fn_7100332c80(s, name, &v);
        }
    }
}

// After the base method, stores the flag at +0x121e in the blackboard section.
void fn_71006395fc(UnkPoisonFlyAI* self) {
    fn_7101108108(self);
    SetSectionBool(&self->field_0x1260, g_unk_71726b7cb8, &g_unk_71726fa598, self->field_0x121e);
}
