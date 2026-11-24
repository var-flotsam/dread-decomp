// CChozoWarriorXAIComponent unit: runs the base handler (fn_710049b40c, its flag off) and then, once, when
// the pending flag (+0x15502) is set and the component's entry (+0x14d08) is already in the other object's
// list (+0x28), clears the flag and moves the entry to the front of that list. Names and layouts are
// placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkChozoXListTag_l48f748;
using UnkChozoXList_l48f748 = base::UnkSList<UnkChozoXListTag_l48f748>;

struct UnkChozoXOther_l48f748 {
    u8 pad_0x0[0x28];
    UnkChozoXList_l48f748 list;  // 0x28
};

struct UnkChozoWarriorXAI_l48f748 {
    u8 pad_0x0[0x14d08];
    u8 entry[0x7fa];  // 0x14d08
    bool pending;     // 0x15502
};
static_assert(__builtin_offsetof(UnkChozoWarriorXAI_l48f748, pending) == 0x15502, "layout");

void fn_710049b40c(UnkChozoWarriorXAI_l48f748* self, UnkChozoXOther_l48f748* other, u32 id, bool flag);

static inline s32 IndexOf(UnkChozoXList_l48f748* list, u64 v) {
    u32 i = 0;
    for (UnkChozoXList_l48f748::Node* n = list->head; n; n = n->next, ++i)
        if (n->value == v)
            return static_cast<s32>(i);
    return -1;
}

static inline UnkChozoXList_l48f748::Node* Find(UnkChozoXList_l48f748* list, u64 v) {
    for (UnkChozoXList_l48f748::Node* n = list->head; n; n = n->next)
        if (n->value == v)
            return n;
    return nullptr;
}

static inline void Erase(UnkChozoXList_l48f748* list, UnkChozoXList_l48f748::Node* pos) {
    UnkChozoXList_l48f748::Node* prev = nullptr;
    for (UnkChozoXList_l48f748::Node* it = list->head; it;) {
        UnkChozoXList_l48f748::Node* next = it->next;
        if (it == pos) {
            if (prev) {
                prev->next = next;
                if (list->tail == pos)
                    list->tail = prev;
            } else {
                list->head = next;
            }
            if (--list->count == 0) {
                list->head = nullptr;
                list->tail = nullptr;
            }
            list->allocator->vf_0x48(pos);
            return;
        }
        prev = it;
        it = next;
    }
}

void CChozoWarriorXAIComponent_MovePendingEntryToListFront(UnkChozoWarriorXAI_l48f748* self, UnkChozoXOther_l48f748* other, u32 id, bool flag) {
    fn_710049b40c(self, other, id, false);
    if (!self->pending)
        return;
    u64 v = reinterpret_cast<u64>(self->entry);
    if (IndexOf(&other->list, v) < 0)
        return;
    self->pending = false;
    if (UnkChozoXList_l48f748::Node* n = Find(&other->list, v))
        Erase(&other->list, n);
    other->list.PushFront(v);
}
