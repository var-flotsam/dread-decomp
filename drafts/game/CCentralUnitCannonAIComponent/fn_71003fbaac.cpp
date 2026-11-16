// CCentralUnitCannonAIComponent's unit (tools/tu_map.py): the component's initialisation. After the base step it caches
// its character class as a CCharClassCentralUnitCannonAIComponent, binds the callbacks of its six states to itself and
// registers each state (once, by id) with its state machine. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkCannonStateMachine_l3fbaac;

struct UnkCannonDelegate_l3fbaac {
    void* object;
    void (*function)();
};

// A callback bound to an object.
static inline UnkCannonDelegate_l3fbaac MakeDelegate(void* object, void (*function)()) {
    UnkCannonDelegate_l3fbaac delegate;
    delegate.object = object;
    delegate.function = function;
    return delegate;
}

struct UnkCannonState_l3fbaac {
    void** vtable;
    UnkCannonStateMachine_l3fbaac* machine;  // 0x8
    u32 id;                                  // 0x10
    u8 unk_0x14[0x1c];
    UnkCannonDelegate_l3fbaac field_0x30;
    UnkCannonDelegate_l3fbaac field_0x40;
    UnkCannonDelegate_l3fbaac field_0x50;
    u8 unk_0x60[0x10];
};
static_assert(sizeof(UnkCannonState_l3fbaac) == 0x70, "UnkCannonState_l3fbaac layout");

struct UnkCannonStateMachine_l3fbaac {
    u8 unk_0x0[0x8];
    void* owner;  // 0x8
    u8 unk_0x10[0x10];
    u32 count;  // 0x20
    u32 states[0x22];  // UnkCannonState_l3fbaac* entries, 4-byte aligned

    UnkCannonState_l3fbaac** States() { return reinterpret_cast<UnkCannonState_l3fbaac**>(states); }

    void Add(UnkCannonState_l3fbaac* state) {
        UnkCannonState_l3fbaac** begin = States();
        UnkCannonState_l3fbaac** end = begin + count;
        for (UnkCannonState_l3fbaac** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};

struct UnkCannonAI_l3fbaac {
    u8 unk_0x0[0x10];
    UnkReflObject* char_class;  // 0x10
    u8 unk_0x18[0x146b0 - 0x18];
    UnkReflObject* cannon_char_class;  // 0x146b0
    u8 unk_0x146b8[0x20];
    UnkCannonState_l3fbaac states[6];  // 0x146d8
    UnkCannonStateMachine_l3fbaac machine;  // 0x14978
};
static_assert(__builtin_offsetof(UnkCannonAI_l3fbaac, states) == 0x146d8, "layout");
static_assert(__builtin_offsetof(UnkCannonAI_l3fbaac, machine) == 0x14978, "layout");

bool fn_71010f977c(char* s);
void fn_71003fe740();
void fn_71003fe744();
void fn_71003fe77c();
void fn_71003fe780();
void fn_71003fe784();
void fn_71003fe788();
void fn_71003fe78c();
void fn_71003fe7bc();
void fn_71003fe7c0();

bool fn_71003fbaac(UnkCannonAI_l3fbaac* self) {
    bool result = fn_71010f977c(reinterpret_cast<char*>(self));
    UnkReflObject* char_class = self->char_class;
    self->cannon_char_class =
        char_class && fn_710009d50c(char_class->vf_0x0(), fn_71003feb44()) ? char_class : nullptr;

    self->machine.owner = self;
    UnkCannonState_l3fbaac* states = self->states;
    states[0].field_0x30 = MakeDelegate(self, fn_71003fe740);
    states[1].field_0x30 = MakeDelegate(self, fn_71003fe744);
    states[2].field_0x30 = MakeDelegate(self, fn_71003fe77c);
    states[2].field_0x50 = MakeDelegate(self, fn_71003fe780);
    states[3].field_0x30 = MakeDelegate(self, fn_71003fe784);
    states[3].field_0x50 = MakeDelegate(self, fn_71003fe788);
    states[4].field_0x30 = MakeDelegate(self, fn_71003fe78c);
    states[4].field_0x50 = MakeDelegate(self, fn_71003fe7bc);
    states[5].field_0x30 = MakeDelegate(self, fn_71003fe7c0);
    self->machine.Add(&states[0]);
    self->machine.Add(&states[1]);
    self->machine.Add(&states[2]);
    self->machine.Add(&states[3]);
    self->machine.Add(&states[4]);
    self->machine.Add(&states[5]);
    return result;
}
