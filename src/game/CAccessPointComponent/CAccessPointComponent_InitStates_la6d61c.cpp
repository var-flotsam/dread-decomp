// CAccessPointComponent unit (tools/tu_map.py, unit 1581): fn_7100a6d61c runs the usable component's state
// setup, registers the access point's four states on its machine (+0x5e8) and binds their callbacks, then
// fills the registry map at +0x928 under the owner's name. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CUsableComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkRefl_fn_7100a6d894;
extern template bool base::reflection::refl_RegistryMapInit<UnkRefl_fn_7100a6d894>(
    base::reflection::UnkReflRegistryMap*, const base::global::CStrId&);

struct UnkStateObject;

void fn_7100a74268();
void fn_7100a7426c();
void fn_7100a74270();
void fn_7100a74274();
void fn_7100a74278();
void fn_7100a74330();
void fn_7100a743b4();
void fn_7100a743b8();
void fn_7100a743bc();
void fn_7100a743c0();
void fn_7100a743cc();
void fn_7100a743d8();

struct UnkApMachine_la6d61c;

struct UnkApDelegate_la6d61c {
    void* object;
    void (*function)();
};

struct UnkApState_la6d61c {
    void** vtable;
    UnkApMachine_la6d61c* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkApDelegate_la6d61c field_0x30;
    UnkApDelegate_la6d61c field_0x40;
    UnkApDelegate_la6d61c field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkApState_la6d61c) == 0x70, "layout");

struct UnkApMachine_la6d61c {
    void** vtable;
    void* owner;
    UnkApState_la6d61c* current;
    UnkApState_la6d61c* previous;
    u32 count;
    u32 states[0x20];  // UnkApState_la6d61c* entries, 4-byte aligned
    UnkApState_la6d61c** States() { return reinterpret_cast<UnkApState_la6d61c**>(states); }
    u32 pending;
    bool busy;
    bool field_0xa9;
    u8 pad_0xaa[0xb8 - 0xaa];

    // Registers `state` unless one with its id is registered already.
    void Add(UnkApState_la6d61c* state) {
        UnkApState_la6d61c** begin = States();
        UnkApState_la6d61c** end = begin + count;
        for (UnkApState_la6d61c** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};
static_assert(sizeof(UnkApMachine_la6d61c) == 0xb8, "layout");

struct UnkApOwnerInfo_la6d61c {
    u8 pad_0x0[0x30];
    CStrId name;
};

struct UnkAp_la6d61c {
    u8 pad_0x0[0x18];
    UnkApOwnerInfo_la6d61c* info;
    u8 pad_0x20[0x5e8 - 0x20];
    UnkApMachine_la6d61c machine;  // 0x5e8
    UnkApState_la6d61c states[4];  // 0x6a0
    u8 pad_0x860[0x928 - 0x860];
    base::reflection::UnkReflRegistryMap registry;  // 0x928
};
static_assert(__builtin_offsetof(UnkAp_la6d61c, states) == 0x6a0, "layout");
static_assert(__builtin_offsetof(UnkAp_la6d61c, registry) == 0x928, "layout");

static inline void Bind(UnkAp_la6d61c* self, UnkApState_la6d61c* state, void (*a)(), void (*b)(), void (*c)()) {
    self->machine.Add(state);
    state->field_0x30 = {self, a};
    state->field_0x50 = {self, b};
    state->field_0x40 = {self, c};
}

void CAccessPointComponent_InitStates(UnkAp_la6d61c* self) {
    fn_7100a9ea64(reinterpret_cast<UnkStateObject*>(self));
    self->machine.owner = self;
    Bind(self, &self->states[0], fn_7100a74268, fn_7100a7426c, fn_7100a74270);
    Bind(self, &self->states[1], fn_7100a74274, fn_7100a74278, fn_7100a74330);
    Bind(self, &self->states[3], fn_7100a743b4, fn_7100a743b8, fn_7100a743bc);
    Bind(self, &self->states[2], fn_7100a743c0, fn_7100a743cc, fn_7100a743d8);

    base::reflection::refl_RegistryMapInit<UnkRefl_fn_7100a6d894>(&self->registry, CStrId(self->info->name));
}
