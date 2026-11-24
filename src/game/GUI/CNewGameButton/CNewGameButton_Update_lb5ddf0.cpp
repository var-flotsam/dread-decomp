// GUI::CNewGameButton's unit (tools/tu_map.py unit 1707): the button's update. When a new game was asked for
// (flag at 0x2a1) and fn_7101053a98 does not veto it, the main menu is disabled, the GUI key dropped, the
// script function Init.CreateAndInitNewGame called for the cave start point, and a fresh UUID string stored
// in the game manager's second blackboard. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerDef.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2656.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "game/Uuid.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

// nn::util's UUID and formatter (as src/game/UnkPlayReport.cpp declares them).
namespace nn::util {
Uuid GenerateUuid();
s32 TSNPrintf(char* buffer, u64 size, const char* format, ...);
}  // namespace nn::util

struct UnkNewGameButton {
    u8 pad_0x0[0x2a1];
    bool field_0x2a1;  // a new game was asked for
};

extern CStrId g_unk_7172765228 __attribute__((visibility("hidden")));

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

// `dt` (s0) reaches the base update untouched in the original.
void CNewGameButton_UpdateAndCreatePendingNewGame(UnkNewGameButton* self, f32 dt) {
    fn_71011d8fe4(reinterpret_cast<UnkNewGameScreen*>(self), dt);
    if (!self->field_0x2a1 || fn_7101053a98(g_unk_7101d47310->field_0x1040))
        return;
    UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
    if (menu)
        menu = fn_710009d50c(menu->vf_0x0(), fn_7100b34230()) ? menu : nullptr;
    fn_71011cf2a4(reinterpret_cast<UnkGuiDisplayObject*>(menu), true);
    fn_710002ffb4(&g_unk_717271e8f8->field_0xca8, &g_unk_7172765228);
    fn_7100000790("Init.CreateAndInitNewGame", "iss", fn_7100dffc58(g_unk_7101d47310), "s010_cave",
                  "StartPoint0");

    CBlackboard* blackboard = &g_unk_7101d47310->field_0x820;
    nn::util::Uuid uuid = nn::util::GenerateUuid();
    char id[0x40];
    nn::util::TSNPrintf(id, sizeof(id), "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                        uuid.data[0], uuid.data[1], uuid.data[2], uuid.data[3], uuid.data[4], uuid.data[5],
                        uuid.data[6], uuid.data[7], uuid.data[8], uuid.data[9], uuid.data[10], uuid.data[11],
                        uuid.data[12], uuid.data[13], uuid.data[14], uuid.data[15]);
    SetValue(blackboard, TableId(0x1148), DefId(g_unk_71727594b8), fn_7100007f84, CRntString(id));
    self->field_0x2a1 = false;
}
