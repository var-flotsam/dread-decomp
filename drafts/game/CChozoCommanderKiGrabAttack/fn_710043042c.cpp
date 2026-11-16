// CChozoCommanderKiGrabAttack's unit (652): registers the attack's seven states (+0x150) on its state
// machine (+0x88), each once by id, binds their callbacks to the attack, then requests state 1 unless
// the machine is busy. Names and layouts are placeholders.
#include "types.h"

struct UnkKiGrabMachine_l43042c;

struct UnkKiGrabDelegate_l43042c {
    void* object;
    void (*function)();
};

struct UnkKiGrabState_l43042c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    UnkKiGrabMachine_l43042c* machine;
    s32 id;
    u8 pad_0x14[0x1c];
    UnkKiGrabDelegate_l43042c field_0x30;
    UnkKiGrabDelegate_l43042c field_0x40;
    UnkKiGrabDelegate_l43042c field_0x50;
    u8 pad_0x60[0x10];
};

struct UnkKiGrabMachine_l43042c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkKiGrabState_l43042c* previous, UnkKiGrabState_l43042c* next);  // after entering
    virtual void vf_0x28(UnkKiGrabState_l43042c* current, UnkKiGrabState_l43042c* next);   // after leaving

    void* owner;
    UnkKiGrabState_l43042c* current;
    UnkKiGrabState_l43042c* previous;
    u32 count;
    u8 states_raw[16 * sizeof(UnkKiGrabState_l43042c*)];  // the states, only 4-aligned after the count

    UnkKiGrabState_l43042c** States() { return reinterpret_cast<UnkKiGrabState_l43042c**>(states_raw); }
    s32 pending;
    bool busy;

    // Registers `state` unless one with its id is registered already.
    void Add(UnkKiGrabState_l43042c* state) {
        UnkKiGrabState_l43042c** begin = States();
        UnkKiGrabState_l43042c** end = begin + count;
        for (UnkKiGrabState_l43042c** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }

    UnkKiGrabState_l43042c** End() { return States() + count; }

    // The state `id`, or null.
    UnkKiGrabState_l43042c* Find(s32 id) {
        UnkKiGrabState_l43042c** end = End();
        for (UnkKiGrabState_l43042c** p = States(); p != end; ++p)
            if ((*p)->id == id)
                return *p;
        return nullptr;
    }

    // Switches to the state `id` now unless the machine is busy (then it stays pending).
    void Request(s32 id) {
        pending = id;
        if (busy)
            return;
        if (!current || current->id != id) {
            if (UnkKiGrabState_l43042c* next = Find(id)) {
                if (current) {
                    current->vf_0x18(id);
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

struct UnkKiGrab_l43042c {
    u8 pad_0x0[0x88];
    UnkKiGrabMachine_l43042c machine;     // 0x88
    u8 pad_0x138[0x18];
    UnkKiGrabState_l43042c states[7];     // 0x150
};
static_assert(__builtin_offsetof(UnkKiGrabMachine_l43042c, pending) == 0xa4, "layout");
static_assert(__builtin_offsetof(UnkKiGrab_l43042c, states) == 0x150, "layout");

void fn_7100434b34();
void fn_7100434b38();
void fn_7100434b3c();
void fn_7100434b4c();
void fn_7100434b78();
void fn_7100434b7c();
void fn_7100434b8c();
void fn_7100434be0();
void fn_7100434be4();
void fn_7100434bf4();
void fn_7100434c2c();
void fn_7100434c30();
void fn_7100434c40();
void fn_7100434c70();
void fn_7100434c74();
void fn_7100434c78();

void fn_710043042c(UnkKiGrab_l43042c* self) {
    UnkKiGrabMachine_l43042c* machine = &self->machine;
    machine->owner = self;

    UnkKiGrabState_l43042c* state = &self->states[0];
    machine->Add(state);
    state->field_0x30 = {self, fn_7100434b34};

    state = &self->states[1];
    machine->Add(state);
    state->field_0x30 = {self, fn_7100434b38};
    state->field_0x50 = {self, fn_7100434b3c};

    state = &self->states[2];
    machine->Add(state);
    state->field_0x30 = {self, fn_7100434b4c};
    state->field_0x40 = {self, fn_7100434b78};
    state->field_0x50 = {self, fn_7100434b7c};

    state = &self->states[3];
    machine->Add(state);
    state->field_0x30 = {self, fn_7100434b8c};
    state->field_0x40 = {self, fn_7100434be0};
    state->field_0x50 = {self, fn_7100434be4};

    state = &self->states[4];
    machine->Add(state);
    state->field_0x30 = {self, fn_7100434bf4};
    state->field_0x40 = {self, fn_7100434c2c};
    state->field_0x50 = {self, fn_7100434c30};

    state = &self->states[5];
    machine->Add(state);
    state->field_0x30 = {self, fn_7100434c40};
    state->field_0x50 = {self, fn_7100434c70};

    state = &self->states[6];
    machine->Add(state);
    state->field_0x30 = {self, fn_7100434c74};
    state->field_0x50 = {self, fn_7100434c78};

    machine->Request(1);
}
