// CDoorLifeComponent (tools/tu_map.py; the name from reflection): adds a key to the key list at
// +0xaf8 (fn_710002fd10) or removes it (fn_710002ffb4); when the list's state (fn_710002fb70)
// changed, the door opens (fn_71008d0ef4) or closes (fn_71008d498c), as CMagnetSlidingBlockComponent
// does with its list. Names and layout are placeholders.
#include "game/UnkGuiRoot.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU39.h"
#include "types.h"

struct UnkDoorLife_b44a2 {
    u8 pad_0x0[0xaf8];
    u8 field_0xaf8[0x10];
    UnkGuiKeyList* keys() { return reinterpret_cast<UnkGuiKeyList*>(field_0xaf8); }
};

void fn_71008d0740(UnkDoorLife_b44a2* self, bool add, base::global::CStrId* key) {
    UnkGuiKeyList* list = self->keys();
    bool before = fn_710002fb70(list);
    bool after;
    if (!add) {
        fn_710002ffb4(list, key);
        after = fn_710002fb70(list);
    } else {
        fn_710002fd10(list, key);
        after = fn_710002fb70(list);
    }
    if (before == after)
        return;
    if (!after)
        fn_71008d498c(self, 0, 0, 0);
    else
        fn_71008d0ef4(self);
}
