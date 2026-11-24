// CCharClassEmmyValveComponent unit (tools/tu_map.py, unit 1212): fn_71007b2334 registers the valve's three
// states on its machine (+0x40) and binds their two callbacks, then fills the registry map at +0x258 under
// the owner's name. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "types.h"

using base::global::CStrId;

struct UnkRefl_fn_71007b24f4;
extern template bool base::reflection::refl_RegistryMapInit<UnkRefl_fn_71007b24f4>(
    base::reflection::UnkReflRegistryMap*, const base::global::CStrId&);

void fn_71007b4c60();
void fn_71007b4c64();
void fn_71007b4c68();
void fn_71007b4c6c();
void fn_71007b4c70();
void fn_71007b4c74();

struct UnkValveMachine_l7b2334;

struct UnkValveDelegate_l7b2334 {
    void* object;
    void (*function)();
};

struct UnkValveState_l7b2334 {
    void** vtable;
    UnkValveMachine_l7b2334* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkValveDelegate_l7b2334 field_0x30;
    UnkValveDelegate_l7b2334 field_0x40;
    UnkValveDelegate_l7b2334 field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkValveState_l7b2334) == 0x70, "layout");

struct UnkValveMachine_l7b2334 {
    void** vtable;
    void* owner;
    UnkValveState_l7b2334* current;
    UnkValveState_l7b2334* previous;
    u32 count;
    u32 states[0x20];  // UnkValveState_l7b2334* entries, 4-byte aligned
    UnkValveState_l7b2334** States() { return reinterpret_cast<UnkValveState_l7b2334**>(states); }
    u8 pad_0xa4[0xb0 - 0xa4];

    // Registers `state` unless one with its id is registered already.
    void Add(UnkValveState_l7b2334* state) {
        UnkValveState_l7b2334** begin = States();
        UnkValveState_l7b2334** end = begin + count;
        for (UnkValveState_l7b2334** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};
static_assert(sizeof(UnkValveMachine_l7b2334) == 0xb0, "layout");

struct UnkValveOwnerInfo_l7b2334 {
    u8 pad_0x0[0x30];
    CStrId name;
};

struct UnkValve_l7b2334 {
    u8 pad_0x0[0x18];
    UnkValveOwnerInfo_l7b2334* info;
    u8 pad_0x20[0x40 - 0x20];
    UnkValveMachine_l7b2334 machine;   // 0x40
    UnkValveState_l7b2334 states[3];   // 0xf0
    u8 pad_0x240[0x258 - 0x240];
    base::reflection::UnkReflRegistryMap registry;  // 0x258
};
static_assert(__builtin_offsetof(UnkValve_l7b2334, states) == 0xf0, "layout");
static_assert(__builtin_offsetof(UnkValve_l7b2334, registry) == 0x258, "layout");

static inline void Bind(UnkValve_l7b2334* self, UnkValveState_l7b2334* state, void (*a)(), void (*b)()) {
    self->machine.Add(state);
    state->field_0x30 = {self, a};
    state->field_0x50 = {self, b};
}

void CCharClassEmmyValveComponent_InitStateMachineAndRegistry(UnkValve_l7b2334* self) {
    self->machine.owner = self;
    Bind(self, &self->states[0], fn_71007b4c60, fn_71007b4c64);
    Bind(self, &self->states[1], fn_71007b4c68, fn_71007b4c6c);
    Bind(self, &self->states[2], fn_71007b4c70, fn_71007b4c74);

    base::reflection::refl_RegistryMapInit<UnkRefl_fn_71007b24f4>(&self->registry, CStrId(self->info->name));
}
