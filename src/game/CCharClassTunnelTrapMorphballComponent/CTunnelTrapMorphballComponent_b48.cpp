// CTunnelTrapMorphballComponent (unit of CCharClassTunnelTrapMorphballComponent, tools/tu_map.py):
// fn_7100a43e7c and fn_7100a43f78 read one of the trap's flags each (g_unk_717274a620, g_unk_717274a648)
// from the blackboard section of the current scenario (fn_71010472e0 creates it when missing): the
// property's name is looked up in the key list at +0x88 (count +0x90, values at +0x80) as in
// CEventScenarioComponent_b46.cpp. False when it is not found. fn_7100a445c8 stores the first flag in the
// given blackboard unless it is locked (the setter as in CDoorLifeComponent.cpp). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);
extern UnkProperty g_unk_717274a620;
extern UnkProperty g_unk_717274a648;

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

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

struct UnkTunnelTrapValue_b48 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

struct UnkTunnelTrap_b48 {
    u8 pad_0x0[0x80];
    UnkTunnelTrapValue_b48* values;
    u64* keys;
    u32 count;

    // The flag `property` names, false when the section or the flag is missing.
    __attribute__((always_inline)) bool ReadFlag(UnkProperty& property) {
        bool value = false;
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard::CSection* section = fn_71010472e0(&manager->field_0x968, fn_71011a2fbc(manager->field_0x608));
        if (section) {
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
            fn_710006d11c(section, &values[index].id, &value);
        }
        return value;
    }
};

bool fn_7100a43e7c(UnkTunnelTrap_b48* self) {
    return self->ReadFlag(g_unk_717274a620);
}

bool fn_7100a43f78(UnkTunnelTrap_b48* self) {
    return self->ReadFlag(g_unk_717274a648);
}

void fn_7100a445c8(UnkTunnelTrap_b48* self, CBlackboard* blackboard, u32 value) {
    CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717274a620.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkTunnelTrapValue_b48* values = self->values;
    bool flag = value != 0;
    SetSectionBool(blackboard, section_id, &values[index].name, &flag);
}
