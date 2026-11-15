// CEnemyMovement unit: moves the movement's listener part (+0x6e8) from the ledge it holds (+0x178, a
// counted reference whose object at +0x10 keeps a singly linked list at +0x128) to the ledge in
// `slot`: unregisters it from the old list, then registers it with the new one unless already there.
// Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkLedgeListenersTag_l0cb838;
using UnkLedgeListeners_l0cb838 = base::UnkSList<UnkLedgeListenersTag_l0cb838>;
using UnkLedgeListenerNode_l0cb838 = UnkLedgeListeners_l0cb838::Node;

static inline s32 IndexOf(UnkLedgeListeners_l0cb838* list, u64 value) {
    u32 i = 0;
    for (UnkLedgeListenerNode_l0cb838* n = list->head; n; n = n->next, ++i) {
        if (n->value == value)
            return static_cast<s32>(i);
    }
    return -1;
}

static inline UnkLedgeListenerNode_l0cb838* Find(UnkLedgeListeners_l0cb838* list, u64 value) {
    for (UnkLedgeListenerNode_l0cb838* n = list->head; n; n = n->next) {
        if (n->value == value)
            return n;
    }
    return nullptr;
}

static inline void Erase(UnkLedgeListeners_l0cb838* list, UnkLedgeListenerNode_l0cb838* node) {
    UnkLedgeListenerNode_l0cb838* prev = nullptr;
    UnkLedgeListenerNode_l0cb838* it = list->head;
    while (it) {
        UnkLedgeListenerNode_l0cb838* cur = it;
        UnkLedgeListenerNode_l0cb838* next = it->next;
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

struct UnkLedgeOwner_l0cb838 {
    u8 pad_0x0[0x128];
    UnkLedgeListeners_l0cb838 listeners;  // 0x128
};

struct UnkLedgeRef_l0cb838 : UnkRefObject {
    UnkLedgeOwner_l0cb838* owner;  // 0x10
};

struct UnkEnemyMovement_l0cb838 {
    u8 pad_0x0[0x178];
    UnkLedgeRef_l0cb838* ledge;  // 0x178
    u8 pad_0x180[0x6e8 - 0x180];
    u8 listener[0x30];  // 0x6e8
};

void fn_71010cb838(UnkEnemyMovement_l0cb838* self, UnkLedgeRef_l0cb838** slot) {
    if (UnkLedgeRef_l0cb838* old = self->ledge) {
        if (UnkLedgeOwner_l0cb838* owner = old->owner) {
            UnkLedgeListeners_l0cb838* list = &owner->listeners;
            u64 key = reinterpret_cast<u64>(self->listener);
            if (IndexOf(list, key) >= 0) {
                if (UnkLedgeListenerNode_l0cb838* node = Find(list, key))
                    Erase(list, node);
            }
        }
    }
    if (UnkLedgeRef_l0cb838* ledge = *slot) {
        if (UnkLedgeOwner_l0cb838* owner = ledge->owner) {
            UnkLedgeListeners_l0cb838* list = &owner->listeners;
            u64 key = reinterpret_cast<u64>(self->listener);
            if (IndexOf(list, key) < 0)
                list->PushBack(key);
        }
    }
}
