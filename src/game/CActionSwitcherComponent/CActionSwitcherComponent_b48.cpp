// CActionSwitcherComponent unit (tools/tu_map.py): fn_7100875d98 reads the switcher's flag (the shape
// of CEventScenarioComponent_b46.cpp's fn_71007e15d0: the property's name looked up in the key list at
// +0x58, count +0x60, values at +0x50; the blackboard section of the current scenario) into +0x88. When
// it was read and is set, the switcher is turned on (fn_7100875ed0) and, if its definition (+0x80) says
// so at +0x69, the owner's (+0x30) object gets the id 0xb0c1699f8b57b14b (fn_71009c0f1c). Names and
// layouts are placeholders. fn_710087616c and fn_71008762d8 follow a progress value: when the definition
// asks for it (+0x68) the owner's animation (+0x220 of +0x30) gets it (fn_710117ad18); reaching 1 turns the
// switcher on, falling below 1 turns it off, and the new state is stored in the section (unless the
// blackboard is locked; the setter as in CDoorLifeComponent.cpp).
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/CCharClassActivatableComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU372.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

struct UnkWeakTarget;

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

struct UnkActionSwitcherValue_b48 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

struct UnkActionSwitcherAnimation_b48;

struct UnkActionSwitcherOwner_b48 {
    u8 pad_0x0[0x220];
    UnkActionSwitcherAnimation_b48* animation;
};

struct UnkActionSwitcherDef_b48 {
    u8 pad_0x0[0x68];
    bool animate;
    bool notify;
};

struct UnkActionSwitcher_b48 {
    u8 pad_0x0[0x30];
    union {
        UnkWeakTarget* owner;
        UnkActionSwitcherOwner_b48* owner_actor;
    };
    u8 pad_0x38[0x18];
    UnkActionSwitcherValue_b48* values;
    u64* keys;
    u32 count;
    u8 pad_0x64[0x1c];
    UnkActionSwitcherDef_b48* def;
    bool on;

    // Stores `on` under the switcher's property in the current scenario's section.
    __attribute__((always_inline)) void StoreOn(bool* value, bool on) {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        u64 index = 0;
        u32 n = count;
        if (n) {
            u64* k = keys;
            u64 key = (u64)g_unk_717272a3f0.name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        UnkActionSwitcherValue_b48* v = values;
        *value = on;
        SetSectionBool(blackboard, section_id, &v[index].name, value);
    }
};

void fn_7100875ed0(UnkActionSwitcher_b48* self, bool on, bool notify);

void fn_7100875d98(UnkActionSwitcher_b48* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717272a3f0.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkActionSwitcherValue_b48* values = self->values;
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id)) {
        bool* on = &self->on;
        if (fn_710006d11c(section, &values[index].id, on) && *on) {
            fn_7100875ed0(self, true, true);
            if (self->def->notify) {
                if (void* object = fn_71002b3b84(self->owner))
                    fn_71009c0f1c(object, 0xb0c1699f8b57b14b);
            }
        }
    }
}

void fn_710087616c(UnkActionSwitcher_b48* self, f32 progress) {
    if (self->def->animate)
        fn_710117ad18(self->owner_actor->animation, false, progress);
    if (!self->on && progress == 1.0f) {
        fn_7100875ed0(self, true, false);
        bool value;
        self->StoreOn(&value, true);
    }
}

void fn_71008762d8(UnkActionSwitcher_b48* self, f32 progress) {
    if (self->def->animate)
        fn_710117ad18(self->owner_actor->animation, false, progress);
    if (self->on && progress < 1.0f) {
        fn_7100875ed0(self, false, false);
        bool value;
        self->StoreOn(&value, false);
    }
}
