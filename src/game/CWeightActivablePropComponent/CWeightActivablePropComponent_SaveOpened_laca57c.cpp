// CWeightActivablePropComponent's unit: stores whether the prop is in state 2 under "Opened"
// (g_unk_7172755b08) in the blackboard section of the current scenario, unless the blackboard is locked.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CWeightActivablePropComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

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

struct UnkWeightValue_laca57c {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkWeightState_laca57c {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkWeightSave_laca57c {
    u8 pad_0x0[0x140];
    UnkWeightState_laca57c* field_0x140;
    u8 pad_0x148[0x210 - 0x148];
    UnkWeightValue_laca57c* values;
    u64* keys;
    u32 count;
};

template <u32 Offset>
struct UnkSmOwner_b61;

void fn_7100aca57c(UnkSmOwner_b61<0x130>* owner) {
    UnkWeightSave_laca57c* self = reinterpret_cast<UnkWeightSave_laca57c*>(owner);
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_7100de9c30(manager);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_7172755b08.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkWeightValue_laca57c* values = self->values;
    bool opened = self->field_0x140 && self->field_0x140->state == 2;
    SetSectionBool(blackboard, section_id, &values[index].name, &opened);
}
