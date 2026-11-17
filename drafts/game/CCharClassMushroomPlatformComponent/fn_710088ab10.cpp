// CCharClassMushroomPlatformComponent unit (tools/tu_map.py unit 1345): binds the four states' callbacks to
// the object, registers each on the state machine (+0x260) once by id, then requests state 1.
// Names and layouts are placeholders.
#include "types.h"

void fn_710088cfd4();
void fn_710088cfd8();
void fn_710088cfdc();
void fn_710088cfe0();
void fn_710088cfe4();
void fn_710088cfe8();
void fn_710088cfec();
void fn_710088cff0();
void fn_710088cff4();
void fn_710088cff8();
void fn_710088cffc();
void fn_710088d000();

struct UnkMushMachine_l88ab10;

struct UnkMushDelegate_l88ab10 {
    void* object;
    void (*function)();
};

struct UnkMushState_l88ab10 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    UnkMushMachine_l88ab10* machine;
    s32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkMushDelegate_l88ab10 field_0x30;
    UnkMushDelegate_l88ab10 field_0x40;
    UnkMushDelegate_l88ab10 field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkMushState_l88ab10) == 0x70, "layout");

struct UnkMushStateEx_l88ab10 : UnkMushState_l88ab10 {
    u8 pad_0x70[0x8];
};
static_assert(sizeof(UnkMushStateEx_l88ab10) == 0x78, "layout");

struct UnkMushMachine_l88ab10 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkMushState_l88ab10* previous, UnkMushState_l88ab10* next);  // after entering
    virtual void vf_0x28(UnkMushState_l88ab10* current, UnkMushState_l88ab10* next);   // after leaving

    void* owner;
    UnkMushState_l88ab10* current;
    UnkMushState_l88ab10* previous;
    u32 count;
    u32 states[0x20];  // UnkMushState_l88ab10* entries, 4-byte aligned
    s32 pending;
    bool busy;

    UnkMushState_l88ab10** States() { return reinterpret_cast<UnkMushState_l88ab10**>(states); }

    // Registers `state` unless one with its id is registered already.
    void Add(UnkMushState_l88ab10* state) {
        UnkMushState_l88ab10** begin = States();
        UnkMushState_l88ab10** end = begin + count;
        for (UnkMushState_l88ab10** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }

    UnkMushState_l88ab10* Find(s32 id) {
        UnkMushState_l88ab10** end = States() + count;
        UnkMushState_l88ab10** p = States();
        for (; p != end; ++p)
            if ((*p)->id == id)
                break;
        return p != end ? *p : nullptr;
    }

    // Switches to the state `id` now unless the machine is busy, in which case it stays pending.
    void Request(s32 id) {
        pending = id;
        if (busy)
            return;
        if (!current || current->id != id) {
            if (UnkMushState_l88ab10* next = Find(id)) {
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
static_assert(__builtin_offsetof(UnkMushMachine_l88ab10, pending) == 0xa4, "layout");

struct UnkMushPlatform_l88ab10 {
    u8 pad_0x0[0x260];
    UnkMushMachine_l88ab10 machine;  // 0x260
    UnkMushState_l88ab10 state0;    // 0x310
    UnkMushStateEx_l88ab10 state1;  // 0x380
    UnkMushStateEx_l88ab10 state2;  // 0x3f8
    UnkMushState_l88ab10 state3;    // 0x470
};
static_assert(__builtin_offsetof(UnkMushPlatform_l88ab10, state0) == 0x310, "layout");
static_assert(__builtin_offsetof(UnkMushPlatform_l88ab10, state3) == 0x470, "layout");

static inline void Bind(UnkMushPlatform_l88ab10* self, UnkMushState_l88ab10* state, void (*a)(), void (*b)(),
                        void (*c)()) {
    self->machine.Add(state);
    state->field_0x30 = {self, a};
    state->field_0x50 = {self, b};
    state->field_0x40 = {self, c};
}

void fn_710088ab10(UnkMushPlatform_l88ab10* self) {
    self->machine.owner = self;
    Bind(self, &self->state0, fn_710088cfd4, fn_710088cfd8, fn_710088cfdc);
    Bind(self, &self->state1, fn_710088cfe0, fn_710088cfe4, fn_710088cfe8);
    Bind(self, &self->state2, fn_710088cfec, fn_710088cff0, fn_710088cff4);
    Bind(self, &self->state3, fn_710088cff8, fn_710088cffc, fn_710088d000);
    self->machine.Request(1);
}
