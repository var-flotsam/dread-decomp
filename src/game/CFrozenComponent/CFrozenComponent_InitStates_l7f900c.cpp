// CFrozenComponent's unit (tools/tu_map.py unit 1260): after the base component's step succeeds, caches
// the owner at +0x10 when it is a CCharClassFrozenComponent, then registers the four frozen states on the
// state machine at +0x228 and binds their callbacks. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CFrozenComponent.h"
#include "game/UnkComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

void fn_71007fb604();
void fn_71007fb610();
void fn_71007fb61c();
void fn_71007fb628();
void fn_71007fb634();
void fn_71007fb640();
void fn_71007fb64c();
void fn_71007fb658();
void fn_71007fb664();
void fn_71007fb670();
void fn_71007fb67c();
void fn_71007fb688();

struct UnkFrozenMachine_l7f900c;

struct UnkFrozenDelegate_l7f900c {
    void* object;
    void (*function)();
};

struct UnkFrozenState_l7f900c {
    void** vtable;
    UnkFrozenMachine_l7f900c* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkFrozenDelegate_l7f900c field_0x30;
    UnkFrozenDelegate_l7f900c field_0x40;
    UnkFrozenDelegate_l7f900c field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkFrozenState_l7f900c) == 0x70, "layout");

struct UnkFrozenMachine_l7f900c {
    void** vtable;
    void* owner;
    UnkFrozenState_l7f900c* current;
    UnkFrozenState_l7f900c* previous;
    u32 count;
    u32 states[0x20];  // UnkFrozenState_l7f900c* entries, 4-byte aligned
    UnkFrozenState_l7f900c** States() { return reinterpret_cast<UnkFrozenState_l7f900c**>(states); }
    u32 pending;
    bool busy;
    bool field_0x2d1;

    // Registers `state` unless one with its id is registered already.
    void Add(UnkFrozenState_l7f900c* state) {
        UnkFrozenState_l7f900c** begin = States();
        UnkFrozenState_l7f900c** end = begin + count;
        for (UnkFrozenState_l7f900c** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};

struct UnkFrozenComponent_l7f900c {
    u8 pad_0x0[0x10];
    UnkReflObject* owner;  // 0x10
    u8 pad_0x18[0x68 - 0x18];
    UnkFrozenState_l7f900c states[4];  // 0x68
    UnkFrozenMachine_l7f900c machine;  // 0x228
    UnkReflObject* char_class;  // 0x2d8
};
static_assert(__builtin_offsetof(UnkFrozenComponent_l7f900c, machine) == 0x228, "layout");
static_assert(__builtin_offsetof(UnkFrozenComponent_l7f900c, char_class) == 0x2d8, "layout");

static inline void Bind(UnkFrozenComponent_l7f900c* self, UnkFrozenState_l7f900c* state, void (*a)(),
                        void (*b)(), void (*c)()) {
    state->field_0x30 = {self, a};
    state->field_0x50 = {self, b};
    state->field_0x40 = {self, c};
    self->machine.Add(state);
}

bool CFrozenComponent_CacheCharClassAndRegisterStates(void* component, void* arg) {
    UnkFrozenComponent_l7f900c* self = static_cast<UnkFrozenComponent_l7f900c*>(component);
    if (!fn_71010c41bc(static_cast<UnkBaseComponent*>(component), arg))
        return false;
    UnkReflObject* owner = self->owner;
    self->char_class = owner && fn_710009d50c(owner->vf_0x0(), fn_71007ed888()) ? owner : nullptr;
    self->machine.owner = self;
    Bind(self, &self->states[0], fn_71007fb604, fn_71007fb610, fn_71007fb61c);
    Bind(self, &self->states[1], fn_71007fb628, fn_71007fb634, fn_71007fb640);
    Bind(self, &self->states[2], fn_71007fb64c, fn_71007fb658, fn_71007fb664);
    Bind(self, &self->states[3], fn_71007fb670, fn_71007fb67c, fn_71007fb688);
    return true;
}
