// CEmmyCaveAIComponent (tools/tu_map.py): a reader and a writer of one flag in the current scenario's
// blackboard section (only with a scenario). The reader runs the base routine, reads the flag under
// g_unk_71726db748 (key list +0x2a718, count +0x2a720, values +0x2a710, looked up as in
// CEventScenarioComponent_b46.cpp) and marks +0x29eb8 when it is set; the writer runs its base routine
// and stores false under it unless the blackboard is locked. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);
extern UnkProperty g_unk_71726db748;

struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkEmmyCaveValue_b49 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

struct UnkEmmyCaveAI_b49 {
    u8 pad_0x0[0x29eb8];
    bool field_0x29eb8;
    u8 pad_0x29eb9[0x2a710 - 0x29eb9];
    UnkEmmyCaveValue_b49* values;
    u64* keys;
    u32 count;

    __attribute__((always_inline)) UnkEmmyCaveValue_b49* Find(const UnkProperty& property) {
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

void fn_7100540430(UnkEmmyCaveAI_b49* self);
void fn_7100557ae4(UnkEmmyCaveAI_b49* self);

void fn_71004f0814(UnkEmmyCaveAI_b49* self) {
    fn_7100540430(self);
    UnkGameManager* manager = g_unk_7101d47310;
    if (!manager->field_0x608)
        return;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
        bool value = true;
        fn_710006d11c(section, &self->Find(g_unk_71726db748)->id, &value);
        if (value)
            self->field_0x29eb8 = true;
    }
}

void fn_71004f0924(UnkEmmyCaveAI_b49* self) {
    fn_7100557ae4(self);
    UnkGameManager* manager = g_unk_7101d47310;
    if (!manager->field_0x608)
        return;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
        UnkEmmyCaveValue_b49* name = self->Find(g_unk_71726db748);
        UnkTypedValue v;
        bool value = false;
        if (!blackboard->field_0xe8) {
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(section, &name->name, &v);
        }
    }
}
