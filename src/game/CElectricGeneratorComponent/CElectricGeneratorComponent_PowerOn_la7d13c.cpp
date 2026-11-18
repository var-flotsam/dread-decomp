// CElectricGeneratorComponent unit (tools/tu_map.py unit 1587): for each of the generator's sub-area ids
// (+0x540) that the sub-area manager has a record of, switches on what the scenario lists under the record
// (kind 4: slot 0x168 with 1) and lights what it lists as kind 1 (fn_710079fc24); then stores true under the
// generator's property in the current scenario's blackboard section and calls the script named at +0x570
// with the owner. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2611.h"
#include "game/UnkTU365.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

struct UnkGeneratorRecord_ovb0120;
UnkGeneratorRecord_ovb0120* fn_7100a7cd10(base::global::CRntVector<UnkGeneratorRecord_ovb0120>* self, u32 index);

struct UnkScenarioKey_ovb0032;
struct UnkScenarioLayer_ovb0032;
void* fn_71011a3234(UnkScenarioLayer_ovb0032* layers, u32 index, UnkScenarioKey_ovb0032* const* key);

struct UnkSubArea;
// The record's id for the scenario's layer of the given kind (as src/game/CSubAreaManager.cpp declares it).

struct UnkSwitchable_la7d13c {
    void** vtable;
    void vf_0x168(bool on) {
        reinterpret_cast<void (*)(UnkSwitchable_la7d13c*, bool)>(vtable[0x168 / 8])(this, on);
    }
};
struct UnkLight_la7d13c;
UnkSwitchable_la7d13c* fn_7100a7cee8(void* object);
UnkLight_la7d13c* fn_7100a7cfc4(void* object);
void fn_710079fc24(UnkLight_la7d13c* light, bool flag, f32 a, f32 b);

// A record as this function reads it: its sub-area id first.
struct UnkGeneratorRecordView_la7d13c {
    CStrId id;
};

struct UnkRecordArray_la7d13c {
    UnkGeneratorRecord_ovb0120* data;
    u32 count;
};

struct UnkSubAreaManager_la7d13c {
    u8 pad_0x0[0x860];
    UnkRecordArray_la7d13c records;  // 0x860
};

// What a scenario layer holds under a key: 0x40-byte parts.
struct UnkPart_la7d13c {
    u8 bytes[0x40];
};
struct UnkPartList_la7d13c {
    UnkPart_la7d13c* data;
    u32 count;
};
struct UnkPartTarget_la7d13c {
    u8 pad_0x0[0x10];
    void* object;
};

struct UnkGeneratorValue_la7d13c {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkElectricGenerator_la7d13c {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x4d0 - 0x38];
    UnkGeneratorValue_la7d13c* values;  // 0x4d0
    u64* keys;                          // 0x4d8
    u32 count;                          // 0x4e0
    u8 pad_0x4e4[0x540 - 0x4e4];
    base::global::CRntVector<base::UnkStrIdMember> ids;  // 0x540
    u8 pad_0x568[0x8];
    CStrId script;  // 0x570
};
static_assert(__builtin_offsetof(UnkElectricGenerator_la7d13c, script) == 0x570, "layout");

// The sub-area manager, created on first use.
static inline UnkSubAreaManager_la7d13c* SubAreaManager_la7d13c() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return static_cast<UnkSubAreaManager_la7d13c*>(manager);
}

static inline UnkPartList_la7d13c* Parts_la7d13c(UnkGeneratorRecord_ovb0120* record, u32 layer, u32 kind) {
    UnkScenarioLayer_ovb0032* layers = reinterpret_cast<UnkScenarioLayer_ovb0032*>(g_unk_7101d47310->field_0x608);
    const CStrId* key = fn_7100fac818(reinterpret_cast<UnkSubArea*>(record), kind);
    return static_cast<UnkPartList_la7d13c*>(
        fn_71011a3234(layers, layer, reinterpret_cast<UnkScenarioKey_ovb0032* const*>(key)));
}

// Stores the bool at `value` under `name` in `section` (unless the blackboard is locked).
static inline void SetBool_la7d13c(CBlackboard* blackboard, CBlackboard::CSection* section, UnkPropertyName* name,
                                   bool* value) {
    if (!blackboard->field_0xe8) {
        UnkTypedValue v;
        v.type = fn_710000d228();
        v.value = value;
        fn_7100332c80(section, name, &v);
    }
}

void fn_7100a7d13c(UnkElectricGenerator_la7d13c* self) {
    UnkSubAreaManager_la7d13c* manager = SubAreaManager_la7d13c();
    auto* records = reinterpret_cast<base::global::CRntVector<UnkGeneratorRecord_ovb0120>*>(&manager->records);
    base::global::CRntVector<base::UnkStrIdMember>* ids = &self->ids;
    for (u32 i = 0; i < self->ids.count; ++i) {
        for (u32 j = 0; j < manager->records.count; ++j) {
            UnkGeneratorRecord_ovb0120* record = fn_7100a7cd10(records, j);
            CStrId id = reinterpret_cast<UnkGeneratorRecordView_la7d13c*>(record)->id;
            if (fn_71000ce740(ids, i)->data != id.data)
                continue;
            UnkPartList_la7d13c* switches = Parts_la7d13c(record, 0, 4);
            for (u64 k = 0; k < switches->count; ++k) {
                UnkBreakableTarget* target = *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(&switches->data[k]));
                if (!target)
                    continue;
                void* object = reinterpret_cast<UnkPartTarget_la7d13c*>(target)->object;
                if (!object)
                    continue;
                if (UnkSwitchable_la7d13c* switchable = fn_7100a7cee8(object))
                    switchable->vf_0x168(true);
            }
            UnkPartList_la7d13c* lights = Parts_la7d13c(record, 2, 1);
            for (u64 k = 0; k < lights->count; ++k) {
                UnkBreakableTarget* target = *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(&lights->data[k]));
                void* object = target ? reinterpret_cast<UnkPartTarget_la7d13c*>(target)->object : nullptr;
                if (UnkLight_la7d13c* light = fn_7100a7cfc4(object))
                    fn_710079fc24(light, false, 0.3f, 1.0f);
            }
            break;
        }
    }

    UnkGameManager* game = g_unk_7101d47310;
    if (game->field_0x608) {
        CBlackboard* blackboard = &game->field_0x968;
        CBlackboard::CSection* section = fn_71010472e0(blackboard, fn_71011a2fbc(game->field_0x608));
        if (section) {
            u64 index = 0;
            u32 n = self->count;
            if (n) {
                u64* keys = self->keys;
                u64 key = (u64)reinterpret_cast<const UnkProperty*>(g_unk_7172753148)->name.data;
                for (; index != n; ++index) {
                    if (keys[index] == key)
                        break;
                }
            }
            UnkGeneratorValue_la7d13c* values = self->values;
            bool on = true;
            SetBool_la7d13c(blackboard, section, &values[index].name, &on);
        }
    }

    if (self->script.data != &g_unk_7101d47108->empty)
        fn_7100000790(self->script.Text(), "o", self->owner);
}
