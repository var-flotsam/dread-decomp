// CMorphBallMovement's unit (hint only): moves the movement's listener part (+0x2278) from the list of the object it
// held (+0x178) to the list of the object it now holds (+0x24d0), each reached through the holder's +0x10.
// Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkBallListenersTag_l948124;
using UnkBallListeners_l948124 = base::UnkSList<UnkBallListenersTag_l948124>;
using UnkBallListenerNode_l948124 = UnkBallListeners_l948124::Node;

using UnkBallListenerNode_l948124 = UnkBallListeners_l948124::Node;

static inline s32 IndexOf(UnkBallListeners_l948124* list, u64 value) {
    u32 i = 0;
    for (UnkBallListenerNode_l948124* n = list->head; n; n = n->next, ++i) {
        if (n->value == value)
            return static_cast<s32>(i);
    }
    return -1;
}

static inline UnkBallListenerNode_l948124* Find(UnkBallListeners_l948124* list, u64 value) {
    for (UnkBallListenerNode_l948124* n = list->head; n; n = n->next) {
        if (n->value == value)
            return n;
    }
    return nullptr;
}

static inline void Erase(UnkBallListeners_l948124* list, UnkBallListenerNode_l948124* node) {
    UnkBallListenerNode_l948124* prev = nullptr;
    UnkBallListenerNode_l948124* it = list->head;
    while (it) {
        UnkBallListenerNode_l948124* cur = it;
        UnkBallListenerNode_l948124* next = it->next;
        it = next;
        if (cur != node) {
            prev = cur;
            continue;
        }
        if (prev) {
            prev->next = next;
            if (list->tail == node)
                list->tail = prev;
        } else {
            list->head = next;
        }
        if (--list->count == 0) {
            list->head = nullptr;
            list->tail = nullptr;
        }
        list->allocator->vf_0x48(node);
        return;
    }
}

struct UnkBallListOwner_l948124 {
    u8 pad_0x0[0x128];
    UnkBallListeners_l948124 listeners;  // 0x128
};

struct UnkBallHolder_l948124 {
    u8 pad_0x0[0x10];
    UnkBallListOwner_l948124* owner;  // 0x10
};

struct UnkBallMovement_l948124 {
    u8 pad_0x0[0x178];
    UnkBallHolder_l948124* previous;  // 0x178
    u8 pad_0x180[0x20f8];
    u8 listener[0x30];  // 0x2278
    u8 pad_0x22a8[0x228];
    UnkBallHolder_l948124* current;  // 0x24d0
};
static_assert(__builtin_offsetof(UnkBallMovement_l948124, current) == 0x24d0, "layout");

void fn_7100948124(UnkBallMovement_l948124* self) {
    if (UnkBallHolder_l948124* holder = self->previous) {
        if (UnkBallListOwner_l948124* owner = holder->owner) {
            UnkBallListeners_l948124* list = &owner->listeners;
            u64 key = reinterpret_cast<u64>(self->listener);
            if (IndexOf(list, key) >= 0) {
                if (UnkBallListenerNode_l948124* node = Find(list, key))
                    Erase(list, node);
            }
        }
    }
    if (UnkBallHolder_l948124* holder = self->current) {
        if (UnkBallListOwner_l948124* owner = holder->owner) {
            UnkBallListeners_l948124* list = &owner->listeners;
            u64 key = reinterpret_cast<u64>(self->listener);
            if (IndexOf(list, key) < 0)
                list->PushBack(key);
        }
    }
}
