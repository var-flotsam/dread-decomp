// Unit 2635: registers the eight states of a music-state holder on its state machine (+0x30), each once
// by id, then, unless held, switches the machine to state 1. Names and layouts are placeholders.
#include "types.h"

struct UnkMachine_lfcfe14;

struct UnkState_lfcfe14 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(s32 previousId);
    virtual void Exit(s32 nextId);
    UnkMachine_lfcfe14* machine;  // 0x8
    u32 id;                       // 0x10
    u8 pad_0x14[0x150 - 0x14];
};
static_assert(sizeof(UnkState_lfcfe14) == 0x150, "layout");

#pragma pack(push, 4)
struct UnkMachine_lfcfe14 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEnter(UnkState_lfcfe14* previous, UnkState_lfcfe14* next);
    virtual void OnExit(UnkState_lfcfe14* current, UnkState_lfcfe14* next);
    void* owner;                 // 0x8
    UnkState_lfcfe14* current;   // 0x10
    UnkState_lfcfe14* previous;  // 0x18
    u32 count;                   // 0x20
    u32 states[0x20];            // 0x24, UnkState_lfcfe14* entries, 4-byte aligned
    UnkState_lfcfe14** States() { return reinterpret_cast<UnkState_lfcfe14**>(states); }

    // Registers `state` unless one with its id is registered already.
    void Add(UnkState_lfcfe14* state) {
        UnkState_lfcfe14** begin = States();
        UnkState_lfcfe14** end = begin + count;
        for (UnkState_lfcfe14** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }

    UnkState_lfcfe14* Find(u32 id) {
        UnkState_lfcfe14** begin = States();
        UnkState_lfcfe14** end = begin + count;
        UnkState_lfcfe14* found = nullptr;
        for (UnkState_lfcfe14** p = begin; p != end; ++p) {
            if ((*p)->id == id) {
                found = *p;
                break;
            }
        }
        return found;
    }

    // Switches to the state with `id`, if it is registered and not current already.
    void SetState(u32 id) {
        if (current && current->id == id)
            return;
        UnkState_lfcfe14* next = Find(id);
        if (!next)
            return;
        if (current) {
            current->Exit(id);
            OnExit(current, next);
        }
        previous = current;
        current = next;
        next->Enter(previous ? previous->id : 0x7fffffff);
        OnEnter(previous, next);
    }
};
#pragma pack(pop)

enum UnkMusicPhase_lfcfe14 : s32 {
    UnkMusicPhase_lfcfe14_Idle = 0,
    UnkMusicPhase_lfcfe14_Starting = 1,
    UnkMusicPhase_lfcfe14_Done = 0x7fffffff,
};

struct UnkMusicState_lfcfe14 {
    u8 pad_0x0[0x30];
    UnkMachine_lfcfe14 machine;     // 0x30
    UnkMusicPhase_lfcfe14 phase;    // 0xd4
    bool held;                      // 0xd8
    u8 pad_0xd9[0xe0 - 0xd9];
    UnkState_lfcfe14 states[8];     // 0xe0
    u32 field_0xb60;
    bool field_0xb64;
};
static_assert(__builtin_offsetof(UnkMusicState_lfcfe14, phase) == 0xd4, "layout");
static_assert(__builtin_offsetof(UnkMusicState_lfcfe14, field_0xb60) == 0xb60, "layout");

void fn_7100fcfe14(UnkMusicState_lfcfe14* self) {
    self->machine.Add(&self->states[0]);
    self->machine.Add(&self->states[1]);
    self->machine.Add(&self->states[2]);
    self->machine.Add(&self->states[3]);
    self->machine.Add(&self->states[6]);
    self->machine.Add(&self->states[7]);
    self->machine.Add(&self->states[4]);
    self->machine.Add(&self->states[5]);
    self->phase = UnkMusicPhase_lfcfe14_Starting;
    self->field_0xb64 = false;
    if (self->held)
        return;
    self->machine.SetState(1);
    self->phase = UnkMusicPhase_lfcfe14_Done;
}
