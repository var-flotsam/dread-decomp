// CChozoWarriorXUltimateGrabAttack's unit (tools/tu_map.py): sets up the attack's state machine
// (+0x80): registers its twelve states (each once, by id), binds each state's callbacks to the
// attack, then switches to state 1 unless the machine is locked. Names and layouts are placeholders.
#include "game/UnkStateMachineFind.h"
#include "types.h"

struct UnkEffect;
struct UnkGrabTimer_A2_b65;
template <u32 Shift>
struct UnkGrabAttack_b64a2;

void fn_710048c13c();
void fn_710048c140();
void fn_710048c144(UnkGrabTimer_A2_b65* self, f32 dt);
void fn_710048c254();
void fn_710048c258();
void fn_710048c288();
void fn_710048c28c();
void fn_710048c29c();
void fn_710048c2f0();
void fn_710048c2f4();
void fn_710048c304();
void fn_710048c33c();
void fn_710048c340();
void fn_710048c350();
void fn_710048c380();
void fn_710048c384(UnkEffect* self);
void fn_710048c430();
void fn_710048c440();
void fn_710048c4b4();
void fn_710048c4b8();
void fn_710048c4c8();
void fn_710048c51c();
void fn_710048c520();
void fn_710048c530();
void fn_710048c568();
void fn_710048c56c();
void fn_710048c57c();
void fn_710048c5cc();
void fn_710048c5d0();
void fn_710048c5d4(UnkGrabAttack_b64a2<8>* self);
void fn_710048c674();

struct UnkUltGrabMachine_486de8;

struct UnkUltGrabDelegate_486de8 {
    void* object;
    void (*function)();
};

struct UnkUltGrabState_486de8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(u32 previous_id);
    virtual void Exit(u32 next_id);

    UnkUltGrabMachine_486de8* machine;
    u32 id;
    u8 pad_0x14[0x1c];
    UnkUltGrabDelegate_486de8 on_enter;   // 0x30
    UnkUltGrabDelegate_486de8 on_update;  // 0x40
    UnkUltGrabDelegate_486de8 on_exit;    // 0x50
    u8 pad_0x60[0x10];
};
static_assert(sizeof(UnkUltGrabState_486de8) == 0x70, "layout");

struct UnkUltGrabMachineVtable_486de8 {
    void* pad_0x0[4];
    void (*on_entered)(UnkUltGrabMachine_486de8* self, UnkUltGrabState_486de8* previous,
                       UnkUltGrabState_486de8* state);  // 0x20
    void (*on_leaving)(UnkUltGrabMachine_486de8* self, UnkUltGrabState_486de8* current,
                       UnkUltGrabState_486de8* state);  // 0x28
};

struct UnkUltGrabMachine_486de8 {
    UnkUltGrabMachineVtable_486de8* vtable;
    void* owner;
    UnkUltGrabState_486de8* current;
    UnkUltGrabState_486de8* previous;
    u32 count;
    u32 states[0x20];  // UnkUltGrabState_486de8* entries, 4-byte aligned
    u32 next_id;       // 0xa4
    bool locked;       // 0xa8

    UnkUltGrabState_486de8** States() { return reinterpret_cast<UnkUltGrabState_486de8**>(states); }

    void Add(UnkUltGrabState_486de8* state) {
        UnkUltGrabState_486de8** begin = States();
        UnkUltGrabState_486de8** end = begin + count;
        for (UnkUltGrabState_486de8** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }

    UnkUltGrabState_486de8* Find(u32 id) {
        return FindStateById(States(), count, id);
    }

    void ChangeState(u32 id) {
        next_id = id;
        if (locked)
            return;
        if (!current || current->id != id) {
            if (UnkUltGrabState_486de8* state = Find(id)) {
                if (current) {
                    current->Exit(id);
                    vtable->on_leaving(this, current, state);
                }
                previous = current;
                current = state;
                state->Enter(previous ? previous->id : 0x7fffffff);
                vtable->on_entered(this, previous, state);
            }
        }
        next_id = 0x7fffffff;
    }
};
static_assert(__builtin_offsetof(UnkUltGrabMachine_486de8, next_id) == 0xa4, "layout");

struct UnkUltGrab_486de8 {
    u8 pad_0x0[0x80];
    UnkUltGrabMachine_486de8 machine;  // 0x80
    u8 pad_0x130[0x18];
    UnkUltGrabState_486de8 states[12];  // 0x148
};
static_assert(__builtin_offsetof(UnkUltGrab_486de8, states) == 0x148, "layout");

static inline void Bind(UnkUltGrab_486de8* self, UnkUltGrabState_486de8* state, void (*enter)(),
                        void (*update)(), void (*exit)()) {
    self->machine.Add(state);
    state->on_enter = {self, enter};
    state->on_update = {self, update};
    state->on_exit = {self, exit};
}

static inline void BindEnterExit(UnkUltGrab_486de8* self, UnkUltGrabState_486de8* state, void (*enter)(),
                                 void (*exit)()) {
    self->machine.Add(state);
    state->on_enter = {self, enter};
    state->on_exit = {self, exit};
}

void fn_7100486de8(UnkUltGrab_486de8* self) {
    self->machine.owner = self;
    self->machine.Add(&self->states[0]);
    self->states[0].on_enter = {self, fn_710048c13c};
    self->machine.Add(&self->states[1]);
    self->states[1].on_enter = {self, fn_710048c140};
    self->states[1].on_exit = {self, reinterpret_cast<void (*)()>(fn_710048c144)};
    self->states[1].on_update = {self, fn_710048c254};
    Bind(self, &self->states[2], fn_710048c258, fn_710048c288, fn_710048c28c);
    Bind(self, &self->states[3], fn_710048c29c, fn_710048c2f0, fn_710048c2f4);
    Bind(self, &self->states[4], fn_710048c304, fn_710048c33c, fn_710048c340);
    BindEnterExit(self, &self->states[5], fn_710048c350, fn_710048c380);
    BindEnterExit(self, &self->states[6], reinterpret_cast<void (*)()>(fn_710048c384), fn_710048c430);
    Bind(self, &self->states[7], fn_710048c440, fn_710048c4b4, fn_710048c4b8);
    Bind(self, &self->states[8], fn_710048c4c8, fn_710048c51c, fn_710048c520);
    Bind(self, &self->states[9], fn_710048c530, fn_710048c568, fn_710048c56c);
    Bind(self, &self->states[10], fn_710048c57c, fn_710048c5cc, fn_710048c5d0);
    BindEnterExit(self, &self->states[11], reinterpret_cast<void (*)()>(fn_710048c5d4), fn_710048c674);
    self->machine.ChangeState(1);
}
