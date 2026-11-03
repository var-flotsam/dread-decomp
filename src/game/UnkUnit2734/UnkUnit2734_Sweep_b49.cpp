// Unit 2734 (tools/tu_map.py; no reflected class): fn_710119d584 walks the list at +0x40 (an entry at +0x8
// of each node, the next node at +0x40) and drops every node whose entry fn_710119d5f0 accepts; the drop
// (fn_710119d7e4) gives the node to carry on from. Names and layouts are placeholders.
#include "types.h"

struct UnkSweepEntry_b49;

struct UnkSweepNode_b49 {
    u8 pad_0x0[0x8];
    u8 entry[0x38];  // UnkSweepEntry_b49
    UnkSweepNode_b49* next;  // 0x40
};

struct UnkSweepList_b49 {
    u8 pad_0x0[0x40];
    UnkSweepNode_b49* head;
};

// Its first argument is unused (the caller leaves x0 as it is): an empty struct by value.
struct UnkUnusedArg_b49 {};
bool fn_710119d5f0(UnkUnusedArg_b49, UnkSweepEntry_b49* entry, bool flag);
UnkSweepNode_b49* fn_710119d7e4(UnkSweepList_b49* self, UnkSweepNode_b49* node);

void fn_710119d584(UnkSweepList_b49* self, bool flag) {
    UnkSweepNode_b49* node = self->head;
    while (node) {
        if (fn_710119d5f0(UnkUnusedArg_b49(), reinterpret_cast<UnkSweepEntry_b49*>(node->entry), flag))
            node = fn_710119d7e4(self, node);
        else
            node = node->next;
    }
}
