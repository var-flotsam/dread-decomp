// CWaterNozzleComponent's unit (tools/tu_map.py): fn_71008a13cc sets up the component's state machine,
// registering each of its four states once by id and binding its three callbacks to the component.
// Names and layouts are placeholders.
#include "types.h"

void fn_71008a2bc4();
void fn_71008a2bf4();
void fn_71008a2bf8();
void fn_71008a2bfc();
void fn_71008a2c0c();
void fn_71008a2c10();
void fn_71008a2c14();
void fn_71008a2c18();
void fn_71008a2c1c();
void fn_71008a2c20();
void fn_71008a2c24();
void fn_71008a2c28();

struct UnkNozzleMachine_l8a13cc;

struct UnkNozzleDelegate_l8a13cc {
    void* object;
    void (*function)();
};

struct UnkNozzleState_l8a13cc {
    void** vtable;
    UnkNozzleMachine_l8a13cc* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkNozzleDelegate_l8a13cc field_0x30;
    UnkNozzleDelegate_l8a13cc field_0x40;
    UnkNozzleDelegate_l8a13cc field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkNozzleState_l8a13cc) == 0x70, "layout");

struct UnkNozzleMachine_l8a13cc {
    void** vtable;
    void* owner;
    UnkNozzleState_l8a13cc* current;
    UnkNozzleState_l8a13cc* previous;
    u32 count;
    u32 states[0x20];  // UnkNozzleState_l8a13cc* entries, 4-byte aligned
    UnkNozzleState_l8a13cc** States() { return reinterpret_cast<UnkNozzleState_l8a13cc**>(states); }
    u8 pad_0xa4[0xb0 - 0xa4];

    // Registers `state` unless one with its id is registered already.
    void Add(UnkNozzleState_l8a13cc* state) {
        UnkNozzleState_l8a13cc** begin = States();
        UnkNozzleState_l8a13cc** end = begin + count;
        for (UnkNozzleState_l8a13cc** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};
static_assert(sizeof(UnkNozzleMachine_l8a13cc) == 0xb0, "layout");

struct UnkWaterNozzle_l8a13cc {
    u8 pad_0x0[0x40];
    UnkNozzleMachine_l8a13cc machine;  // 0x40
    UnkNozzleState_l8a13cc states[4];  // 0xf0
};
static_assert(__builtin_offsetof(UnkWaterNozzle_l8a13cc, states) == 0xf0, "layout");

static inline void Bind(UnkWaterNozzle_l8a13cc* self, UnkNozzleState_l8a13cc* state, void (*a)(), void (*c)(),
                        void (*b)()) {
    self->machine.Add(state);
    state->field_0x30 = {self, a};
    state->field_0x50 = {self, c};
    state->field_0x40 = {self, b};
}

void fn_71008a13cc(UnkWaterNozzle_l8a13cc* self) {
    self->machine.owner = self;
    Bind(self, &self->states[0], fn_71008a2bc4, fn_71008a2bf4, fn_71008a2bf8);
    Bind(self, &self->states[1], fn_71008a2bfc, fn_71008a2c0c, fn_71008a2c10);
    Bind(self, &self->states[3], fn_71008a2c14, fn_71008a2c18, fn_71008a2c1c);
    Bind(self, &self->states[2], fn_71008a2c20, fn_71008a2c24, fn_71008a2c28);
}
