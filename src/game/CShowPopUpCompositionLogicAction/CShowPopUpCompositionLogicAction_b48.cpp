// CShowPopUpCompositionLogicAction unit (tools/tu_map.py): fn_7100a3fff4 runs the base action's step
// (fn_7100a55ac8), then hands every non-empty id of the list at +0x10 to the GUI composition at +0x10
// of the game manager's +0x2258 entry (fn_7100b5fd6c). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkTutoExitAction_b47;
void fn_7100a55ac8(UnkTutoExitAction_b47* self);
void fn_7100b5fd6c(void* object, base::global::UnkStringData** name, void* block, bool a, bool b);
base::UnkStrIdMember* fn_71000ce740(base::UnkVector<base::UnkStrIdMember>* self, u32 index);

struct UnkPopUpEntry_b48 {
    u8 pad_0x0[0x10];
    void* composition;
};

struct UnkShowPopUpAction_b48 {
    u8 pad_0x0[0x10];
    base::UnkVector<base::UnkStrIdMember> ids;
};

void fn_7100a3fff4(UnkShowPopUpAction_b48* self) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    for (u32 i = 0; i < self->ids.count; ++i) {
        if (fn_71000ce740(&self->ids, i)->data == &g_unk_7101d47108->empty)
            continue;
        auto* entry = static_cast<UnkPopUpEntry_b48*>(g_unk_7101d47310->field_0x2258);
        if (!entry || !entry->composition)
            continue;
        fn_7100b5fd6c(entry->composition, &fn_71000ce740(&self->ids, i)->data, &g_unk_7101d470e0, true, false);
    }
}
