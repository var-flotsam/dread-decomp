// CEventPropComponent (unit of CCharClassEventPropComponent, tools/tu_map.py): three setters that mark one
// of the prop's flags (+0x78, +0x79 "EventLaunched", +0x7a "EventFinished") and store true under its
// property in the blackboard section of the current scenario unless the blackboard is locked (the name
// looked up in the key list at +0x58, count +0x60, values at +0x50, as in CEventScenarioComponent_b46.cpp;
// the setter as in CDoorLifeComponent.cpp). The first then calls vtable slot 0x170 when the owner (+0x30)
// is ready (+0x1b4) and, if it needs one (+0x1b0), has its object (+0x1a8). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CCharClassEventPropComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);
extern UnkProperty g_unk_717271ebc0;
extern UnkProperty g_unk_717271ebe8;  // "EventLaunched"
extern UnkProperty g_unk_717271ec10;  // "EventFinished"

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

struct UnkEventPropValue_b48 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkEventPropOwner_b48 {
    u8 pad_0x0[0x1a8];
    void* object;
    bool needs_object;
    u8 pad_0x1b1[3];
    bool ready;
};

struct UnkEventProp_b48 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkEventPropOwner_b48* owner;
    u8 pad_0x38[0x18];
    UnkEventPropValue_b48* values;
    u64* keys;
    u32 count;
    u8 pad_0x64[0x14];
    bool field_0x78;
    bool launched;
    bool finished;

    void vf_0x170() { reinterpret_cast<void (*)(UnkEventProp_b48*)>(vtable[0x170 / 8])(this); }

    // Stores true under `property` in the current scenario's section.
    __attribute__((always_inline)) void StoreTrue(CBlackboard* blackboard, const CStrId& section_id,
                                                  UnkProperty& property, bool* value) {
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
        UnkEventPropValue_b48* v = values;
        *value = true;
        SetSectionBool(blackboard, section_id, &v[index].name, value);
    }
};

void fn_71007dc328(UnkEventProp_b48* self) {
    self->field_0x78 = true;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    bool value;
    self->StoreTrue(blackboard, section_id, g_unk_717271ebc0, &value);
    UnkEventPropOwner_b48* owner = self->owner;
    if (owner && owner->ready && (!owner->needs_object || owner->object))
        self->vf_0x170();
}

void fn_71007dc478(UnkEventProp_b48* self) {
    self->launched = true;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    bool value;
    self->StoreTrue(blackboard, section_id, g_unk_717271ebe8, &value);
}

void fn_71007dc598(UnkEventProp_b48* self) {
    self->finished = true;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    bool value;
    self->StoreTrue(blackboard, section_id, g_unk_717271ec10, &value);
}
