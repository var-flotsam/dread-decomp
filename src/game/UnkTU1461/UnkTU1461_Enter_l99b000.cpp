// Unit 1461 (tools/tu_map.py, near CSamusGrappleMovementState): after the base step, tells the owner
// g_unk_710164b368 (fn_7100d7c94c), requests state 1 from the state machine at +0x188 (none is 5), then drops
// the reference held at +0xb18 and clears +0xb88. Names and layouts are placeholders.
#include "game/CSamusMovementState/CParams.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU2523.h"
#include "types.h"

extern const u64 g_unk_710164b368;

struct UnkGrappleState_l99b000 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};
typedef UnkGrappleState_l99b000* __attribute__((aligned(4))) UnkGrappleStatePtr_l99b000;

struct UnkGrappleMachine_l99b000 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkGrappleState_l99b000* previous, UnkGrappleState_l99b000* next);
    virtual void vf_0x28(UnkGrappleState_l99b000* current, UnkGrappleState_l99b000* next);
    u64 field_0x8;
    UnkGrappleState_l99b000* current;
    UnkGrappleState_l99b000* previous;
    u32 count;
    UnkGrappleStatePtr_l99b000 states[16];
    u32 pending;
    bool busy;
};

struct UnkGrappleRef_l99b000 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // destroys it
    s32 refcount;
};

struct UnkGrappleMovement_l99b000 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x150];
    UnkGrappleMachine_l99b000 machine;  // 0x188
    u8 pad_0x238[0x8e0];
    UnkGrappleRef_l99b000* field_0xb18;
    u8 pad_0xb20[0x68];
    u64 field_0xb88;
};
static_assert(__builtin_offsetof(UnkGrappleMovement_l99b000, machine) == 0x188, "layout");
static_assert(__builtin_offsetof(UnkGrappleMachine_l99b000, pending) == 0xa4, "layout");
static_assert(__builtin_offsetof(UnkGrappleMovement_l99b000, field_0xb18) == 0xb18, "layout");
static_assert(__builtin_offsetof(UnkGrappleMovement_l99b000, field_0xb88) == 0xb88, "layout");

// Switches to state `id` and clears the pending id.
template <u32 None>
static inline void SwitchState_l99b000(UnkGrappleMachine_l99b000* self, u32 id, UnkGrappleState_l99b000* current) {
    if (!current || current->id != id) {
        UnkGrappleState_l99b000* found = FindStateById(self->states, self->count, id);
        if (found) {
            if (current) {
                current->vf_0x18(id);
                self->vf_0x28(self->current, found);
            }
            self->previous = self->current;
            self->current = found;
            found->vf_0x10(self->previous ? self->previous->id : None);
            self->vf_0x20(self->previous, found);
        }
    }
    self->pending = None;
}

// Asks for state `id` unless the machine is in it already; switches now unless the machine is busy.
template <u32 None>
static inline void RequestState_l99b000(UnkGrappleMachine_l99b000* self, u32 id) {
    UnkGrappleState_l99b000* current = self->current;
    if (current && current->id == id)
        return;
    self->pending = id;
    if (self->busy)
        return;
    SwitchState_l99b000<None>(self, id, current);
}

void fn_710099b000(UnkGrappleMovement_l99b000* self, void* arg) {
    fn_71009b0ce4(reinterpret_cast<CSamusAirMovementState*>(self), arg);
    fn_7100d7c94c(self->owner, &g_unk_710164b368);
    RequestState_l99b000<5>(&self->machine, 1);
    UnkGrappleRef_l99b000* ref = self->field_0xb18;
    if (ref) {
        if (--ref->refcount == 0)
            ref->vf_0x10();
        self->field_0xb18 = nullptr;
    }
    self->field_0xb88 = 0;
}
