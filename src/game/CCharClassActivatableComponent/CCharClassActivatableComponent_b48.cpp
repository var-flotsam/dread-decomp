// CActivatableComponent (unit of CCharClassActivatableComponent, tools/tu_map.py): fn_7100878680 stores
// the "Enabled" flag in the blackboard section of the current scenario (unless the blackboard is locked
// or fn_710024fb2c refuses; the property's name looked up in the key list at +0x58, count +0x60, values
// at +0x50, as in CEventScenarioComponent_b46.cpp; the setter as in CDoorLifeComponent.cpp), then sets
// +0x7c to 1 or 0 from it. fn_7100878570 is its reader: it marks the component loaded (+0x78) and, when
// the flag is found, switches the entity at +0x18 to it (fn_7100232864) and sets +0x7c to 1. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "game/CCharClassActivatableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

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

struct UnkActivatableValue_b48 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkEntityState;
void fn_7100232864(UnkEntityState* entity, bool value);

struct UnkActivatable_b48 {
    u8 pad_0x0[0x18];
    UnkEntityState* entity;
    u8 pad_0x20[0x30];
    UnkActivatableValue_b48* values;
    u64* keys;
    u32 count;
    u8 pad_0x64[0x14];
    bool loaded;
    u8 pad_0x79[3];
    f32 enabled;
};

void fn_7100878680(UnkActivatable_b48* self, bool enabled) {
    UnkGameManager* manager = g_unk_7101d47310;
    bool value = enabled;
    if (manager->field_0x968.field_0xe8 || !fn_710024fb2c(self))
        return;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717272a418.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkActivatableValue_b48* values = self->values;
    SetSectionBool(blackboard, section_id, &values[index].name, &value);
    self->enabled = value ? 1.0f : 0.0f;
}

void fn_7100878570(UnkActivatable_b48* self) {
    self->loaded = true;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    bool enabled = false;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717272a418.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkActivatableValue_b48* values = self->values;
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id)) {
        if (fn_710006d11c(section, reinterpret_cast<const CStrId*>(&values[index].name), &enabled)) {
            fn_7100232864(self->entity, enabled);
            self->enabled = 1.0f;
        }
    }
}
