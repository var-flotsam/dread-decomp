// CTriggerComponent's unit: moves the component's key (+0x30) from the previous target's list to the
// new target's, points the slot that held the previous target at the new one and copies the new
// target's name. Names and layouts are placeholders.
#include <algorithm>

#include "base/UnkSList.h"
#include "base/global/CStrId.h"
#include "game/CTriggerComponent.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkTriggerListTag_la601cc;
using UnkTriggerList_la601cc = base::UnkSList<UnkTriggerListTag_la601cc>;

struct UnkTriggerTarget_la601cc : UnkWeakTarget {
    u8 pad_0x8[0x28];
    CStrId name;  // 0x30
    u8 pad_0x38[0x2e0];
    UnkTriggerList_la601cc listeners;  // 0x318
};
static_assert(__builtin_offsetof(UnkTriggerTarget_la601cc, listeners) == 0x318, "layout");

// A slot holding a target's weak handle (4-byte aligned in its array).
struct __attribute__((packed, aligned(4))) UnkTriggerSlot_la601cc {
    UnkWeakHandle* handle;
    u8 pad_0x8[0x10];
};
static_assert(sizeof(UnkTriggerSlot_la601cc) == 0x18, "layout");

struct UnkTriggerComponent_la601cc {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
    u8 pad_0x38[0x28];
    u32 slot_count;                       // 0x60
    u8 slots[16 * sizeof(UnkTriggerSlot_la601cc)];  // 0x64
    u8 pad_0x1e4[4];

    UnkTriggerSlot_la601cc* begin() { return reinterpret_cast<UnkTriggerSlot_la601cc*>(slots); }
    UnkTriggerSlot_la601cc* end() { return begin() + slot_count; }
    CStrId name;  // 0x1e8
};
static_assert(__builtin_offsetof(UnkTriggerComponent_la601cc, name) == 0x1e8, "layout");

static inline UnkTriggerList_la601cc::Node* Find(UnkTriggerList_la601cc* list, u64 v) {
    for (UnkTriggerList_la601cc::Node* n = list->head; n; n = n->next)
        if (n->value == v)
            return n;
    return nullptr;
}

static inline void Erase(UnkTriggerList_la601cc* list, UnkTriggerList_la601cc::Node* pos) {
    UnkTriggerList_la601cc::Node* prev = nullptr;
    for (UnkTriggerList_la601cc::Node* it = list->head; it;) {
        UnkTriggerList_la601cc::Node* next = it->next;
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

struct UnkRelocateActor;

void fn_7100a601cc(void* self_, UnkRelocateActor* target_, UnkRelocateActor* previous_) {
    auto* self = static_cast<UnkTriggerComponent_la601cc*>(self_);
    auto* target = reinterpret_cast<UnkTriggerTarget_la601cc*>(target_);
    auto* previous = reinterpret_cast<UnkTriggerTarget_la601cc*>(previous_);
    UnkTriggerSlot_la601cc* slot = std::find_if(self->begin(), self->end(), [previous](const UnkTriggerSlot_la601cc& s) {
        return s.handle && s.handle->target == previous;
    });
    if (UnkTriggerList_la601cc::Node* node = Find(&previous->listeners, self->key))
        Erase(&previous->listeners, node);
    target->listeners.PushBack(self->key);
    AssignRef(&slot->handle, fn_7100d78378(target));
    self->name = target->name;
}
