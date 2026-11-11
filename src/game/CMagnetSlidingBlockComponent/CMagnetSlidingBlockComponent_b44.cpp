// CMagnetSlidingBlockComponent (tools/tu_map.py; the name from reflection): adds a key to the key
// list at +0x358 (fn_710002fd10) or removes it (fn_710002ffb4); when the list's state
// (fn_710002fb70) changed, fn_7100920f40 is told the negated new state. Names and layout are
// placeholders.
#include "game/UnkGuiRoot.h"
#include "game/UnkTU39.h"
#include "types.h"

struct UnkMagnetSlidingBlock_b44a2 {
    u8 pad_0x0[0x358];
    u8 field_0x358[0x10];
    UnkGuiKeyList* keys() { return reinterpret_cast<UnkGuiKeyList*>(field_0x358); }
};

void fn_7100920f40(UnkMagnetSlidingBlock_b44a2* self, bool state);

void fn_710091f100(UnkMagnetSlidingBlock_b44a2* self, bool remove, base::global::CStrId* key) {
    UnkGuiKeyList* list = self->keys();
    bool before = fn_710002fb70(list);
    if (!remove)
        fn_710002fd10(list, key);
    else
        fn_710002ffb4(list, key);
    if (before != fn_710002fb70(list))
        fn_7100920f40(self, !fn_710002fb70(list));
}
