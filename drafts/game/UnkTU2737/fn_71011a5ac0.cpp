// Unit 2737 (a scheduler of timed objects, no reliable name): three singly linked lists of objects
// (all of them, those waiting to join, those that have finished) and the passes that move objects
// between them. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
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

void* fn_7101073b80(void* object, f32 time, u32 id, u32 a, u32 b, void* x, void* y, void* z);
void fn_7101073c30(UnkTimed_b0253* object);
const char* fn_7101073c20(UnkTimed_b0253* object);
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

// Queues an object: at once as finished when it has no time, else as waiting.
u32 fn_71011a5ac0(UnkScheduler_b0253* self, f32 time, u32 a, u32 b, void* x, void* y, void* z) {
    u32 id = self->next_id;
    self->next_id = id + 1;
    UnkTimed_b0253* object = static_cast<UnkTimed_b0253*>(fn_71000839a0(0x50, 5));
    fn_7101073b80(object, time, id, a & 1, b & 1, x, y, z);
    if (time < 0.0f) {
        fn_7101073c30(object);
        fn_71011a5958(self, object);
    } else {
        self->pending.PushBack(reinterpret_cast<u64>(object));
    }
    return id;
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

// IndexOf for the value of a node.
static inline s32 IndexOfNode(UnkList_b0253& list, UnkList_b0253::Node* node) {
    u32 index = 0;
    UnkList_b0253::Node* first = list.head;
    u64 value = node->value;
    for (UnkList_b0253::Node* n = first; n; n = n->next, ++index)
        if (n->value == value)
            return (s32)index;
    return -1;
}

static inline UnkTimed_b0253* Object(UnkList_b0253::Node* n) {
    return reinterpret_cast<UnkTimed_b0253*>(n->value);
}

// Counts an object down and finishes it when it has run out (continues the loop otherwise).
#define TICK_OBJECT(n)                                  \
    do {                                                \
        if (IndexOfNode(self->finished, (n)) >= 0)      \
            continue;                                   \
        Object(n)->remaining -= dt;                     \
        if (!(Object(n)->remaining <= 0.0f))            \
            continue;                                   \
        fn_7101073c30(Object(n));                       \
        fn_71011a5958(self, Object(n));                 \
    } while (0)

// Ticks the objects: those that run out of time finish. Then the finished ones are destroyed and the
// waiting ones join the full list.
void fn_71011a5798(UnkScheduler_b0253* self, f32 dt, bool only_flagged) {
    if (self->all.head) {
        if (only_flagged) {
            for (UnkList_b0253::Node* n = self->all.head; n; n = n->next) {
                if (!Object(n)->field_0x5)
                    continue;
                TICK_OBJECT(n);
            }
        } else {
            for (UnkList_b0253::Node* n = self->all.head; n; n = n->next)
                TICK_OBJECT(n);
        }
    }
    fn_71011a5650(self);
    for (UnkList_b0253::Node* n = self->pending.head; n; n = n->next)
        self->all.PushBack(n->value);
    FreeNodes(self->pending);
}

// Stops the objects with an id: those in the full list finish, those waiting are dropped.
void fn_71011a5bc8(UnkScheduler_b0253* self, u32 id) {
    for (UnkList_b0253::Node* n = self->all.head; n; n = n->next)
        if (Object(n)->id == id)
            fn_71011a5958(self, Object(n));
    for (UnkList_b0253::Node* n = self->pending.head; n;) {
        if (Object(n)->id == id) {
            UnkList_b0253::Node* next = n->next;
            EraseNode(self->pending, n);
            n = next;
        } else {
            n = n->next;
        }
    }
}

// The same for the objects with a name.
void fn_71011a5cdc(UnkScheduler_b0253* self, const base::global::CRntString* name) {
    for (UnkList_b0253::Node* n = self->all.head; n; n = n->next)
        if (fn_710007f140(name, fn_7101073c20(Object(n))))
            fn_71011a5958(self, Object(n));
    for (UnkList_b0253::Node* n = self->pending.head; n;) {
        if (fn_710007f140(name, fn_7101073c20(Object(n)))) {
            UnkList_b0253::Node* next = n->next;
            EraseNode(self->pending, n);
            n = next;
        } else {
            n = n->next;
        }
    }
}
