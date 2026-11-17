// CMagnetSlidingBlockComponent's unit (tools/tu_map.py 1412): after the base component's step, caches the
// owner at +0x10 when it is a CCharClassMagnetSlidingBlockComponent, then binds the callbacks of the five
// sliding states and registers them on the state machine at +0x280. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

void fn_7100921b34();
void fn_7100921b40();
void fn_7100921b4c();
void fn_7100921b58();
void fn_7100921b64();
void fn_7100921b70();
void fn_7100921b74();
void fn_7100921b80();
void fn_7100921b8c();
void fn_7100921b98();
void fn_7100921b9c();

struct UnkMagnetMachine_l91e8c0;

struct UnkMagnetDelegate_l91e8c0 {
    void* object;
    void (*function)();
};

struct UnkMagnetState_l91e8c0 {
    void** vtable;
    UnkMagnetMachine_l91e8c0* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkMagnetDelegate_l91e8c0 field_0x30;
    UnkMagnetDelegate_l91e8c0 field_0x40;
    UnkMagnetDelegate_l91e8c0 field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkMagnetState_l91e8c0) == 0x70, "layout");

struct UnkMagnetMachine_l91e8c0 {
    void** vtable;
    void* owner;
    UnkMagnetState_l91e8c0* current;
    UnkMagnetState_l91e8c0* previous;
    u32 count;
    u32 states[0x20];  // UnkMagnetState_l91e8c0* entries, 4-byte aligned
    UnkMagnetState_l91e8c0** States() { return reinterpret_cast<UnkMagnetState_l91e8c0**>(states); }

    // Registers `state` unless one with its id is registered already.
    void Add(UnkMagnetState_l91e8c0* state) {
        UnkMagnetState_l91e8c0** begin = States();
        UnkMagnetState_l91e8c0** end = begin + count;
        for (UnkMagnetState_l91e8c0** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};

struct UnkMagnetSlidingBlock_l91e8c0 {
    u8 pad_0x0[0x10];
    UnkReflObject* owner;  // 0x10
    u8 pad_0x18[0x40 - 0x18];
    UnkReflObject* char_class;  // 0x40
    u8 pad_0x48[0x50 - 0x48];
    UnkMagnetState_l91e8c0 states[5];  // 0x50
    UnkMagnetMachine_l91e8c0 machine;  // 0x280
};
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91e8c0, machine) == 0x280, "layout");

bool fn_710091e8c0(void* component, void* arg) {
    UnkMagnetSlidingBlock_l91e8c0* self = static_cast<UnkMagnetSlidingBlock_l91e8c0*>(component);
    fn_71010c41bc(static_cast<UnkBaseComponent*>(component), arg);
    UnkReflObject* owner = self->owner;
    self->char_class = owner && fn_710009d50c(owner->vf_0x0(), fn_710091b104()) ? owner : nullptr;
    self->machine.owner = self;

    UnkMagnetState_l91e8c0* state = &self->states[0];
    state->field_0x30 = {self, fn_7100921b34};
    state->field_0x40 = {self, fn_7100921b40};
    self->machine.Add(state);

    state = &self->states[1];
    state->field_0x30 = {self, fn_7100921b4c};
    state->field_0x50 = {self, fn_7100921b58};
    self->machine.Add(state);

    state = &self->states[2];
    state->field_0x30 = {self, fn_7100921b64};
    state->field_0x50 = {self, fn_7100921b70};
    self->machine.Add(state);

    state = &self->states[3];
    state->field_0x30 = {self, fn_7100921b74};
    state->field_0x40 = {self, fn_7100921b80};
    self->machine.Add(state);

    state = &self->states[4];
    state->field_0x30 = {self, fn_7100921b8c};
    state->field_0x50 = {self, fn_7100921b98};
    state->field_0x40 = {self, fn_7100921b9c};
    self->machine.Add(state);
    return true;
}
