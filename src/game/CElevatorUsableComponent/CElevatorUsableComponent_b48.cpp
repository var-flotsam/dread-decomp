// CElevatorUsableComponent unit (tools/tu_map.py): fn_7100a82588 stores true under the usable's property
// (g_unk_7172753148) in the blackboard section of the current scenario unless the blackboard is locked
// (the name looked up in the key list at +0x4d8, count +0x4e0, values at +0x4d0, as in
// CEventScenarioComponent_b46.cpp; the setter as in CDoorLifeComponent.cpp), hands the game manager
// (fn_7100dfaf88) the name at +0x10 of the id fn_7100de9bf8 gives (counting its use, +0x44) with the tag
// "[MANTAIN]", then runs fn_7100aa30bc. CTrainUsableComponent_b48.cpp has the same code (fn_7100a9a1f8). Names and
// layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CRntString;
using base::global::CStrId;

// Stores the bool at `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
static inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

// The id fn_7100de9bf8 gives, as this caller sees its data.
struct UnkUsableName_b48 {
    u8 pad_0x0[0x10];
    u8 name[0x34];
    union {
        u32 uses;
    };
};

struct UnkUsableValue_b48 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkUsable_b48 {
    u8 pad_0x0[0x4d0];
    UnkUsableValue_b48* values;
    u64* keys;
    u32 count;
};

// The name of the id, counting the use.
static inline void* UseName(const CStrId* id) {
    ++reinterpret_cast<UnkUsableName_b48*>(id->data)->uses;
    return reinterpret_cast<UnkUsableName_b48*>(id->data)->name;
}

void fn_7100a82588(UnkUsable_b48* self, u32 arg) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    bool value;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)reinterpret_cast<UnkProperty*>(g_unk_7172753148)->name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkUsableValue_b48* values = self->values;
    value = true;
    SetSectionBool(blackboard, section_id, &values[index].name, &value);
    UnkGameManager* gm = g_unk_7101d47310;
    fn_7100dfaf88(gm, UseName(fn_7100de9bf8(gm)), CRntString("[MANTAIN]"), true);
    fn_7100aa30bc(self, arg);
}
