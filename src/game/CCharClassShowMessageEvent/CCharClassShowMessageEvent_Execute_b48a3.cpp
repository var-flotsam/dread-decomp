// CCharClassShowMessageEvent unit (tools/tu_map.py): hands the event's message id (+0x10), when not
// empty, with the block at +0x18 to the GUI composition at +0x10 of the game manager's +0x2258 entry
// (fn_7100b5fd6c). Names and layout are placeholders.
#include "base/global/CStrId.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkMessageEntry_b48a3 {
    u8 pad_0x0[0x10];
    void* composition;
};

struct UnkShowMessageEvent_b48a3 {
    u8 pad_0x0[0x10];
    base::global::CStrId id;
    u8 field_0x18[1];
};

void fn_7100be02c8(UnkShowMessageEvent_b48a3* self) {
    auto* entry = static_cast<UnkMessageEntry_b48a3*>(g_unk_7101d47310->field_0x2258);
    if (!entry || !entry->composition)
        return;
    if (self->id.data == &g_unk_7101d47108->empty)
        return;
    fn_7100b5fd6c(entry->composition, &self->id.data, self->field_0x18, true, false);
}
