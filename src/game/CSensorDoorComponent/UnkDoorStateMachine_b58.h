#pragma once

// A door's state machine as src/game/CDoorLifeComponent.cpp sees it (its RequestState, inlined), and
// the "opened" send with mode 2 and a local block {-1, 1.0f, 1} in place of UnkSpitSend's first
// descriptor (as src/game/CQuarentineDoorComponent/CQuarentineDoorComponent_Send_b54.cpp). Used by
// CSensorDoorComponent and CQuarentineDoorComponent. Names and layouts are placeholders.

#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "types.h"

struct UnkDoorState_b58 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The states are only 4-aligned after the count.
typedef UnkDoorState_b58* __attribute__((aligned(4))) UnkDoorStatePtr_b58;

struct UnkDoorStateMachine_b58 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkDoorState_b58* previous, UnkDoorState_b58* next);  // after entering
    virtual void vf_0x28(UnkDoorState_b58* current, UnkDoorState_b58* next);   // after leaving

    u64 field_0x8;
    UnkDoorState_b58* current;
    UnkDoorState_b58* previous;
    u32 count;
    UnkDoorStatePtr_b58 states[16];
    s32 pending;
    bool field_0xa8;
};

// The original walks the states with a byte count it tests for zero after the shift (the barrier keeps
// the compiler from testing the count instead) and steps the pointer before the count.
static inline UnkDoorState_b58* FindState_b58(UnkDoorStateMachine_b58* self, s32 id) {
    u64 n = (u64)self->count * sizeof(UnkDoorStatePtr_b58);
    asm("" : "+r"(n));
    for (UnkDoorStatePtr_b58* p = self->states; n != 0; ++p, n -= sizeof(UnkDoorStatePtr_b58)) {
        if ((*p)->id == id)
            return *p;
    }
    return nullptr;
}

static inline void RequestState_b58(UnkDoorStateMachine_b58* self, s32 id) {
    self->pending = id;
    if (self->field_0xa8)
        return;
    UnkDoorState_b58* current = self->current;
    if (!current || current->id != id) {
        if (UnkDoorState_b58* next = FindState_b58(self, id)) {
            if (current) {
                current->vf_0x18(id);
                self->vf_0x28(self->current, next);
            }
            // Stored previous first and reread: a local `prev` lets the compiler split the path.
            self->previous = self->current;
            self->current = next;
            next->vf_0x10(self->previous ? self->previous->id : 0x7fffffff);
            self->vf_0x20(self->previous, next);
        }
    }
    self->pending = 0x7fffffff;
}

struct UnkDoorSendParams_b58 {
    s64 field_0x0;
    f32 field_0x8;
    u32 field_0xc;
};

static inline void SendOpened_b58(void* target) {
    u64 hash = base::global::CName("opened").hash;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 2;
    UnkDoorSendParams_b58 params;
    params.field_0x0 = -1;
    params.field_0x8 = 1.0f;
    params.field_0xc = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &params, &g_unk_7172769740, &mask);
}
