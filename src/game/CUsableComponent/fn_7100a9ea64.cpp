// Sets up an object's state machine: registers its seven states (each once, by id) and binds
// each state's three callbacks to the object, then passes the owner's id to fn_7100a9ee2c.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkStateMachine;

struct UnkDelegate {
    void* object;
    void (*function)();
};

struct UnkState {
    void** vtable;
    UnkStateMachine* machine;
    u32 id;
    u8 unk_0x14[0x1c];
    UnkDelegate field_0x30;
    UnkDelegate field_0x40;
    UnkDelegate field_0x50;
    u8 unk_0x60[0x10];
};
static_assert(sizeof(UnkState) == 0x70, "UnkState layout");

struct UnkStateMachine {
    u8 unk_0x0[0x8];
    void* owner;
    u8 unk_0x10[0x10];
    u32 count;
    u32 states[0x22];  // UnkState* entries, 4-byte aligned

    UnkState** States() { return reinterpret_cast<UnkState**>(states); }

    void Add(UnkState* state) {
        UnkState** begin = States();
        UnkState** end = begin + count;
        for (UnkState** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};

struct UnkOwnerInfo {
    u8 unk_0x0[0x30];
    CStrId id;
};

struct UnkStateObject {
    u8 unk_0x0[0x18];
    UnkOwnerInfo* info;
    u8 unk_0x20[0xe0];
    UnkStateMachine machine;
    UnkState states[7];
    u8 field_0x4c0[0x8];
};
static_assert(__builtin_offsetof(UnkStateObject, states) == 0x1b0, "layout");
static_assert(__builtin_offsetof(UnkStateObject, field_0x4c0) == 0x4c0, "layout");

void fn_7100aa457c();
void fn_7100aa4588();
void fn_7100aa4594();
void fn_7100aa45a0();
void fn_7100aa45ac();
void fn_7100aa45b8();
void fn_7100aa45c4();
void fn_7100aa45d0();
void fn_7100aa45dc();
void fn_7100aa45e8();
void fn_7100aa45f4();
void fn_7100aa4600();
void fn_7100aa460c();
void fn_7100aa4618();
void fn_7100aa4624();
void fn_7100aa4630();
void fn_7100aa463c();
void fn_7100aa4648();
void fn_7100aa4654();
void fn_7100aa4660();
void fn_7100aa466c();
void fn_7100a9ee2c(void* self, CStrId id);

static inline void Bind(UnkStateObject* self, UnkState* state, void (*a)(), void (*b)(), void (*c)()) {
    self->machine.Add(state);
    state->field_0x30 = {self, a};
    state->field_0x50 = {self, b};
    state->field_0x40 = {self, c};
}

void fn_7100a9ea64(UnkStateObject* self) {
    self->machine.owner = self;
    Bind(self, &self->states[0], fn_7100aa457c, fn_7100aa4588, fn_7100aa4594);
    Bind(self, &self->states[1], fn_7100aa45a0, fn_7100aa45ac, fn_7100aa45b8);
    Bind(self, &self->states[2], fn_7100aa45c4, fn_7100aa45d0, fn_7100aa45dc);
    Bind(self, &self->states[3], fn_7100aa45e8, fn_7100aa45f4, fn_7100aa4600);
    Bind(self, &self->states[4], fn_7100aa460c, fn_7100aa4618, fn_7100aa4624);
    Bind(self, &self->states[5], fn_7100aa4630, fn_7100aa463c, fn_7100aa4648);
    Bind(self, &self->states[6], fn_7100aa4654, fn_7100aa4660, fn_7100aa466c);
    fn_7100a9ee2c(self->field_0x4c0, self->info->id);
}
