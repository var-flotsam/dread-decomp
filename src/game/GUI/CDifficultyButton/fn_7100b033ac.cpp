// GUI::CDifficultyButton's unit (tools/tu_map.py unit 1666): pressing the button plays the press sound, marks
// the main menu's slot for the current difficulty (by which of the three difficulty names the game manager's
// profile holds), drops the GUI key, plays a fade, sets the button's flag and stores a fresh UUID string in
// the game manager's second blackboard. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerDef.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU57.h"
#include "game/UnkTypedValue.h"
#include "game/Uuid.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using unk_static_init::UnkEnumType;

// nn::util's UUID and formatter (as src/game/UnkPlayReport.cpp declares them).
namespace nn::util {
Uuid GenerateUuid();
s32 TSNPrintf(char* buffer, u64 size, const char* format, ...);
}  // namespace nn::util

struct UnkVec {
    u64 xy;
    u32 z;
};

extern CStrId g_unk_717275dd18 __attribute__((visibility("hidden")));

// The name of difficulty `index` (0 to 2).

// The game manager's view with the current profile's difficulty name.
struct UnkDifficultyProfile_lb033ac {
    u8 pad_0x0[0x7f0];
    CRntString difficulty;  // 0x7f0
};

// A main menu slot.
struct UnkMenuSlot_lb033ac {
    bool marked;
    u8 pad_0x1[0x4f];
};

struct UnkMainMenu_lb033ac {
    u8 pad_0x0[0xb0];
    UnkMenuSlot_lb033ac slots[1];  // 0xb0
};

struct UnkDifficultyButton {
    u8 pad_0x0[0x2a8];
    bool field_0x2a8;
};

static inline const UnkQualifiedId& Qid(UnkGameManagerDef& def) {
    if (def.qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&def.qid, &def, g_unk_7101d470e0);
    return def.qid;
}
static inline const UnkQualifiedId& DefId(UnkPropertyEx<>& def) {
    return Qid(reinterpret_cast<UnkGameManagerDef&>(def));
}
static inline const CStrId& TableId(u32 offset) {
    return *reinterpret_cast<CStrId*>(GameManagerIdTable().pad_0x0 + offset);
}
template <class T>
static inline void SetValue(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                            const base::reflection::CType* (*type)(), const T& value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = type();
            v.value = const_cast<T*>(&value);
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

static inline bool IsDifficulty(const CRntString& current, u32 index) {
    const CRntString& name = fn_7100df703c(index);
    return fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&current),
                         reinterpret_cast<const UnkCompared_0x30*>(&name));
}

// Plays the effect player's fade at the origin.
static inline void PlayFade(void* effects) {
    UnkVec origin;
    origin.xy = 0;
    origin.z = 0;
    fn_7101053850(effects, &origin, 1, 2.0f, 1.0f);
}

void fn_7100b033ac(UnkDifficultyButton* self) {
    fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f01d0), self);
    const CRntString& current = reinterpret_cast<UnkDifficultyProfile_lb033ac*>(g_unk_7101d47310)->difficulty;
    u64 slot;
    if (IsDifficulty(current, 0))
        slot = 1;
    else if (IsDifficulty(current, 1))
        slot = 5;
    else
        slot = IsDifficulty(current, 2) ? 9 : 1;
    if (auto* menu = static_cast<UnkMainMenu_lb033ac*>(fn_7100deab70(g_unk_7101d47310, CRntString("MAINMENU"))))
        menu->slots[slot].marked = true;
    fn_710002fd10(&g_unk_717271e8f8->field_0xca8, &g_unk_717275dd18);
    PlayFade(g_unk_7101d47310->field_0x1040);
    self->field_0x2a8 = true;

    CBlackboard* blackboard = &g_unk_7101d47310->field_0x820;
    nn::util::Uuid uuid = nn::util::GenerateUuid();
    char id[0x40];
    nn::util::TSNPrintf(id, sizeof(id), "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                        uuid.data[0], uuid.data[1], uuid.data[2], uuid.data[3], uuid.data[4], uuid.data[5],
                        uuid.data[6], uuid.data[7], uuid.data[8], uuid.data[9], uuid.data[10], uuid.data[11],
                        uuid.data[12], uuid.data[13], uuid.data[14], uuid.data[15]);
    SetValue(blackboard, TableId(0x1148), DefId(g_unk_71727594b8), fn_7100007f84, CRntString(id));
}
