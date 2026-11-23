// CAIComponent unit (tools/tu_map.py unit 2702): clears four flags, named by ids of the reflection id
// table, in the blackboard section g_unk_71726b7cb8 of the blackboard at +0x1260, unless the blackboard
// is locked. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CStrId;

struct UnkBlackboardOwner_l0f9fe4 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;
};

// Stores `value` under `name` in the blackboard section `section` names, unless the blackboard is locked.
static inline void SetFlag_l0f9fe4(CBlackboard* blackboard, const CStrId& section, const CStrId& name,
                                   bool value) {
    UnkTypedValue v;
    bool stored = value;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            v.type = fn_710000d228();
            v.value = &stored;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<CStrId*>(&name)), &v);
        }
    }
}

static inline const CStrId& Id_l0f9fe4(u32 index) {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[index];
}

void fn_71010f9fe4(void* owner) {
    auto* self = reinterpret_cast<UnkBlackboardOwner_l0f9fe4*>(owner);
    CBlackboard* blackboard = &self->blackboard;
    SetFlag_l0f9fe4(blackboard, g_unk_71726b7cb8, Id_l0f9fe4(0x11c), false);
    SetFlag_l0f9fe4(blackboard, g_unk_71726b7cb8, Id_l0f9fe4(0x117), false);
    SetFlag_l0f9fe4(blackboard, g_unk_71726b7cb8, Id_l0f9fe4(0x11d), false);
    SetFlag_l0f9fe4(blackboard, g_unk_71726b7cb8, Id_l0f9fe4(0x11e), false);
}
