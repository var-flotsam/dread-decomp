// CDoorLifeComponent unit (tools/tu_map.py): fn_71008d30e4 enables or disables the door in the current
// scenario's blackboard section (unless it is locked): "Disabled" (g_unk_71727310e8) gets the opposite of
// `enabled`; when enabling a door whose section already has "StayOpened" (g_unk_7172731110), that flag is
// rewritten too, from the door's state (+0x128 of +0x9f8, inverted) when +0x418 is set, else false. Keys at
// +0x678 (count +0x680, values at +0x670), looked up as in CEventScenarioComponent_b46.cpp; fn_71010474f4
// is src/game/CBlackboard.cpp's. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1376.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

bool fn_71010474f4(CBlackboard* self, const CStrId& section, const CStrId& name);

void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

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

struct UnkDoorEnableValue_b49 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

struct UnkDoorEnableState_b49 {
    u8 pad_0x0[0x128];
    bool field_0x128;
};

struct UnkDoorEnable_b49 {
    u8 pad_0x0[0x418];
    bool field_0x418;
    u8 pad_0x419[0x670 - 0x419];
    UnkDoorEnableValue_b49* values;
    u64* keys;
    u32 count;
    u8 pad_0x684[0x9f8 - 0x684];
    UnkDoorEnableState_b49* state;

    __attribute__((always_inline)) UnkDoorEnableValue_b49* Find(const UnkProperty& property) {
        u64 index = 0;
        u32 n = count;
        if (n) {
            u64* k = keys;
            u64 key = (u64)property.name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        return &values[index];
    }
};

void fn_71008d30e4(UnkDoorEnable_b49* self, bool enabled) {
    UnkGameManager* manager = g_unk_7101d47310;
    if (manager->field_0x968.field_0xe8)
        return;
    CBlackboard* blackboard = &manager->field_0x968;
    bool value;
    {
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        UnkDoorEnableValue_b49* name = self->Find(g_unk_71727310e8);
        value = !enabled;
        SetSectionBool(blackboard, section_id, &name->name, &value);
    }
    if (!enabled)
        return;
    bool known;
    {
        CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
        known = fn_71010474f4(blackboard, section_id, self->Find(g_unk_7172731110)->id);
    }
    if (!known)
        return;
    if (self->field_0x418)
        value = !self->state->field_0x128;
    CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    UnkDoorEnableValue_b49* name = self->Find(g_unk_7172731110);
    SetSectionBool(blackboard, section_id, &name->name, &value);
}
