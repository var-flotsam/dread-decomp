// CEmmyGrabSamusAttack (unit 800): sets up the grab's state machine. Registers its thirteen states
// (each once, by id) and binds their enter, update and exit callbacks to the attack, then changes
// to state 1. Names and layouts are placeholders.
#include "types.h"

struct UnkGrabAttack_ovb0014;
struct UnkGrabTick_b54;
void CEmmyGrabSamusAttack_ResetTimersAndSendKindAnim(UnkGrabAttack_ovb0014* self);
void fn_7100511324(UnkGrabTick_b54* self, f32 dt);
void fn_710051109c();
void fn_71005110a0();
void fn_71005110a4();
void fn_71005110a8();
void fn_71005110ac();
void fn_71005110b0();
void fn_71005110b4();
void fn_71005110b8();
void fn_71005110bc();
void fn_71005110c0();
void fn_71005110c4();
void fn_71005110c8();
void fn_71005110cc();
void fn_71005110d0();
void fn_71005110d4();
void fn_7100511160();
void fn_7100511164();
void fn_7100511168();
void fn_710051116c();
void fn_7100511170();
void fn_7100511174();
void fn_7100511178();
void fn_710051117c();
void fn_7100511180();
void fn_7100511184();
void fn_7100511188();
void fn_7100511250();
void fn_7100511254();
void fn_7100511258();
void fn_7100511320();

using UnkCallback_505b60 = void (*)();

struct UnkDelegate_505b60 {
    void* object;
    UnkCallback_505b60 function;
};

struct UnkGrabStateMachine_505b60;

struct UnkGrabState_505b60 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(u32 previous_id);
    virtual void Exit(u32 next_id);

    UnkGrabStateMachine_505b60* machine;
    u32 id;
    u8 unk_0x14[0x1c];
    UnkDelegate_505b60 on_enter;   // 0x30
    UnkDelegate_505b60 on_update;  // 0x40
    UnkDelegate_505b60 on_exit;    // 0x50
    u8 unk_0x60[0x10];
};
static_assert(sizeof(UnkGrabState_505b60) == 0x70, "layout");

// The id that names no state.
constexpr u32 kNoState_505b60 = 14;

struct UnkGrabStateMachine_505b60 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEntered(UnkGrabState_505b60* previous, UnkGrabState_505b60* next);
    virtual void OnLeaving(UnkGrabState_505b60* current, UnkGrabState_505b60* next);

    void* owner;
    UnkGrabState_505b60* current;   // 0x10
    UnkGrabState_505b60* previous;  // 0x18
    u32 count;
    u32 states[0x20];  // UnkGrabState_505b60* entries, 4-byte aligned
    u32 pending;       // 0xa4
    bool busy;         // 0xa8

    UnkGrabState_505b60** States() { return reinterpret_cast<UnkGrabState_505b60**>(states); }

    void Add(UnkGrabState_505b60* state) {
        UnkGrabState_505b60** begin = States();
        UnkGrabState_505b60** end = begin + count;
        for (UnkGrabState_505b60** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }

    UnkGrabState_505b60* Find(u32 id) {
        UnkGrabState_505b60** begin = States();
        UnkGrabState_505b60** end = begin + count;
        for (UnkGrabState_505b60** p = begin; p != end; ++p)
            if ((*p)->id == id)
                return *p;
        return nullptr;
    }

    void ChangeState(u32 id) {
        pending = id;
        if (busy)
            return;
        if (!current || current->id != id) {
            if (UnkGrabState_505b60* next = Find(id)) {
                if (current) {
                    current->Exit(id);
                    OnLeaving(current, next);
                }
                previous = current;
                current = next;
                next->Enter(previous ? previous->id : kNoState_505b60);
                OnEntered(previous, next);
            }
        }
        pending = kNoState_505b60;
    }
};
static_assert(__builtin_offsetof(UnkGrabStateMachine_505b60, pending) == 0xa4, "layout");

struct UnkGrabAttack_505b60 {
    u8 unk_0x0[0x28];
    UnkGrabStateMachine_505b60 machine;  // 0x28
    u8 unk_0xd8[0x28];
    UnkGrabState_505b60 states[13];  // 0x100
};
static_assert(__builtin_offsetof(UnkGrabAttack_505b60, states) == 0x100, "layout");

void fn_7100505b60(UnkGrabAttack_505b60* self) {
    self->machine.owner = self;
    self->machine.Add(&self->states[0]);
    self->states[0].on_enter = {self, reinterpret_cast<UnkCallback_505b60>(&CEmmyGrabSamusAttack_ResetTimersAndSendKindAnim)};
    self->machine.Add(&self->states[1]);
    self->states[1].on_enter = {self, fn_710051109c};
    self->states[1].on_exit = {self, fn_71005110a0};
    self->machine.Add(&self->states[2]);
    self->states[2].on_enter = {self, fn_71005110a4};
    self->states[2].on_exit = {self, fn_71005110a8};
    self->machine.Add(&self->states[3]);
    self->states[3].on_enter = {self, fn_71005110ac};
    self->states[3].on_update = {self, fn_71005110b0};
    self->states[3].on_exit = {self, fn_71005110b4};
    self->machine.Add(&self->states[4]);
    self->states[4].on_enter = {self, fn_71005110b8};
    self->states[4].on_update = {self, fn_71005110bc};
    self->states[4].on_exit = {self, fn_71005110c0};
    self->machine.Add(&self->states[5]);
    self->states[5].on_enter = {self, fn_71005110c4};
    self->states[5].on_update = {self, fn_71005110c8};
    self->states[5].on_exit = {self, fn_71005110cc};
    self->machine.Add(&self->states[6]);
    self->states[6].on_enter = {self, fn_71005110d0};
    self->states[6].on_exit = {self, fn_71005110d4};
    self->machine.Add(&self->states[7]);
    self->states[7].on_enter = {self, fn_7100511160};
    self->states[7].on_exit = {self, fn_7100511164};
    self->machine.Add(&self->states[8]);
    self->states[8].on_enter = {self, fn_7100511168};
    self->states[8].on_update = {self, fn_710051116c};
    self->states[8].on_exit = {self, fn_7100511170};
    self->machine.Add(&self->states[9]);
    self->states[9].on_enter = {self, fn_7100511174};
    self->states[9].on_exit = {self, fn_7100511178};
    self->states[9].on_update = {self, fn_710051117c};
    self->machine.Add(&self->states[10]);
    self->states[10].on_enter = {self, fn_7100511180};
    self->states[10].on_update = {self, fn_7100511184};
    self->states[10].on_exit = {self, fn_7100511188};
    self->machine.Add(&self->states[11]);
    self->states[11].on_enter = {self, fn_7100511250};
    self->states[11].on_update = {self, fn_7100511254};
    self->states[11].on_exit = {self, fn_7100511258};
    self->machine.Add(&self->states[12]);
    self->states[12].on_enter = {self, fn_7100511320};
    self->states[12].on_exit = {self, reinterpret_cast<UnkCallback_505b60>(&fn_7100511324)};
    self->machine.ChangeState(1);
}
