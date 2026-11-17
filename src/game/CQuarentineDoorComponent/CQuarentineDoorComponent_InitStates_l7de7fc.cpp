// CQuarentineDoorComponent's unit: fn_71007de7fc registers the door's three states on its machine (+0xa0),
// binds their three callbacks to the door, then asks the machine for state 2. Names and layouts are
// placeholders.
#include "types.h"

void fn_71007dfdfc();
void fn_71007dfe00();
void fn_71007dfe04();
void fn_71007dfe08();
void fn_71007dfe0c();
void fn_71007dfe10();
void fn_71007dfe14();
void fn_71007dfe18();
void fn_71007dfe1c();

struct UnkDoorMachine_l7de7fc;
struct UnkDoorState_l7de7fc;

struct UnkDoorDelegate_l7de7fc {
    void* object;
    void (*function)();
};

struct UnkDoorStateVtbl_l7de7fc {
    void* pad_0x0[2];
    void (*vf_0x10)(UnkDoorState_l7de7fc* self, u32 previous_id);  // entered
    void (*vf_0x18)(UnkDoorState_l7de7fc* self, u32 next_id);      // left
};

struct UnkDoorState_l7de7fc {
    UnkDoorStateVtbl_l7de7fc* vtable;
    UnkDoorMachine_l7de7fc* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkDoorDelegate_l7de7fc field_0x30;
    UnkDoorDelegate_l7de7fc field_0x40;
    UnkDoorDelegate_l7de7fc field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkDoorState_l7de7fc) == 0x70, "layout");

struct UnkDoorMachineVtbl_l7de7fc {
    void* pad_0x0[4];
    void (*vf_0x20)(UnkDoorMachine_l7de7fc* self, UnkDoorState_l7de7fc* previous, UnkDoorState_l7de7fc* next);
    void (*vf_0x28)(UnkDoorMachine_l7de7fc* self, UnkDoorState_l7de7fc* current, UnkDoorState_l7de7fc* next);
};

struct UnkDoorMachine_l7de7fc {
    static const u32 kNoState = 0x7fffffff;

    UnkDoorMachineVtbl_l7de7fc* vtable;
    void* owner;
    UnkDoorState_l7de7fc* current;
    UnkDoorState_l7de7fc* previous;
    u32 count;
    u32 states[0x20];  // UnkDoorState_l7de7fc* entries, 4-byte aligned
    u32 pending;       // 0xa4
    u8 changing;       // 0xa8
    u8 pad_0xa9[0xb0 - 0xa9];

    UnkDoorState_l7de7fc** States() { return reinterpret_cast<UnkDoorState_l7de7fc**>(states); }

    // Registers `state` unless one with its id is registered already.
    void Add(UnkDoorState_l7de7fc* state) {
        UnkDoorState_l7de7fc** begin = States();
        UnkDoorState_l7de7fc** end = begin + count;
        for (UnkDoorState_l7de7fc** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }

    // Leaves the current state for the registered one with `id`, unless it is current already.
    void Change(u32 id) {
        if (current && current->id == id)
            return;
        UnkDoorState_l7de7fc** begin = States();
        UnkDoorState_l7de7fc** end = begin + count;
        UnkDoorState_l7de7fc** p = begin;
        for (; p != end; ++p)
            if ((*p)->id == id)
                break;
        if (p == end)
            return;
        UnkDoorState_l7de7fc* next = *p;
        if (!next)
            return;
        if (current) {
            current->vtable->vf_0x18(current, id);
            vtable->vf_0x28(this, current, next);
        }
        previous = current;
        current = next;
        u32 previous_id = previous ? previous->id : kNoState;
        next->vtable->vf_0x10(next, previous_id);
        vtable->vf_0x20(this, previous, next);
    }

    // Asks for state `id`; it is entered now unless a change is under way.
    void Request(u32 id) {
        pending = id;
        if (!changing) {
            Change(pending);
            pending = kNoState;
        }
    }
};
static_assert(sizeof(UnkDoorMachine_l7de7fc) == 0xb0, "layout");

struct UnkQuarentineDoor_l7de7fc {
    u8 pad_0x0[0xa0];
    UnkDoorMachine_l7de7fc machine;    // 0xa0
    UnkDoorState_l7de7fc states[3];    // 0x150
};
static_assert(__builtin_offsetof(UnkQuarentineDoor_l7de7fc, states) == 0x150, "layout");

static inline void Bind(UnkQuarentineDoor_l7de7fc* self, UnkDoorState_l7de7fc* state, void (*a)(), void (*b)(),
                        void (*c)()) {
    self->machine.Add(state);
    state->field_0x30 = {self, a};
    state->field_0x50 = {self, b};
    state->field_0x40 = {self, c};
}

void fn_71007de7fc(UnkQuarentineDoor_l7de7fc* self) {
    self->machine.owner = self;
    Bind(self, &self->states[0], fn_71007dfdfc, fn_71007dfe00, fn_71007dfe04);
    Bind(self, &self->states[1], fn_71007dfe08, fn_71007dfe0c, fn_71007dfe10);
    Bind(self, &self->states[2], fn_71007dfe14, fn_71007dfe18, fn_71007dfe1c);
    self->machine.Request(2);
}
