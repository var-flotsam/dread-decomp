// CAbility's unit (tools/tu_map.py unit 427): sets up the ability's state machine, registering each of its six
// states once by id and binding their callbacks to the ability, then calls fn_71002f8848.
// Names and layouts are placeholders.
#include "game/CAbility.h"
#include "types.h"

void fn_71002faa98();
void fn_71002faaa4();
void fn_71002faab0();
void fn_71002faabc();
void fn_71002faac8();
void fn_71002faad4();
void fn_71002faae0();
void fn_71002faaec();
void fn_71002faaf8();
void fn_71002fab04();

struct UnkAbilityMachine_l2f8608;

struct UnkAbilityDelegate_l2f8608 {
    void* object;
    void (*function)();
};

struct UnkAbilityState_l2f8608 {
    void** vtable;
    UnkAbilityMachine_l2f8608* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkAbilityDelegate_l2f8608 field_0x30;
    UnkAbilityDelegate_l2f8608 field_0x40;
    UnkAbilityDelegate_l2f8608 field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkAbilityState_l2f8608) == 0x70, "layout");

struct UnkAbilityMachine_l2f8608 {
    void** vtable;
    void* owner;
    UnkAbilityState_l2f8608* current;
    UnkAbilityState_l2f8608* previous;
    u32 count;
    u32 states[0x20];  // UnkAbilityState_l2f8608* entries, 4-byte aligned
    UnkAbilityState_l2f8608** States() { return reinterpret_cast<UnkAbilityState_l2f8608**>(states); }

    // Registers `state` unless one with its id is registered already.
    void Add(UnkAbilityState_l2f8608* state) {
        UnkAbilityState_l2f8608** begin = States();
        UnkAbilityState_l2f8608** end = begin + count;
        for (UnkAbilityState_l2f8608** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};

struct UnkAbility_l2f8608 {
    void** vtable;
    u64 field_0x8;
    UnkAbilityMachine_l2f8608 machine;  // 0x10
    u8 pad_0xb8[0xe8 - 0xb8];
    UnkAbilityState_l2f8608 states[6];  // 0xe8
};
static_assert(__builtin_offsetof(UnkAbility_l2f8608, states) == 0xe8, "layout");

static inline void Bind(UnkAbility_l2f8608* self, UnkAbilityState_l2f8608* state, void (*a)()) {
    self->machine.Add(state);
    state->field_0x30 = {self, a};
}

static inline void Bind(UnkAbility_l2f8608* self, UnkAbilityState_l2f8608* state, void (*a)(), void (*b)(),
                        void (*c)()) {
    self->machine.Add(state);
    state->field_0x30 = {self, a};
    state->field_0x50 = {self, b};
    state->field_0x40 = {self, c};
}

void fn_71002f8608(UnkAbility_l2f8608* self) {
    self->machine.owner = self;
    Bind(self, &self->states[0], fn_71002faa98);
    Bind(self, &self->states[1], fn_71002faaa4);
    Bind(self, &self->states[2], fn_71002faab0);
    Bind(self, &self->states[3], fn_71002faabc);
    Bind(self, &self->states[4], fn_71002faac8, fn_71002faad4, fn_71002faae0);
    Bind(self, &self->states[5], fn_71002faaec, fn_71002faaf8, fn_71002fab04);
    fn_71002f8848(reinterpret_cast<CAbility*>(self));
}
