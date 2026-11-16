// Unit 1325 of tools/tu_map.py: removing a timed entry (a name, a timer and a list of values) from its
// intrusive list, destroying and freeing it. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CEmmyProtoAIComponent.h"
#include "types.h"

using base::global::CStrId;

// The types src/game/UnkUnit1325/UnkUnit1325_b36.cpp gives the entry and the list.
struct UnkTimedNode;
struct UnkTimedList;

struct UnkTimedValuesTag_l86f718 {};
using UnkTimedValues_l86f718 = base::UnkSList<UnkTimedValuesTag_l86f718>;

struct UnkTimedEntry_l86f718 {
    CStrId id;                      // 0x0
    f32 timer;                      // 0x8
    UnkTimedValues_l86f718 values;  // 0x10
    u8 pad_0x40[0x8];
    UnkTimedEntry_l86f718* next;  // 0x48
};
static_assert(__builtin_offsetof(UnkTimedEntry_l86f718, next) == 0x48, "UnkTimedEntry_l86f718 layout");

struct UnkTimedEntryList_l86f718 {
    u64 field_0x0;
    u32 count;                       // 0x8
    UnkTimedEntry_l86f718* head;     // 0x10
    UnkTimedEntry_l86f718* tail;     // 0x18
    base::UnkAllocator* allocator;   // 0x20
};

// The value list's destructor: its base vtable back, then its nodes freed.
static inline void ReleaseValues(UnkTimedValues_l86f718& list) {
    list.field_0x0 = reinterpret_cast<u64>(g_unk_7101a46040 + 2);
    for (UnkTimedValues_l86f718::Node* n = list.head; n;) {
        UnkTimedValues_l86f718::Node* next = n->next;
        list.allocator->vf_0x48(n);
        n = next;
    }
    list.count = 0;
    list.head = nullptr;
    list.tail = nullptr;
}

// Destroys an entry and gives its memory back to the list's allocator.
static inline void DestroyEntry(UnkTimedEntryList_l86f718* list, UnkTimedEntry_l86f718* entry) {
    if (entry) {
        ReleaseValues(entry->values);
        entry->id.Reset();
        list->allocator->vf_0x48(entry);
    }
}

// Removes `node` and returns the one after it (null when it is not in the list).
UnkTimedNode* fn_710086f718(UnkTimedList* self, UnkTimedNode* node) {
    auto* list = reinterpret_cast<UnkTimedEntryList_l86f718*>(self);
    auto* entry = reinterpret_cast<UnkTimedEntry_l86f718*>(node);
    UnkTimedEntry_l86f718* last = nullptr;
    UnkTimedEntry_l86f718* it = list->head;
    while (it) {
        UnkTimedEntry_l86f718* prev = last;
        last = it;
        it = it->next;
        if (last != entry)
            continue;
        if (prev)
            prev->next = it;
        else
            list->head = it;
        if (prev && list->tail == entry)
            list->tail = prev;
        if (--list->count == 0) {
            list->head = nullptr;
            list->tail = nullptr;
        }
        DestroyEntry(list, entry);
        break;
    }
    return reinterpret_cast<UnkTimedNode*>(it);
}
