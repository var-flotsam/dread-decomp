// CScenario unit (tools/tu_map.py, the name from reflection): moving an object between two of the
// scenario's lists (base/UnkSList.h). Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkScenarioListTag_b52;
using UnkScenarioList_b52 = base::UnkSList<UnkScenarioListTag_b52>;

struct UnkScenarioLists_b52 {
    u8 pad_0x0[0x310];
    UnkScenarioList_b52 list_0x310;
    u8 pad_0x340[0x40];
    UnkScenarioList_b52 list_0x380;
};

// Removes the first node holding `value` (as src/game_lo/fn_710070f240.cpp) and frees it.
static inline __attribute__((always_inline)) void Remove(UnkScenarioList_b52& list, u64 value) {
    UnkScenarioList_b52::Node* node = list.head;
    for (; node; node = node->next) {
        if (node->value == value)
            break;
    }
    if (!node)
        return;
    UnkScenarioList_b52::Node* prev = nullptr;
    UnkScenarioList_b52::Node* cur = list.head;
    UnkScenarioList_b52::Node* next;
    UnkScenarioList_b52::Node* last = nullptr;
    for (;;) {
        if (!cur)
            return;
        prev = last;
        next = cur->next;
        if (cur == node)
            break;
        last = cur;
        cur = next;
    }
    if (prev) {
        prev->next = next;
        if (list.tail == node)
            list.tail = prev;
    } else {
        list.head = next;
    }
    if (--list.count == 0) {
        list.head = nullptr;
        list.tail = nullptr;
    }
    list.allocator->vf_0x48(node);
}

static inline __attribute__((always_inline)) s32 IndexOf(const UnkScenarioList_b52& list, u64 value) {
    u32 i = 0;
    for (UnkScenarioList_b52::Node* n = list.head; n; n = n->next, ++i)
        if (n->value == value)
            return (s32)i;
    return -1;
}

static inline __attribute__((always_inline)) void Move(UnkScenarioList_b52& from, UnkScenarioList_b52& to, u64 value) {
    Remove(from, value);
    if (IndexOf(to, value) < 0)
        to.PushBack(value);
}

// Moves `object` from the list at +0x380 to the one at +0x310 (once).
void fn_71011a28e8(UnkScenarioLists_b52* self, void* object) {
    Move(self->list_0x380, self->list_0x310, reinterpret_cast<u64>(object));
}

// Moves `object` from the list at +0x310 to the one at +0x380 (once).
void fn_71011a2c40(UnkScenarioLists_b52* self, void* object) {
    Move(self->list_0x310, self->list_0x380, reinterpret_cast<u64>(object));
}
