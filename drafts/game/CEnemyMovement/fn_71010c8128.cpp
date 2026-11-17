// CEnemyMovement unit: runs the base reset, clears two fields, asks the movement's state machine (+0x700) for
// state 1 and unregisters the listener part (+0x6e8) from the ledge it holds (+0x178). Names and layouts are
// placeholders.
#include "base/UnkSList.h"
#include "game/CComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkMoveListenersTag_l0c8128;
using UnkMoveListeners_l0c8128 = base::UnkSList<UnkMoveListenersTag_l0c8128>;
using UnkMoveListenerNode_l0c8128 = UnkMoveListeners_l0c8128::Node;

static inline s32 IndexOf(UnkMoveListeners_l0c8128* list, u64 value) {
    u32 i = 0;
    for (UnkMoveListenerNode_l0c8128* n = list->head; n; n = n->next, ++i) {
        if (n->value == value)
            return static_cast<s32>(i);
    }
    return -1;
}

static inline UnkMoveListenerNode_l0c8128* Find(UnkMoveListeners_l0c8128* list, u64 value) {
    for (UnkMoveListenerNode_l0c8128* n = list->head; n; n = n->next) {
        if (n->value == value)
            return n;
    }
    return nullptr;
}

static inline void Erase(UnkMoveListeners_l0c8128* list, UnkMoveListenerNode_l0c8128* node) {
    UnkMoveListenerNode_l0c8128* prev = nullptr;
    UnkMoveListenerNode_l0c8128* it = list->head;
    while (it) {
        UnkMoveListenerNode_l0c8128* cur = it;
        UnkMoveListenerNode_l0c8128* next = it->next;
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

struct UnkMoveLedgeOwner_l0c8128 {
    u8 pad_0x0[0x128];
    UnkMoveListeners_l0c8128 listeners;  // 0x128
};

struct UnkMoveLedgeRef_l0c8128 : UnkRefObject {
    UnkMoveLedgeOwner_l0c8128* owner;  // 0x10
};

struct UnkMoveState_l0c8128 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

struct UnkMoveStateMachine_l0c8128 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkMoveState_l0c8128* previous, UnkMoveState_l0c8128* next);  // after entering
    virtual void vf_0x28(UnkMoveState_l0c8128* current, UnkMoveState_l0c8128* next);   // after leaving

    u64 field_0x8;
    UnkMoveState_l0c8128* current;   // 0x10
    UnkMoveState_l0c8128* previous;  // 0x18
    u32 count;                        // 0x20
    u8 storage[16 * sizeof(UnkMoveState_l0c8128*)];  // 0x24, the states (only 4-aligned after the count)
    s32 pending;                      // 0xa4
    bool field_0xa8;

    UnkMoveState_l0c8128** states() { return reinterpret_cast<UnkMoveState_l0c8128**>(storage); }

    UnkMoveState_l0c8128* FindState(s32 state_id) {
        UnkMoveState_l0c8128** p = states();
        for (u64 size = (u64)count * sizeof(*p); size != 0; ++p, size -= sizeof(*p)) {
            if ((*p)->id == state_id)
                return *p;
        }
        return nullptr;
    }

    void RequestState(s32 state_id) {
        pending = state_id;
        if (field_0xa8)
            return;
        UnkMoveState_l0c8128* cur = current;
        if (!cur || cur->id != state_id) {
            if (UnkMoveState_l0c8128* next = FindState(state_id)) {
                if (cur) {
                    cur->vf_0x18(state_id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                next->vf_0x10(previous ? previous->id : 0x7fffffff);
                vf_0x20(previous, next);
            }
        }
        pending = 0x7fffffff;
    }
};

struct UnkEnemyMovement_l0c8128 {
    u8 pad_0x0[0x178];
    UnkMoveLedgeRef_l0c8128* ledge;  // 0x178
    u8 pad_0x180[0x6e8 - 0x180];
    u8 listener[0x18];  // 0x6e8, the part registered with the ledge
    UnkMoveStateMachine_l0c8128 state_machine;  // 0x700
    u8 pad_0x7b0[0x1bd0 - 0x7b0];
    u64 field_0x1bd0;
    u64 field_0x1bd8;
};
static_assert(__builtin_offsetof(UnkEnemyMovement_l0c8128, state_machine) == 0x700, "layout");

void fn_71010c8128(UnkEnemyMovement_l0c8128* self) {
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
    self->field_0x1bd8 = 0;
    self->field_0x1bd0 = 0;
    self->state_machine.RequestState(1);
    UnkMoveLedgeRef_l0c8128* ledge = self->ledge;
    if (!ledge || !ledge->owner)
        return;
    UnkMoveListeners_l0c8128* list = &ledge->owner->listeners;
    u64 key = reinterpret_cast<u64>(self->listener);
    if (IndexOf(list, key) >= 0) {
        if (UnkMoveListenerNode_l0c8128* node = Find(list, key))
            Erase(list, node);
    }
}
