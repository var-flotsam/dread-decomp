// CInputComponent (tools/tu_map.py unit 1326; the name from reflection): a reset. Clears the
// counters at +0x9e0/+0x9e8 and the value at +0x3c of every entry of the list at +0xab0, clears the
// five flags from +0xb04 when any is set, then registers the component's member at +0x40 with the
// item list singleton (g_unk_7172729910, built on first use; src/game/UnkItemListSingleton.cpp).
// Names and layout are placeholders.
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "game/UnkTU2732.h"
#include "types.h"

struct UnkListedItem;
struct UnkItemListTag;
using UnkItemList = base::UnkSList<UnkItemListTag>;
void fn_710119ba7c(UnkItemList* self, UnkListedItem* item);

extern void* g_unk_7172729910;  // the item list singleton

struct UnkInputEntry_b44a2 {
    u8 pad_0x0[0x3c];
    u32 field_0x3c;
};

struct UnkInputNode_b44a2 {
    UnkInputEntry_b44a2* entry;
    UnkInputNode_b44a2* next;
};

struct UnkInputFlags_b44a2 {
    bool f[5];
};

struct UnkInputComponent_b44a2 {
    u8 pad_0x0[0x40];
    u8 field_0x40[0x9a0];
    u64 field_0x9e0;
    u32 field_0x9e8;
    u8 pad_0x9ec[0xc4];
    UnkInputNode_b44a2* field_0xab0;
    u8 pad_0xab8[0x4c];
    UnkInputFlags_b44a2 flags;  // 0xb04
};

void fn_71008720b4(UnkInputComponent_b44a2* self) {
    UnkInputNode_b44a2* n = self->field_0xab0;
    self->field_0x9e8 = 0;
    self->field_0x9e0 = 0;
    for (; n; n = n->next)
        n->entry->field_0x3c = 0;
    UnkInputFlags_b44a2* flags = &self->flags;
    if (flags->f[0] || flags->f[4] || flags->f[3] || flags->f[1] || flags->f[2])
        *flags = {};
    if (!g_unk_7172729910) {
        void* p = fn_71000839a0(0x2208, 0x3f);
        fn_710119aee8(p);
        g_unk_7172729910 = p;
    }
    fn_710119ba7c(static_cast<UnkItemList*>(g_unk_7172729910), reinterpret_cast<UnkListedItem*>(self->field_0x40));
}
