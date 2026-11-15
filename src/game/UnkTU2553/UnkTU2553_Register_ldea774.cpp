// Unit 2553 (CGameManager::CTunableGameManager): registers an object: appended to the manager's
// first list unless the manager already finds it by its id, and dropped from its second list.
// Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkTU2553ListTag1;
struct UnkTU2553ListTag2;
struct UnkTU2553KeyInfo {
    u8 pad_0x0[0x30];
    u64 id;
};
struct UnkTU2553Object {
    u8 pad_0x0[0x30];
    UnkTU2553KeyInfo* info;
    u8 pad_0x38[0x17d];
    u8 field_0x1b5;
};
struct UnkTU2553Manager_ldea774 {
    u8 pad_0x0[0x2688];
    base::UnkSList<UnkTU2553ListTag1> list;
    u8 pad_0x26b8[0xb0];
    base::UnkSList<UnkTU2553ListTag2> other;
};
static_assert(__builtin_offsetof(UnkTU2553Manager_ldea774, other) == 0x2768, "UnkTU2553Manager_ldea774 layout");

using UnkList2 = base::UnkSList<UnkTU2553ListTag2>;

// Unlinks a node (found from the head) and frees it.
static inline void EraseNode(UnkList2& list, UnkList2::Node* node) {
    UnkList2::Node* last = nullptr;
    UnkList2::Node* it = list.head;
    while (it) {
        UnkList2::Node* prev = last;
        last = it;
        it = it->next;
        if (last != node)
            continue;
        if (prev)
            prev->next = it;
        else
            list.head = it;
        if (prev && list.tail == node)
            list.tail = prev;
        if (--list.count == 0) {
            list.head = nullptr;
            list.tail = nullptr;
        }
        list.allocator->vf_0x48(node);
        return;
    }
}

// Unlinks the node holding `value` and frees it; nothing when it is not there.
static inline void RemoveValue(UnkList2& list, u64 value) {
    UnkList2::Node* node = list.head;
    while (node && node->value != value)
        node = node->next;
    if (node)
        EraseNode(list, node);
}

void fn_7100dea774(UnkGameManager* manager, void* object) {
    auto* self = reinterpret_cast<UnkTU2553Manager_ldea774*>(manager);
    auto* obj = static_cast<UnkTU2553Object*>(object);
    if (!obj || obj->field_0x1b5)
        return;
    u64 id = obj->info->id;
    alignas(8) u32 flags = 0xc;
    if (!fn_7100deba8c(manager, &id, &flags))
        self->list.PushBack(reinterpret_cast<u64>(obj));
    RemoveValue(self->other, reinterpret_cast<u64>(obj));
}
