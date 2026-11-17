// CGrappleBeamComponent unit (tools/tu_map.py, unit 1277): sets up the beam's state machine (registers its four
// states once by id and binds their callbacks to the component), then enters state 1 unless entry is held off.
// Names and layouts are placeholders.
#include "game/UnkStateMachineFind.h"
#include "types.h"

void fn_7100821ce8();
void fn_7100821d6c();
void fn_7100821d70();
void fn_7100821d74();
void fn_7100821d78();
void fn_7100821d7c();
void fn_7100821d94();
void fn_7100821df8();
void fn_7100821dfc();
void fn_7100821e08();

struct UnkGrappleMachine_l818420;

struct UnkGrappleDelegate_l818420 {
    void* object;
    void (*function)();
};

struct UnkGrappleState_l818420 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(u32 previous_id);
    virtual void Exit(u32 next_id);

    UnkGrappleMachine_l818420* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkGrappleDelegate_l818420 field_0x30;
    UnkGrappleDelegate_l818420 field_0x40;
    UnkGrappleDelegate_l818420 field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkGrappleState_l818420) == 0x70, "layout");

struct UnkGrappleMachine_l818420 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEntered(UnkGrappleState_l818420* previous, UnkGrappleState_l818420* next);
    virtual void OnLeaving(UnkGrappleState_l818420* current, UnkGrappleState_l818420* next);

    void* owner;
    UnkGrappleState_l818420* current;
    UnkGrappleState_l818420* previous;
    u32 count;
    u32 states[0x20];  // UnkGrappleState_l818420* entries, 4-byte aligned
    u32 field_0xa4;    // set by the owner around entering its first state

    UnkGrappleState_l818420** States() { return reinterpret_cast<UnkGrappleState_l818420**>(states); }

    // Registers `state` unless one with its id is registered already.
    void Add(UnkGrappleState_l818420* state) {
        UnkGrappleState_l818420** begin = States();
        UnkGrappleState_l818420** end = begin + count;
        for (UnkGrappleState_l818420** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }

    // The state with id `state_id`, or null.
    UnkGrappleState_l818420* Find(u32 state_id) {
        return FindStateById(States(), count, state_id);
    }

    void ChangeState(u32 state_id) {
        if (current && current->id == state_id)
            return;
        UnkGrappleState_l818420* next = Find(state_id);
        if (!next)
            return;
        if (current) {
            current->Exit(state_id);
            OnLeaving(current, next);
        }
        previous = current;
        current = next;
        next->Enter(previous ? previous->id : 5);
        OnEntered(previous, next);
    }
};
static_assert(sizeof(UnkGrappleMachine_l818420) == 0xa8, "layout");

struct UnkGrappleBeam_l818420 {
    u8 pad_0x0[0x480];
    UnkGrappleMachine_l818420 machine;      // 0x480
    bool hold_state;                        // 0x528
    u8 pad_0x529[0x550 - 0x529];
    UnkGrappleState_l818420 states[4];      // 0x550
};
static_assert(__builtin_offsetof(UnkGrappleBeam_l818420, hold_state) == 0x528, "layout");
static_assert(__builtin_offsetof(UnkGrappleBeam_l818420, states) == 0x550, "layout");

static inline void Bind(UnkGrappleBeam_l818420* self, UnkGrappleState_l818420* state, void (*a)(), void (*b)(),
                        void (*c)()) {
    self->machine.Add(state);
    state->field_0x30 = {self, a};
    state->field_0x40 = {self, b};
    state->field_0x50 = {self, c};
}

void fn_7100818420(UnkGrappleBeam_l818420* self) {
    self->machine.owner = self;
    self->machine.Add(&self->states[0]);
    self->states[0].field_0x30 = {self, fn_7100821ce8};
    self->states[0].field_0x40 = {self, fn_7100821d6c};
    Bind(self, &self->states[1], fn_7100821d70, fn_7100821d74, fn_7100821d78);
    Bind(self, &self->states[2], fn_7100821d7c, fn_7100821d94, fn_7100821df8);
    self->machine.Add(&self->states[3]);
    self->states[3].field_0x30 = {self, fn_7100821dfc};
    self->states[3].field_0x50 = {self, fn_7100821e08};

    self->machine.field_0xa4 = 1;
    if (!self->hold_state) {
        self->machine.ChangeState(1);
        self->machine.field_0xa4 = 5;
    }
}
