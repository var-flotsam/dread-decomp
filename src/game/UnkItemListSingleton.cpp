// A singleton (unit 2732; g_unk_7172729910, 0x2208 bytes, built by fn_710119aee8 on first use)
// that begins with a list of the items registered with it: fn_71010ef320 and fn_7101199544 add an
// object's member to it through fn_710119ba7c. An item's state at +0x99c is raised to at least 1
// when it is added. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkListedItem {
    u8 unk_0x0[0x99c];
    s32 state;
};

struct UnkItemListTag;
using UnkItemList = base::UnkSList<UnkItemListTag>;

// Adds `item` at the end of the list unless it is null or already listed.
void fn_710119ba7c(UnkItemList* self, UnkListedItem* item) {
    if (!item)
        return;
    for (UnkItemList::Node* n = self->head; n; n = n->next)
        if (reinterpret_cast<UnkListedItem*>(n->value) == item)
            return;
    if (item->state < 1)
        item->state = 1;
    self->PushBack(reinterpret_cast<u64>(item));
}
