// CPickableComponent's unit (tools/tu_map.py): when the definition's (+0x90) id at +0x60 is not the
// empty string id, hands it with the component's +0x40 block to fn_7100b5fd6c on the object at +0x10
// of the game manager's +0x2258 entry (null without one). Names are placeholders.
#include "base/global/CStrId.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkPickableDef_b45a2 {
    u8 pad_0x0[0x60];
    base::global::UnkStringData* name;
};

struct UnkPickableEntry_b45a2 {
    u8 pad_0x0[0x10];
    void* object;
};


struct UnkPickableComponent_b45a2 {
    u8 pad_0x0[0x40];
    u8 block[0x50];
    UnkPickableDef_b45a2* def;  // 0x90
};

void fn_71009ce12c(UnkPickableComponent_b45a2* self) {
    base::global::UnkStringData** name = &self->def->name;
    if (*name != &g_unk_7101d47108->empty) {
        auto* entry = static_cast<UnkPickableEntry_b45a2*>(g_unk_7101d47310->field_0x2258);
        fn_7100b5fd6c(entry ? entry->object : nullptr, name, self->block, true, false);
    }
}
