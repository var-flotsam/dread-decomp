// Unit 2553 (CGameManager::CTunableGameManager): the game progress in percent: the progress count the
// blackboard holds over the maximum the script's Init.GetMaxGameProgressStats returns. Names and layouts
// are placeholders.
#include <algorithm>

#include "../../game_s7/UnkIter.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

// Calls the script function `name`, collecting what it returns in `results`.
bool fn_7100dfb758(UnkPropBag* results, const char* name, const char* fmt, ...);

// A value a script call returned: where it is in the bag's stream, the hash of its name and its type.
struct UnkScriptValue_ldfb930 {
    u64 offset;
    u64 key;
    s32 type;  // 'i' for an integer
};

struct UnkIdHash_ldfb930 {
    u8 pad_0x0[0x30];
    u64 hash;
};

static inline UnkScriptValue_ldfb930* Values_ldfb930(UnkPropBag& results) {
    return reinterpret_cast<UnkScriptValue_ldfb930*>(results.values.data);
}

// The value named `id`, or the end of the values when there is none.
static inline UnkScriptValue_ldfb930* FindValue_ldfb930(UnkPropBag& results, const CStrId& id) {
    UnkScriptValue_ldfb930* values = Values_ldfb930(results);
    u64 key = reinterpret_cast<UnkIdHash_ldfb930*>(id.data)->hash;
    UnkIter<UnkScriptValue_ldfb930> first{values};
    UnkIter<UnkScriptValue_ldfb930> last{&values[results.values.count]};
    return std::find_if(first, last, [key](const UnkScriptValue_ldfb930& v) { return v.key == key; }).p;
}

static inline const UnkQualifiedId& DefId_ldfb930(UnkPropertyEx<>& def) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), base::global::CStrId::Empty());
    return qid;
}

s32 fn_7100dfb930() {
    UnkGameManager* manager = g_unk_7101d47310;
    f32 ratio = 0.0f;
    UnkPropBag results;
    if (fn_7100dfb758(&results, "Init.GetMaxGameProgressStats", g_unk_710162e2ed)) {
        const UnkQualifiedId& name = DefId_ldfb930(g_unk_71727590f8);
        s32 total = 0;
        UnkScriptValue_ldfb930* it = FindValue_ldfb930(results, name.id);
        if (it != &Values_ldfb930(results)[results.values.count] && it->type == 'i') {
            CRntFile_Seek(&results.lock, it->offset, 0);
            fn_710007c70c(&results.lock, &total, 4);
        }
        s32 progress = 0;
        if (total > 0) {
            CBlackboard* blackboard = &manager->field_0x968;
            const CStrId& section = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x229];
            if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
                fn_7100395a30(s, &name, &progress);
            if (total > 0)
                ratio = (f32)progress / (f32)total;
        }
    }
    return (s32)(ratio * 100.0f);
}
