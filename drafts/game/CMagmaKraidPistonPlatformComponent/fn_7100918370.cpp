// CMagmaKraidPistonPlatformComponent's unit (tools/tu_map.py 1404): sets up the piston's state machine at +0x40,
// registering its four states once by id and binding their callbacks to the component, then asks for state 1
// and enters it unless the machine is locked. Names and layouts are placeholders.
#include "types.h"

void fn_71009198e8();
void fn_71009198ec();
void fn_71009198f0();
void fn_71009198f4();
void fn_71009198f8();

struct UnkPistonMachine_l918370;

struct UnkPistonDelegate_l918370 {
    void* object;
    void (*function)();
};

struct UnkPistonState_l918370 {
    void** vtable;
    UnkPistonMachine_l918370* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkPistonDelegate_l918370 field_0x30;
    UnkPistonDelegate_l918370 field_0x40;
    UnkPistonDelegate_l918370 field_0x50;
    u8 pad_0x60[0x70 - 0x60];

    void Enter(u32 previous_id) {
        reinterpret_cast<void (*)(UnkPistonState_l918370*, u32)>(vtable[0x10 / 8])(this, previous_id);
    }
    void Leave(bool changing) {
        reinterpret_cast<void (*)(UnkPistonState_l918370*, bool)>(vtable[0x18 / 8])(this, changing);
    }
};
static_assert(sizeof(UnkPistonState_l918370) == 0x70, "layout");

static const u32 kNoState_l918370 = 0x7fffffff;

struct UnkPistonMachine_l918370 {
    void** vtable;
    void* owner;
    UnkPistonState_l918370* current;
    UnkPistonState_l918370* previous;
    u32 count;
    u32 states[0x20];  // UnkPistonState_l918370* entries, 4-byte aligned
    UnkPistonState_l918370** States() { return reinterpret_cast<UnkPistonState_l918370**>(states); }
    u32 requested;  // 0xa4
    bool locked;    // 0xa8
    u8 pad_0xa9[0xb0 - 0xa9];

    void OnEntered(UnkPistonState_l918370* from, UnkPistonState_l918370* to) {
        reinterpret_cast<void (*)(UnkPistonMachine_l918370*, UnkPistonState_l918370*, UnkPistonState_l918370*)>(
            vtable[0x20 / 8])(this, from, to);
    }
    void OnLeaving(UnkPistonState_l918370* from, UnkPistonState_l918370* to) {
        reinterpret_cast<void (*)(UnkPistonMachine_l918370*, UnkPistonState_l918370*, UnkPistonState_l918370*)>(
            vtable[0x28 / 8])(this, from, to);
    }

    // Registers `state` unless one with its id is registered already.
    void Add(UnkPistonState_l918370* state) {
        UnkPistonState_l918370** begin = States();
        UnkPistonState_l918370** end = begin + count;
        for (UnkPistonState_l918370** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }

    UnkPistonState_l918370* Find(u32 id) {
        UnkPistonState_l918370** begin = States();
        UnkPistonState_l918370** end = begin + count;
        for (UnkPistonState_l918370** p = begin; p != end; ++p)
            if ((*p)->id == id)
                return *p;
        return nullptr;
    }

    // Leaves the current state for the one with `id`, unless it is current already.
    void ChangeTo(u32 id) {
        if (current && current->id == id)
            return;
        UnkPistonState_l918370* next = Find(id);
        if (!next)
            return;
        if (current) {
            current->Leave(true);
            OnLeaving(current, next);
        }
        previous = current;
        current = next;
        next->Enter(previous ? previous->id : kNoState_l918370);
        OnEntered(previous, next);
    }

    // Asks for the state with `id` and, unless the machine is locked, enters it now.
    void Request(u32 id) {
        requested = id;
        if (locked)
            return;
        ChangeTo(id);
        requested = kNoState_l918370;
    }
};
static_assert(sizeof(UnkPistonMachine_l918370) == 0xb0, "layout");

struct UnkPistonPlatform_l918370 {
    u8 pad_0x0[0x40];
    UnkPistonMachine_l918370 machine;  // 0x40
    UnkPistonState_l918370 states[4];  // 0xf0
};
static_assert(__builtin_offsetof(UnkPistonPlatform_l918370, states) == 0xf0, "layout");

void fn_7100918370(UnkPistonPlatform_l918370* self) {
    self->machine.owner = self;

    UnkPistonState_l918370* state = &self->states[0];
    self->machine.Add(state);
    state->field_0x30 = {self, fn_71009198e8};

    state = &self->states[1];
    self->machine.Add(state);
    state->field_0x30 = {self, fn_71009198ec};
    state->field_0x50 = {self, fn_71009198f0};

    state = &self->states[2];
    self->machine.Add(state);
    state->field_0x30 = {self, fn_71009198f4};

    state = &self->states[3];
    self->machine.Add(state);
    state->field_0x30 = {self, fn_71009198f8};

    self->machine.Request(1);
}
