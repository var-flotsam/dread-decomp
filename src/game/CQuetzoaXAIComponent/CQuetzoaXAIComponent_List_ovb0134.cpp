// CQuetzoaXAIComponent's unit (tools/tu_map.py unit 962): fn_7100640660 runs the base handler
// (fn_710110bb4c) and then, unless the component is blocked (+0x14bc8), moves its entry (+0x149a0) to
// the front of the other object's list (+0x28) when the entry is already listed and its value
// (+0x14bcc) is above 10. The list helpers are base::UnkSList's (the erase as in
// src/game/CGrabComponent/CGrabComponent_Grab_ovb0057.cpp). Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkQuetzoaListTag_ovb0134;
using UnkQuetzoaList_ovb0134 = base::UnkSList<UnkQuetzoaListTag_ovb0134>;

struct UnkQuetzoaOther_ovb0134 {
    u8 pad_0x0[0x28];
    UnkQuetzoaList_ovb0134 list;  // 0x28
};

struct UnkQuetzoaXAI_ovb0134 {
    u8 pad_0x0[0x149a0];
    u8 entry[0x228];  // 0x149a0
    bool blocked;     // 0x14bc8
    u8 pad_0x14bc9[0x3];
    f32 value;  // 0x14bcc
};
static_assert(__builtin_offsetof(UnkQuetzoaXAI_ovb0134, value) == 0x14bcc, "layout");

void fn_710110bb4c(UnkQuetzoaXAI_ovb0134* self, UnkQuetzoaOther_ovb0134* other, void* arg, bool flag);

static inline s32 IndexOf(UnkQuetzoaList_ovb0134* list, u64 v) {
    u32 i = 0;
    for (UnkQuetzoaList_ovb0134::Node* n = list->head; n; n = n->next, ++i)
        if (n->value == v)
            return static_cast<s32>(i);
    return -1;
}

static inline UnkQuetzoaList_ovb0134::Node* Find(UnkQuetzoaList_ovb0134* list, u64 v) {
    for (UnkQuetzoaList_ovb0134::Node* n = list->head; n; n = n->next)
        if (n->value == v)
            return n;
    return nullptr;
}

static inline void Erase(UnkQuetzoaList_ovb0134* list, UnkQuetzoaList_ovb0134::Node* pos) {
    UnkQuetzoaList_ovb0134::Node* prev = nullptr;
    for (UnkQuetzoaList_ovb0134::Node* it = list->head; it;) {
        UnkQuetzoaList_ovb0134::Node* next = it->next;
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

void fn_7100640660(UnkQuetzoaXAI_ovb0134* self, UnkQuetzoaOther_ovb0134* other, void* arg, bool flag) {
    fn_710110bb4c(self, other, arg, flag);
    if (self->blocked)
        return;
    u64 v = reinterpret_cast<u64>(self->entry);
    if (IndexOf(&other->list, v) < 0)
        return;
    if (!(self->value > 10.0f))
        return;
    if (UnkQuetzoaList_ovb0134::Node* n = Find(&other->list, v))
        Erase(&other->list, n);
    other->list.PushFront(v);
}
