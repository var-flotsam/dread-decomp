#pragma once

// The E.M.M.I.'s grab attack (CEmmyGrabSamusAttack's unit, tools/tu_map.py unit 800), as far as the
// hand-written sources know it: one layout, grown by each source that reads a member it does not have yet.
// Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkStateMachineFind.h"
#include "types.h"

struct UnkGrabState_l507d24 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The grab's state machine; its states are only 4-aligned after the count.
struct UnkGrabStateMachine_l507d24 {
    static const s32 kNone = 0xe;

    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkGrabState_l507d24* previous, UnkGrabState_l507d24* next);  // after entering
    virtual void vf_0x28(UnkGrabState_l507d24* current, UnkGrabState_l507d24* next);   // after leaving

    u64 field_0x8;
    UnkGrabState_l507d24* current;
    UnkGrabState_l507d24* previous;
    u32 count;
    u8 storage[16 * sizeof(UnkGrabState_l507d24*)];
    s32 pending;
    bool field_0xa8;

    UnkGrabState_l507d24** items() { return reinterpret_cast<UnkGrabState_l507d24**>(storage); }

    UnkGrabState_l507d24* Find(s32 id) {
        return FindStateById(items(), count, id);
    }

    void Request(s32 id) {
        pending = id;
        if (field_0xa8)
            return;
        UnkGrabState_l507d24* cur = current;
        if (!cur || cur->id != id) {
            if (UnkGrabState_l507d24* next = Find(id)) {
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
        pending = kNone;
    }
};
static_assert(__builtin_offsetof(UnkGrabStateMachine_l507d24, pending) == 0xa4, "layout");

struct UnkGrabAttack_b58a3 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x18];
    UnkGrabStateMachine_l507d24 state_machine;  // 0x28
    u8 pad_0xd8[0x5d8];
    bool field_0x6b0;
};
static_assert(__builtin_offsetof(UnkGrabAttack_b58a3, field_0x6b0) == 0x6b0, "layout");
