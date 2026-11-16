// CCooldownXBossAIComponent's unit (tools/tu_map.py unit 754): runs the base handler (fn_710110bb4c), then
// reorders the component's entries in the other object's list (+0x28): in state 2 the entry at +0x15510
// moves to the front, and while the value at +0x1581c is positive the entry at +0x155a8 moves to the
// front or the back by the distance the owner's component (+0x1b8) reports. Names and layouts are
// placeholders.
#include "base/UnkSList.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkCooldownListTag_l4d4ec4;
using UnkCooldownList_l4d4ec4 = base::UnkSList<UnkCooldownListTag_l4d4ec4>;
using UnkCooldownNode_l4d4ec4 = UnkCooldownList_l4d4ec4::Node;

struct UnkQueueNode_l4d4ec4 {
    u8 pad_0x0[0x8];
    u64 value;  // 0x8
    u8 pad_0x10[0x8];
    UnkQueueNode_l4d4ec4* next;  // 0x18
};

struct UnkQueue_l4d4ec4 {
    u8 pad_0x0[0x10];
    UnkQueueNode_l4d4ec4* head;  // 0x10
};

struct UnkCooldownOther_l4d4ec4 {
    u8 pad_0x0[0x20];
    UnkQueue_l4d4ec4* queue;       // 0x20
    UnkCooldownList_l4d4ec4 list;  // 0x28
};

struct UnkRangeComponent_l4d4ec4 {
    u8 pad_0x0[0x54];
    f32 distance;  // 0x54
};

struct UnkCooldownOwner_l4d4ec4 {
    u8 pad_0x0[0x1b8];
    UnkRangeComponent_l4d4ec4* range;  // 0x1b8
};

struct UnkCooldownAI_l4d4ec4 {
    u8 pad_0x0[0x30];
    UnkCooldownOwner_l4d4ec4* owner;  // 0x30
    u8 pad_0x38[0x1108];
    s32 state;  // 0x1140
    u8 pad_0x1144[0x15130 - 0x1144];
    u8 entry_0x15130[0x3e0];  // 0x15130
    u8 entry_0x15510[0x98];   // 0x15510
    u8 entry_0x155a8[0x274];  // 0x155a8
    f32 value;                // 0x1581c
    u8 pad_0x15820[0x14];
    bool flag;  // 0x15834
};
static_assert(__builtin_offsetof(UnkCooldownAI_l4d4ec4, entry_0x155a8) == 0x155a8, "layout");
static_assert(__builtin_offsetof(UnkCooldownAI_l4d4ec4, flag) == 0x15834, "layout");

// Where `v` is in the list, or -1.
static inline s32 IndexOf(UnkCooldownList_l4d4ec4* list, u64 v) {
    u32 i = 0;
    for (UnkCooldownNode_l4d4ec4* n = list->head; n; n = n->next, ++i)
        if (n->value == v)
            return static_cast<s32>(i);
    return -1;
}

// The same, counting at most `count` nodes.
static inline s32 IndexOfCounted(UnkCooldownList_l4d4ec4* list, u64 v) {
    UnkCooldownNode_l4d4ec4* n = list->head;
    for (u32 i = 0; i < list->count; ++i, n = n->next)
        if (n->value == v)
            return static_cast<s32>(i);
    return -1;
}

// Where `v` is in the queue, or its length.
static inline u32 QueuePosition(UnkQueue_l4d4ec4* queue, u64 v) {
    u32 i = 0;
    for (UnkQueueNode_l4d4ec4* n = queue->head; n; n = n->next, ++i)
        if (n->value == v)
            break;
    return i;
}

// True when `v` is one of the queue's first two.
static inline bool IsNextInQueue(UnkQueue_l4d4ec4* queue, u64 v) {
    u32 i = 0;
    for (UnkQueueNode_l4d4ec4* n = queue->head; n && i < 2; n = n->next, ++i)
        if (n->value == v)
            return true;
    return false;
}

static inline UnkCooldownNode_l4d4ec4* Find(UnkCooldownList_l4d4ec4* list, u64 v) {
    for (UnkCooldownNode_l4d4ec4* n = list->head; n; n = n->next)
        if (n->value == v)
            return n;
    return nullptr;
}

static inline void Erase(UnkCooldownList_l4d4ec4* list, UnkCooldownNode_l4d4ec4* pos) {
    UnkCooldownNode_l4d4ec4* prev = nullptr;
    for (UnkCooldownNode_l4d4ec4* it = list->head; it;) {
        UnkCooldownNode_l4d4ec4* next = it->next;
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

static inline void Remove(UnkCooldownList_l4d4ec4* list, u64 v) {
    if (UnkCooldownNode_l4d4ec4* n = Find(list, v))
        Erase(list, n);
}

struct UnkQuetzoaXAI_ovb0134;
struct UnkQuetzoaOther_ovb0134;

void fn_71004d4ec4(UnkCooldownAI_l4d4ec4* self, UnkCooldownOther_l4d4ec4* other, u32 id, bool flag) {
    fn_710110bb4c(reinterpret_cast<UnkQuetzoaXAI_ovb0134*>(self),
                  reinterpret_cast<UnkQuetzoaOther_ovb0134*>(other), id, flag);
    UnkCooldownList_l4d4ec4* list = &other->list;

    if (self->state == 2 && !self->flag) {
        u64 waiting = reinterpret_cast<u64>(self->entry_0x15130);
        if (QueuePosition(other->queue, waiting) != 0 || self->value > 0.0f) {
            u64 v = reinterpret_cast<u64>(self->entry_0x15510);
            if (IndexOf(list, v) >= 0) {
                Remove(list, v);
                list->PushFront(v);
            }
        }
    }

    if (!(self->value > 0.0f))
        return;
    bool close = false;
    bool medium = false;
    if (UnkRangeComponent_l4d4ec4* range = self->owner->range) {
        f32 distance = range->distance;
        medium = distance < 6500.0f && distance > 2500.0f;
        if (distance <= 2500.0f)
            close = true;
    }
    u64 v = reinterpret_cast<u64>(self->entry_0x155a8);
    s32 index = IndexOfCounted(list, v);
    if (medium) {
        if (index >= 0) {
            Remove(list, v);
            list->PushBack(v);
        }
    } else if (close && index > 0 && !IsNextInQueue(other->queue, v)) {
        Remove(list, v);
        list->PushFront(v);
    }
}
