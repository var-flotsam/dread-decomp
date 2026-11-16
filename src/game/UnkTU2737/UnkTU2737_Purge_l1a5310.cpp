// Unit 2737 (a scheduler of timed objects): destroys every object whose flag at +0x4 matches the argument,
// dropping it from all three lists, then drops matching objects still in the waiting list.
// Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU2662.h"
#include "types.h"

struct UnkTimed_b0253;
struct UnkTimed_l1a5310 {
    u32 id;           // 0x0
    u8 field_0x4;     // 0x4
};

struct UnkListTag_l1a5310 {};
using UnkList_l1a5310 = base::UnkSList<UnkListTag_l1a5310>;

struct UnkScheduler_l1a5310 {
    UnkList_l1a5310 all;       // 0x0
    UnkList_l1a5310 pending;   // 0x30
    UnkList_l1a5310 finished;  // 0x60
};
static_assert(__builtin_offsetof(UnkScheduler_l1a5310, finished) == 0x60, "UnkScheduler_l1a5310 layout");

static inline UnkTimed_l1a5310* ObjectOf(UnkList_l1a5310::Node* node) {
    return reinterpret_cast<UnkTimed_l1a5310*>(node->value);
}

// Unlinks `node` and frees it; the node after it, or null when it is not in the list.
static inline UnkList_l1a5310::Node* Erase(UnkList_l1a5310& list, UnkList_l1a5310::Node* node) {
    UnkList_l1a5310::Node* prev = nullptr;
    UnkList_l1a5310::Node* cur = list.head;
    UnkList_l1a5310::Node* next;
    UnkList_l1a5310::Node* last = nullptr;
    for (;;) {
        if (!cur)
            return nullptr;
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
    return next;
}

// Unlinks the node holding `object` and frees it; nothing when it is not there.
static inline void Remove(UnkList_l1a5310& list, UnkTimed_l1a5310* object) {
    UnkList_l1a5310::Node* node = list.head;
    while (node && ObjectOf(node) != object)
        node = node->next;
    if (node)
        Erase(list, node);
}

void fn_71011a5310(UnkScheduler_l1a5310* self, bool flag) {
    UnkList_l1a5310::Node* node = self->all.head;
    while (node) {
        UnkTimed_l1a5310* object = ObjectOf(node);
        if ((object->field_0x4 != 0) != flag) {
            node = node->next;
            continue;
        }
        Remove(self->pending, object);
        Remove(self->finished, object);
        node = Erase(self->all, node);
        if (object) {
            fn_7101073bcc(reinterpret_cast<UnkTimed_b0253*>(object));
            fn_7100083c00(object);
        }
    }

    node = self->pending.head;
    while (node) {
        if ((ObjectOf(node)->field_0x4 != 0) != flag)
            node = node->next;
        else
            node = Erase(self->pending, node);
    }
}
