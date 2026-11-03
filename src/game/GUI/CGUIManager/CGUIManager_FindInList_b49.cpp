// gui/scripts/guimanager.lua unit (tools/tu_map.py unit 2744): fn_71011b9fac asks every entry of the
// list behind +0x8 (head at +0x40; a node holds its entry at +0x8 and the next node at +0x10) for `key`
// (fn_7100b7e994) and returns the first answer that isn't null. Names and layouts are placeholders.
#include "types.h"

struct UnkGuiListEntry_b49;

struct UnkGuiListNode_b49 {
    u8 pad_0x0[0x8];
    UnkGuiListEntry_b49* entry;
    UnkGuiListNode_b49* next;
};

struct UnkGuiListHolder_b49 {
    u8 pad_0x0[0x40];
    UnkGuiListNode_b49* head;
};

struct UnkGuiListOwner_b49 {
    u8 pad_0x0[0x8];
    UnkGuiListHolder_b49* holder;
};

void* fn_7100b7e994(UnkGuiListEntry_b49* entry, u64 key);

void* fn_71011b9fac(UnkGuiListOwner_b49* self, u64 key) {
    for (UnkGuiListNode_b49* node = self->holder->head; node; node = node->next) {
        if (void* found = fn_7100b7e994(node->entry, key))
            return found;
    }
    return nullptr;
}
