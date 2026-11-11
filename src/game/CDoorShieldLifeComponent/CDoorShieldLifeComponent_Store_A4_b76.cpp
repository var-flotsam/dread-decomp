// CDoorShieldLifeComponent's unit (tools/tu_map.py), fn_71008db4e4: stores the door's "ChangeWhenVisible"
// (g_unk_7172731138, bool at +0x408) and "NextState" (g_unk_7172731160, the CDoorLifeComponent::SState enum
// g_unk_7172731200 at +0x40c) in the current scenario's blackboard section (unless it is locked). Keys at
// +0x678 (count +0x680, values at +0x670), looked up as in src/game/CDoorLifeComponent/CDoorLifeComponent_b49.cpp.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CDoorShieldLifeComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);
extern UnkProperty g_unk_7172731138;  // "ChangeWhenVisible"
// The "NextState" property and the CDoorLifeComponent::SState enum (src/game_s7/fn_71008da6c0.cpp).
struct UnkEnumType;
extern UnkProperty g_unk_7172731160;
extern UnkEnumType g_unk_7172731200;

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

static inline void SetSectionValue(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name,
                                   const base::reflection::CType* type, void* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = type;
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

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

struct UnkDoorShieldValue_A4_b76 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

struct UnkDoorShieldStore_A4_b76 {
    u8 pad_0x0[0x408];
    bool field_0x408;
    u8 pad_0x409[3];
    s32 field_0x40c;
    u8 pad_0x410[0x670 - 0x410];
    UnkDoorShieldValue_A4_b76* values;
    u64* keys;
    u32 count;

    __attribute__((always_inline)) UnkDoorShieldValue_A4_b76* Find(const UnkProperty& property) {
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

void fn_71008db4e4(UnkDoorShieldStore_A4_b76* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    {
        bool* value = &self->field_0x408;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        UnkDoorShieldValue_A4_b76* name = self->Find(g_unk_7172731138);
        SetSectionBool(blackboard, section_id, &name->name, value);
    }
    s32* state = &self->field_0x40c;
    CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    UnkDoorShieldValue_A4_b76* name = self->Find(g_unk_7172731160);
    SetSectionValue(blackboard, section_id, &name->name,
                    reinterpret_cast<const base::reflection::CType*>(&g_unk_7172731200), state);
}
