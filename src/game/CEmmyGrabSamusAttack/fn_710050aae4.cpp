// CEmmyGrabSamusAttack's unit: the attack's message handler. On message 0xf4061ff25a2163d6 of kind 1 it
// moves the grab's state machine on from the state it is in. Names and layouts are placeholders.
#include "game/UnkStateMachineFind.h"
#include "types.h"

struct UnkGrabState_l50aae4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The states are only 4-aligned after the count.
typedef UnkGrabState_l50aae4* __attribute__((aligned(4))) UnkGrabStatePtr_l50aae4;

// The grab's state machine.
struct UnkGrabStateMachine_l50aae4 {
    static const s32 kNone = 0xe;

    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkGrabState_l50aae4* previous, UnkGrabState_l50aae4* next);  // after entering
    virtual void vf_0x28(UnkGrabState_l50aae4* current, UnkGrabState_l50aae4* next);   // after leaving

    u64 field_0x8;
    UnkGrabState_l50aae4* current;
    UnkGrabState_l50aae4* previous;
    u32 count;
    u8 storage[16 * sizeof(UnkGrabStatePtr_l50aae4)];
    s32 pending;
    bool field_0xa8;

    UnkGrabStatePtr_l50aae4* items() { return reinterpret_cast<UnkGrabStatePtr_l50aae4*>(storage); }

    UnkGrabState_l50aae4* Find(s32 id) {
        return FindStateById(items(), count, id);
    }

    void Request(s32 id) {
        SetPending(id);
        if (field_0xa8)
            return;
        UnkGrabState_l50aae4* cur = current;
        if (!cur || cur->id != id) {
            if (UnkGrabState_l50aae4* next = Find(id)) {
                if (cur) {
                    cur->vf_0x18(id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                next->vf_0x10(previous ? previous->id : kNone);
                vf_0x20(previous, next);
            }
        }
        SetPending(kNone);
    }
    void SetPending(s32 id) {
        s32* slot = &pending;
        *slot = id;
    }
};
static_assert(__builtin_offsetof(UnkGrabStateMachine_l50aae4, pending) == 0xa4, "layout");

struct UnkEmmyGrabAttack_l50aae4 {
    u8 pad_0x0[0x28];
    UnkGrabStateMachine_l50aae4 state_machine;  // 0x28
};

struct UnkGrabSm_b61;
void fn_710050add4(UnkGrabSm_b61* self);

struct UnkGrabMessageType_l50aae4 {
    u8 pad_0x0[0x30];
    u64 id;
};
struct UnkGrabMessage_l50aae4 {
    u32 kind;
    u8 pad_0x4[0xc];
    UnkGrabMessageType_l50aae4* type;
};

bool fn_710050aae4(UnkEmmyGrabAttack_l50aae4* self, const UnkGrabMessage_l50aae4* message) {
    if (message->type->id != 0xf4061ff25a2163d6ull || message->kind != 1)
        return true;
    UnkGrabStateMachine_l50aae4* sm = &self->state_machine;
    if (!sm->current)
        return true;
    switch (sm->current->id) {
    case 3:
        sm->Request(9);
        break;
    case 4:
        sm->Request(5);
        break;
    case 5:
        sm->Request(9);
        break;
    case 6:
        sm->Request(10);
        break;
    case 8:
        sm->Request(10);
        break;
    case 9:
        sm->Request(10);
        break;
    case 10:
        fn_710050add4(reinterpret_cast<UnkGrabSm_b61*>(self));
        break;
    }
    return true;
}
