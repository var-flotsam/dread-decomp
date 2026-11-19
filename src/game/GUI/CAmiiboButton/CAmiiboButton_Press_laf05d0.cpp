// GUI::CAmiiboButton's unit (tools/tu_map.py unit 1649): pressing the amiibo button shows why amiibo effects are
// disabled (taking damage, or the hyper suit's beam) through the GUI composition, or else goes on as the menu
// state button does (fn_7100af65b4). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/GUI/CChangeMenuStateButton.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

struct UnkObj_7100af65b4;
bool fn_7100f0ec58(char* effects);
bool fn_7100f0ed04(char* effects);

struct UnkInventory;

struct UnkAmiiboPlayer_laf05d0 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkAmiiboGuiRef_laf05d0 {
    u8 pad_0x0[0x10];
    void* composition;
};

// The property's qualified id, built on first use.
static inline UnkQualifiedId& Qid_laf05d0(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), base::global::CStrId::Empty());
    return qid;
}

static inline void ShowMessage_laf05d0(CStrId& text) {
    auto* ref = static_cast<UnkAmiiboGuiRef_laf05d0*>(g_unk_7101d47310->field_0x2258);
    fn_7100b5fd6c(ref ? ref->composition : nullptr, &text.data, &base::global::CStrId::Empty(), true, false);
}

void fn_7100af05d0(UnkObj_7100af65b4* self) {
    static CStrId s_damage("#GUI_AMIIBO_STATE_EFFECTS_DISABLED_DAMAGE", true);
    static CStrId s_hyper_suit("#GUI_AMIIBO_STATE_EFFECTS_DISABLED_HYPERSUIT", true);
    CStrId* reason = nullptr;
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310)) {
        char* effects = reinterpret_cast<char*>(g_unk_7101d47310->field_0x38);
        if (fn_7100f0ec58(effects) || fn_7100f0ed04(effects)) {
            reason = &s_damage;
        } else if (UnkInventory* inventory = reinterpret_cast<UnkAmiiboPlayer_laf05d0*>(player)->inventory) {
            if (fn_71011428bc(inventory, Qid_laf05d0(g_unk_717275a718)))
                reason = &s_hyper_suit;
        }
    }
    if (reason)
        ShowMessage_laf05d0(*reason);
    else
        fn_7100af65b4(self);
}