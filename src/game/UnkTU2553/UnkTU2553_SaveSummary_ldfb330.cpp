// Unit 2553 (CGameManager::CTunableGameManager): the save slot's summary line: the localized text of
// "#GUI_MISC003", the play time as hh:mm:ss and the game progress in percent that the script's
// Scenario.GetGameProgressPctInTable returns. Names and layouts are placeholders.
#include <stdio.h>

#include <algorithm>

#include "../../game_aca57c/UnkBufferString.h"
#include "../../game_s7/UnkIter.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiLabel.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2733.h"
#include "game/UnkTU500.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

// Calls the script function `name`, collecting what it returns in `results`.
bool CallScriptFunctionByName(UnkPropBag* results, const char* name, const char* fmt, ...);

// The text the language gives (as src/game/CScenario.cpp declares it).
struct UnkDialogText : UnkGuiText {
    ~UnkDialogText() { fn_7100066e44(this); }
    const u32* Chars() const { return *reinterpret_cast<const u32* const*>(unk_0x0 + 8); }
};

// A value a script call returned: where it is in the bag's stream, the hash of its name and its type.
struct UnkScriptValue_ldfb330 {
    u64 offset;
    u64 key;
    s32 type;  // 'i' for an integer, 'f' for a float
};

struct UnkIdHash_ldfb330 {
    u8 pad_0x0[0x30];
    u64 hash;
};

static inline UnkScriptValue_ldfb330* Values_ldfb330(UnkPropBag& results) {
    return reinterpret_cast<UnkScriptValue_ldfb330*>(results.values.data);
}

// The value named `id`, or the end of the values when there is none.
static inline UnkScriptValue_ldfb330* FindValue_ldfb330(UnkPropBag& results, const CStrId& id) {
    UnkScriptValue_ldfb330* values = Values_ldfb330(results);
    u64 key = reinterpret_cast<UnkIdHash_ldfb330*>(id.data)->hash;
    UnkIter<UnkScriptValue_ldfb330> first{values};
    UnkIter<UnkScriptValue_ldfb330> last{&values[results.values.count]};
    return std::find_if(first, last, [key](const UnkScriptValue_ldfb330& v) { return v.key == key; }).p;
}

static inline const UnkQualifiedId& DefId_ldfb330(UnkPropertyEx<>& def) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), base::global::CStrId::Empty());
    return qid;
}

// Reads the float `def` names in the blackboard section `section` into `out`, when the section exists.
static inline void ReadFloat_ldfb330(CBlackboard* blackboard, const CStrId& section, UnkPropertyEx<>& def,
                                     f32* out) {
    const UnkQualifiedId& name = DefId_ldfb330(def);
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        fn_7100395b34(s, &name, out);
}

static inline UnkLanguage* Language_ldfb330() {
    UnkLanguage* language = g_unk_7101d47358;
    if (!language) {
        language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
        fn_710119c020(language);
        g_unk_7101d47358 = language;
    }
    return language;
}

CRntString fn_7100dfb330(UnkGameManager* self) {
    f32 play_time = 0.0f;
    ReadFloat_ldfb330(&self->field_0x968, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x229],
                      g_unk_7172758cf0, &play_time);

    f32 progress = 0.0f;
    UnkPropBag results;
    if (CallScriptFunctionByName(&results, "Scenario.GetGameProgressPctInTable", g_unk_710162e2ed)) {
        CStrId id("GameProgressPct", true);
        UnkScriptValue_ldfb330* it = FindValue_ldfb330(results, id);
        if (it != &Values_ldfb330(results)[results.values.count]) {
            switch (it->type) {
            case 'f':
                CRntFile_Seek(&results.lock, it->offset, 0);
                fn_710007c70c(&results.lock, &progress, 4);
                break;
            case 'i': {
                s32 value;
                CRntFile_Seek(&results.lock, it->offset, 0);
                fn_710007c70c(&results.lock, &value, 4);
                progress = value;
                break;
            }
            }
        }
    }

    UnkBufferString<0x80, g_unk_7101a1b750> time;
    char buffer[0x200];
    u32 seconds = static_cast<s32>(play_time);
    u32 secs = seconds % 60;
    u32 hours = seconds / 3600;
    u32 minutes = seconds / 60 % 60;
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, secs);
    CRntString_AssignCString(time.Str(), buffer);
    {
        static CStrId s_label("#GUI_MISC003", true);
        UnkDialogText text = fn_710119d040(Language_ldfb330(), s_label);
        const u32* chars = text.Chars();
        sprintf(buffer, "%ls / %s / %3.0f%% ", chars ? chars : g_unk_7101648434,
                time.Str()->data ? time.Str()->data : g_unk_710162e2ed, progress);
    }
    return CRntString(buffer);
}
