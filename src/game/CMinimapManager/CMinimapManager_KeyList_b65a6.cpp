// CMinimapManager's unit: removes `key` from (fn_710002ffb4) or adds it to (fn_710002fd10) the key list
// at +0xd63d90 (game/UnkGuiRoot.h). Names are placeholders.
#include "game/UnkGuiRoot.h"

void fn_7100e8f9a0(char* self, bool remove, base::global::CStrId* key) {
    UnkGuiKeyList* list = reinterpret_cast<UnkGuiKeyList*>(self + 0xd63d90);
    if (remove)
        fn_710002ffb4(list, key);
    else
        fn_710002fd10(list, key);
}
