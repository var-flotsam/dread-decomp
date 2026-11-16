// Unit 1454 (tools/tu_map.py): fn_7100982e34 sets up the object's two state machines, registering each state
// once by id and binding its three callbacks to the object. Names and layouts are placeholders.
#include "types.h"

void fn_7100987fd0();
void fn_7100987fd4();
void fn_7100987fd8();
void fn_7100987fdc();
void fn_7100987fe0();
void fn_7100987fe4();
void fn_710098829c();
void fn_71009882a0();
void fn_71009882a4();
void fn_71009882a8();
void fn_71009882ac();
void fn_71009882b0();

struct UnkMachine_l982e34;

struct UnkDelegate_l982e34 {
    void* object;
    void (*function)();
};

struct UnkState_l982e34 {
    void** vtable;
    UnkMachine_l982e34* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkDelegate_l982e34 field_0x30;
    UnkDelegate_l982e34 field_0x40;
    UnkDelegate_l982e34 field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkState_l982e34) == 0x70, "layout");

struct UnkMachine_l982e34 {
    void** vtable;
    void* owner;
    UnkState_l982e34* current;
    UnkState_l982e34* previous;
    u32 count;
    u32 states[0x20];  // UnkState_l982e34* entries, 4-byte aligned
    UnkState_l982e34** States() { return reinterpret_cast<UnkState_l982e34**>(states); }
    u8 pad_0xa4[0xb0 - 0xa4];

    // Registers `state` unless one with its id is registered already.
    void Add(UnkState_l982e34* state) {
        UnkState_l982e34** begin = States();
        UnkState_l982e34** end = begin + count;
        for (UnkState_l982e34** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};
static_assert(sizeof(UnkMachine_l982e34) == 0xb0, "layout");

struct Unk_l982e34 {
    u8 pad_0x0[0x30];
    UnkMachine_l982e34 machine;         // 0x30
    UnkMachine_l982e34 other_machine;   // 0xe0
    UnkState_l982e34 states[3];         // 0x190
    UnkState_l982e34 other_states[2];   // 0x2e0
};
static_assert(__builtin_offsetof(Unk_l982e34, states) == 0x190, "layout");
static_assert(__builtin_offsetof(Unk_l982e34, other_states) == 0x2e0, "layout");

static inline void Bind(Unk_l982e34* self, UnkMachine_l982e34* machine, UnkState_l982e34* state, void (*a)(),
                        void (*b)(), void (*c)()) {
    machine->Add(state);
    state->field_0x30 = {self, a};
    state->field_0x40 = {self, b};
    state->field_0x50 = {self, c};
}

void fn_7100982e34(Unk_l982e34* self) {
    self->machine.owner = self;
    self->machine.Add(&self->states[0]);
    Bind(self, &self->machine, &self->states[1], fn_7100987fd0, fn_7100987fd4, fn_7100987fd8);
    Bind(self, &self->machine, &self->states[2], fn_7100987fdc, fn_7100987fe0, fn_7100987fe4);

    self->other_machine.owner = self;
    Bind(self, &self->other_machine, &self->other_states[0], fn_710098829c, fn_71009882a0, fn_71009882a4);
    Bind(self, &self->other_machine, &self->other_states[1], fn_71009882a8, fn_71009882ac, fn_71009882b0);
}
