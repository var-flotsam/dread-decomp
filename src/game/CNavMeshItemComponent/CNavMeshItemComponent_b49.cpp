// CNavMeshItemComponent unit (tools/tu_map.py): fn_71009c0fc8 stores a string id under the owner's
// name (+0x30 of +0x18) within a named definition (built on first use, as
// src/game/CMinimapManager/CMinimapManager_ReadFlag_b43.cpp's SubId) in the blackboard section of the
// current scenario, unless the blackboard is locked. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

// As src/game/UnkSubareaSetupState.cpp and src/game_s7/fn_7100fc4b34.cpp declare it.
struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
using UnkQualifiedName_7100fc4b34 = UnkQualifiedName;
const UnkSubId_7100fc4b34& fn_71003327a4(UnkQualifiedName_7100fc4b34* self, const CStrId* name);

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkNavMeshNamedDef_b49 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
extern UnkNavMeshNamedDef_b49 g_unk_717273e3e0;

static inline UnkSubId_7100fc4b34 SubId(UnkNavMeshNamedDef_b49& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}

struct UnkNavMeshItemOwner_b49 {
    u8 pad_0x0[0x30];
    CStrId name;
};

struct UnkNavMeshItem_b49 {
    u8 pad_0x0[0x18];
    UnkNavMeshItemOwner_b49* owner;
};

static __attribute__((always_inline)) inline void SetValue(CBlackboard* blackboard, const CStrId& section,
                                                    const UnkSubId_7100fc4b34& key, void* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_7100009658();
            v.value = value;
            fn_7100332c80(s, (UnkPropertyName*)&key.id, &v);
        }
    }
}

void fn_71009c0fc8(UnkNavMeshItem_b49* self, CStrId* value) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    SetValue(blackboard, fn_71011a2fbc(manager->field_0x608), SubId(g_unk_717273e3e0, self->owner->name), value);
}
