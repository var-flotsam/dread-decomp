// CAbilityShinespark unit: fn_7100328408 runs the base ability's state setup, then binds each of the shinespark's
// five states' three callbacks to the object and registers the state on its machine (+0x6a0) once by id.
// Names and layouts are placeholders.
#include "types.h"

void fn_7100332310();
void fn_7100332314();
void fn_7100332318();
void fn_710033231c();
void fn_7100332320();
void fn_7100332324();
void fn_7100332328();
void fn_710033232c();
void fn_7100332330();
void fn_7100332334();
void fn_7100332338();
void fn_7100332404();
void fn_7100332408();
void fn_710033240c();
void fn_7100332464();

struct UnkShinesparkMachine_l328408;

struct UnkShinesparkDelegate_l328408 {
    void* object;
    void (*function)();
};

struct UnkShinesparkState_l328408 {
    void** vtable;
    UnkShinesparkMachine_l328408* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkShinesparkDelegate_l328408 field_0x30;
    UnkShinesparkDelegate_l328408 field_0x40;
    UnkShinesparkDelegate_l328408 field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkShinesparkState_l328408) == 0x70, "layout");

struct UnkShinesparkMachine_l328408 {
    void** vtable;
    void* owner;
    UnkShinesparkState_l328408* current;
    UnkShinesparkState_l328408* previous;
    u32 count;
    u32 states[0x20];  // UnkShinesparkState_l328408* entries, 4-byte aligned
    UnkShinesparkState_l328408** States() { return reinterpret_cast<UnkShinesparkState_l328408**>(states); }

    // Registers `state` unless one with its id is registered already.
    void Add(UnkShinesparkState_l328408* state) {
        UnkShinesparkState_l328408** begin = States();
        UnkShinesparkState_l328408** end = begin + count;
        for (UnkShinesparkState_l328408** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};

struct UnkShinespark_l328408 {
    u8 pad_0x0[0x470];
    UnkShinesparkState_l328408 states[5];   // 0x470
    UnkShinesparkMachine_l328408 machine;   // 0x6a0
};
static_assert(__builtin_offsetof(UnkShinespark_l328408, machine) == 0x6a0, "layout");

void fn_71002f85c4(UnkShinespark_l328408* self);

static inline void Bind(UnkShinespark_l328408* self, UnkShinesparkState_l328408* state, void (*a)(), void (*b)(),
                        void (*c)()) {
    state->field_0x30 = {self, a};
    state->field_0x50 = {self, b};
    state->field_0x40 = {self, c};
    self->machine.Add(state);
}

void fn_7100328408(UnkShinespark_l328408* self) {
    fn_71002f85c4(self);
    self->machine.owner = self;
    Bind(self, &self->states[0], fn_7100332310, fn_7100332314, fn_7100332318);
    Bind(self, &self->states[1], fn_710033231c, fn_7100332320, fn_7100332324);
    Bind(self, &self->states[2], fn_7100332328, fn_710033232c, fn_7100332330);
    Bind(self, &self->states[3], fn_7100332334, fn_7100332338, fn_7100332404);
    Bind(self, &self->states[4], fn_7100332408, fn_710033240c, fn_7100332464);
}
