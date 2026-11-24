// behaviortree::CAutoolGoToRepairPointAction's unit (tools/tu_map.py): runs the class entries
// (fn_7100023854), binds the callbacks of four of the action's six states, then registers all six on its
// state machine (+0x2d0), each once by id. Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "types.h"

struct UnkComponent_71003c286c;

void fn_71003c2868();
void fn_71003c286c(UnkComponent_71003c286c* self);
void fn_71003c28d0();
void fn_71003c28d4();
void fn_71003c28d8();

struct UnkAutoolMachine_l3c0760;

struct UnkAutoolDelegate_l3c0760 {
    void* object;
    void (*function)();
};

struct UnkAutoolState_l3c0760 {
    void** vtable;
    UnkAutoolMachine_l3c0760* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkAutoolDelegate_l3c0760 field_0x30;
    UnkAutoolDelegate_l3c0760 field_0x40;
    UnkAutoolDelegate_l3c0760 field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkAutoolState_l3c0760) == 0x70, "layout");

struct UnkAutoolMachine_l3c0760 {
    void** vtable;
    void* owner;
    UnkAutoolState_l3c0760* current;
    UnkAutoolState_l3c0760* previous;
    u32 count;
    u32 states[0x20];  // UnkAutoolState_l3c0760* entries, 4-byte aligned
    UnkAutoolState_l3c0760** States() { return reinterpret_cast<UnkAutoolState_l3c0760**>(states); }

    // Registers `state` unless one with its id is registered already.
    void Add(UnkAutoolState_l3c0760* state) {
        UnkAutoolState_l3c0760** begin = States();
        UnkAutoolState_l3c0760** end = begin + count;
        for (UnkAutoolState_l3c0760** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};

struct UnkAutoolGoToRepairPoint_l3c0760 {
    u8 pad_0x0[0x30];
    UnkAutoolState_l3c0760 states[6];  // 0x30
    UnkAutoolMachine_l3c0760 machine;  // 0x2d0
};
static_assert(__builtin_offsetof(UnkAutoolGoToRepairPoint_l3c0760, machine) == 0x2d0, "layout");

void fn_71003c0760(UnkAutoolGoToRepairPoint_l3c0760* self) {
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
    self->machine.owner = self;
    self->states[0].field_0x30 = {self, fn_71003c2868};
    self->states[1].field_0x30 = {self, reinterpret_cast<void (*)()>(fn_71003c286c)};
    self->states[1].field_0x50 = {self, fn_71003c28d0};
    self->states[2].field_0x30 = {self, fn_71003c28d4};
    self->states[3].field_0x30 = {self, fn_71003c28d8};
    for (int i = 0; i < 6; ++i)
        self->machine.Add(&self->states[i]);
}
