// GUI::CMissionLog's unit (tools/tu_map.py): stores the log's two record arrays ("EventsList" and "TutosList")
// in the game manager's two blackboards (each unless it is locked; the second is skipped altogether while
// locked). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/GUI/CMissionLog.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkMissionLog.h"
#include "game/UnkTU1645.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

// The "EventsList" and "TutosList" properties and their collection type (src/game_s7/fn_7100ae39e0.cpp).

// The game manager's two blackboards.
struct UnkMissionLogGameManager_b59690 {
    u8 pad_0x0[0x820];
    CBlackboard field_0x820;
    u8 pad_0x910[0x58];
    CBlackboard field_0x968;
};
static_assert(__builtin_offsetof(UnkMissionLogGameManager_b59690, field_0x968) == 0x968, "layout");

static inline const UnkQualifiedId& DefId_b59690(UnkPropertyEx<>& def) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), base::global::CStrId::Empty());
    return qid;
}

// Stores the record array `value` under `name` in the section `section`, unless the blackboard is locked.
static inline void SetRecords_b59690(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                                     void* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = reinterpret_cast<const base::reflection::CType*>(&g_unk_7172764a30);
            v.value = value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

bool fn_7100b59690(UnkMissionLog* self, bool flag) {
    auto* manager = reinterpret_cast<UnkMissionLogGameManager_b59690*>(g_unk_7101d47310);
    CBlackboard* blackboard = &manager->field_0x820;
    SetRecords_b59690(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x37c],
                      DefId_b59690(g_unk_717275ace0), self->entries_0x248);
    SetRecords_b59690(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x37c],
                      DefId_b59690(g_unk_717275ad20), self->entries_0x270);
    manager = reinterpret_cast<UnkMissionLogGameManager_b59690*>(g_unk_7101d47310);
    if (!manager->field_0x968.field_0xe8) {
        blackboard = &manager->field_0x968;
        SetRecords_b59690(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x37c],
                          DefId_b59690(g_unk_717275ace0), self->entries_0x248);
        SetRecords_b59690(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x37c],
                          DefId_b59690(g_unk_717275ad20), self->entries_0x270);
    }
    return false;
}
