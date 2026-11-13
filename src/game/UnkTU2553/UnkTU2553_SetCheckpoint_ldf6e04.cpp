// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): stores three strings (a level id, a
// scenario id and a start point) in the section of the game manager's blackboard that the id table names at
// +0x1bd0, unless the blackboard is locked. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

inline const CStrId& SectionId_ldf6e04() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return *reinterpret_cast<const CStrId*>(s_table.pad_0x0 + 0x1bd0);
}

static inline const UnkQualifiedId& DefId_ldf6e04(UnkPropertyEx<>& def) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), g_unk_7101d470e0);
    return qid;
}

static inline void SetString_ldf6e04(CBlackboard* blackboard, const CStrId& section, UnkPropertyEx<>& def,
                                     void* value) {
    const UnkQualifiedId& name = DefId_ldf6e04(def);
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_7100007f84();
            v.value = value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

void fn_7100df6e04(UnkGameManager* self, void* level, void* scenario, void* start) {
    CStrId section = SectionId_ldf6e04();
    CBlackboard* blackboard = &self->field_0x968;
    SetString_ldf6e04(blackboard, section, g_unk_71727599d0, level);
    SetString_ldf6e04(blackboard, section, g_unk_7172759a10, scenario);
    SetString_ldf6e04(blackboard, section, g_unk_7172759a50, start);
}
