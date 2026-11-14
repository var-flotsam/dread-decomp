// CAIComponent unit (tools/tu_map.py unit 2702, medium confidence): fn_7101110d98 stores a float under a
// fixed property in a fixed section of the blackboard at +0x1260 of `owner`, unless the blackboard is
// locked (the setter as in src/game/CDoorLifeComponent.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkTUProperty.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

extern UnkPropertyName g_unk_7172a06338 __attribute__((visibility("hidden")));

struct UnkBlackboardOwner_b49 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;
};

// Stores the float at `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
static inline void SetSectionFloat(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, f32* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000abbc();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

void fn_7101110d98(f32 value, UnkBlackboardOwner_b49* owner) {
    f32 stored = value;
    SetSectionFloat(&owner->blackboard, g_unk_71726b7cb8, &g_unk_7172a06338, &stored);
}
