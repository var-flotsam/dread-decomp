// Unit 2553 (CGameManager::CTunableGameManager): adds an object to the manager's list unless it is
// already listed or flagged. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkTU2553ListTag;
struct UnkTU2553ListedObject {
    u8 pad_0x0[0x1b5];
    u8 field_0x1b5;
};
struct UnkTU2553Manager_le023e8 {
    u8 pad_0x0[0x27d8];
    base::UnkSList<UnkTU2553ListTag> list;
};

// Position of `obj` in the list, or -1.
static s32 IndexOf(UnkTU2553Manager_le023e8* self, UnkTU2553ListedObject* obj) {
    u32 i = 0;
    for (auto* n = self->list.head; n; n = n->next, ++i)
        if (reinterpret_cast<UnkTU2553ListedObject*>(n->value) == obj)
            return i;
    return -1;
}

void fn_7100e023e8(UnkTU2553Manager_le023e8* self, UnkTU2553ListedObject* obj) {
    if (!obj || obj->field_0x1b5)
        return;
    if (IndexOf(self, obj) >= 0)
        return;
    self->list.PushBack(reinterpret_cast<u64>(obj));
}
