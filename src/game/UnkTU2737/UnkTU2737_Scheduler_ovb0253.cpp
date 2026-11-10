// Unit 2737 (a scheduler of timed objects, no reliable name): three singly linked lists of objects
// (all of them, those waiting to join, those that have finished): the move to the finished list, the
// destruction of the finished objects and the destructor. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkTimed_b0253 {
    u32 id;  // 0x0
    u8 pad_0x4[0x1];
    u8 field_0x5;  // 0x5
    u8 pad_0x6[0x2];
    // A union member: no type-based alias info, so the countdown's store forces the reloads
    // the original has.
    union {
        f32 remaining;  // 0x8
    };
};

struct UnkListTag_b0253 {};
using UnkList_b0253 = base::UnkSList<UnkListTag_b0253>;

struct UnkScheduler_b0253 {
    UnkList_b0253 all;       // 0x0
    UnkList_b0253 pending;   // 0x30
    UnkList_b0253 finished;  // 0x60
    u32 next_id;             // 0x90
};
static_assert(sizeof(UnkList_b0253) == 0x30, "UnkList_b0253 layout");
static_assert(__builtin_offsetof(UnkScheduler_b0253, next_id) == 0x90, "UnkScheduler_b0253 layout");

void fn_7101073bcc(UnkTimed_b0253* object);
extern char g_unk_7101cb5598[];  // the list base's vtable

// The position of a value in a list, or -1.
static inline s32 IndexOf(UnkList_b0253& list, u64 value) {
    u32 index = 0;
    for (UnkList_b0253::Node* n = list.head; n; n = n->next, ++index)
        if (n->value == value)
            return (s32)index;
    return -1;
}

// Unlinks a node (found from the head) and frees it.
static inline void EraseNode(UnkList_b0253& list, UnkList_b0253::Node* node) {
    UnkList_b0253::Node* last = nullptr;
    UnkList_b0253::Node* it = list.head;
    while (it) {
        UnkList_b0253::Node* prev = last;
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
static inline void RemoveValue(UnkList_b0253& list, u64 value) {
    UnkList_b0253::Node* node = list.head;
    while (node && node->value != value)
        node = node->next;
    if (node)
        EraseNode(list, node);
}

// Moves an object from the waiting list to the finished one.
void fn_71011a5958(UnkScheduler_b0253* self, UnkTimed_b0253* object) {
    u64 value = reinterpret_cast<u64>(object);
    if (IndexOf(self->finished, value) < 0)
        self->finished.PushBack(value);
    if (IndexOf(self->pending, value) >= 0)
        RemoveValue(self->pending, value);
}

// Frees the nodes of a list, leaving it empty.
static inline void FreeNodes(UnkList_b0253& list) {
    for (UnkList_b0253::Node* n = list.head; n;) {
        UnkList_b0253::Node* next = n->next;
        list.allocator->vf_0x48(n);
        n = next;
    }
    list.count = 0;
    list.head = nullptr;
    list.tail = nullptr;
}

// Destroys the object a node holds.
static inline void DestroyValue(UnkList_b0253::Node* n) {
    UnkTimed_b0253* object = reinterpret_cast<UnkTimed_b0253*>(n->value);
    if (object) {
        fn_7101073bcc(object);
        fn_7100083c00(object);
        n->value = 0;
    }
}

// Destroys the finished objects, dropping them from the full list.
void fn_71011a5650(UnkScheduler_b0253* self) {
    for (UnkList_b0253::Node* n = self->finished.head; n; n = n->next) {
        if (self->all.head)
            RemoveValue(self->all, n->value);
        DestroyValue(n);
    }
    FreeNodes(self->finished);
}

// What a list's own destructor does: restore the base vtable and free the nodes.
static inline void ReleaseList(UnkList_b0253& list) {
    list.field_0x0 = reinterpret_cast<u64>(g_unk_7101cb5598 + 0x10);
    FreeNodes(list);
}

// The destructor: empties the three lists, destroying the objects of the first, then the lists go.
void fn_71011a514c(UnkScheduler_b0253* self) {
    for (UnkList_b0253::Node* n = self->all.head; n; n = n->next)
        DestroyValue(n);
    FreeNodes(self->all);
    FreeNodes(self->pending);
    FreeNodes(self->finished);
    ReleaseList(self->finished);
    ReleaseList(self->pending);
    ReleaseList(self->all);
}
