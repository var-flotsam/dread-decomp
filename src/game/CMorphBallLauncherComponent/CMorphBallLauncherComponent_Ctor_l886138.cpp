// CMorphBallLauncherComponent's unit: the component's constructor. After the component base constructor it
// sets the two vtables, four states (ids 1 to 4), the state machine, a callback set with an empty string id,
// a cleared block and two listeners. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAbilityOpticCamouflage.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

extern void* g_unk_7101a7bdf0[];
extern void* g_unk_7101a7bfa8[];
extern void* g_unk_7101a7c058[];

struct UnkMblMachine_l886138;

struct UnkMblState_l886138 {
    void** vtable;
    UnkMblMachine_l886138* machine;
    u32 id;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u64 field_0x48;
    u64 field_0x50;
    u64 field_0x58;
    u64 field_0x60;
    u64 field_0x68;
};
static_assert(sizeof(UnkMblState_l886138) == 0x70, "layout");

struct UnkMblMachine_l886138 {
    void** vtable;
    void* owner;
    UnkMblState_l886138* current;
    UnkMblState_l886138* previous;
    u32 count;
    u32 states[0x20];
    u32 pending;
    bool busy;
    bool field_0xa9;
    u8 pad_0xaa[0xb0 - 0xaa];
};
static_assert(sizeof(UnkMblMachine_l886138) == 0xb0, "layout");

struct UnkMblCallbacks_l886138 {
    void** vtable;
    CRntVector<void*> items;  // 0x8
    u32 field_0x30;
    u64 field_0x38;
    CStrId id;  // 0x40
};

struct UnkMblListener_l886138 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkMorphBallLauncher_l886138 {
    void** vtable;
    u8 pad_0x8[0x38];
    void** vtable_0x40;
    UnkMblState_l886138 states[4];      // 0x48
    UnkMblMachine_l886138 machine;      // 0x208
    UnkMblCallbacks_l886138 callbacks;  // 0x2b8
    bool field_0x300;
    u64 field_0x308;
    u64 field_0x310;
    u64 field_0x318;
    u64 field_0x320;
    u64 field_0x328;
    u64 field_0x330;
    u64 field_0x338;
    u64 field_0x340;
    UnkMblListener_l886138 listener_0x348;
    UnkMblListener_l886138 listener_0x368;
};
static_assert(__builtin_offsetof(UnkMorphBallLauncher_l886138, callbacks) == 0x2b8, "layout");
static_assert(__builtin_offsetof(UnkMorphBallLauncher_l886138, listener_0x368) == 0x368, "layout");

static inline void InitState(UnkMblState_l886138* state, u32 id) {
    state->machine = nullptr;
    state->id = id;
    state->field_0x18 = 0;
    state->field_0x20 = 0;
    state->field_0x28 = 2;
    state->field_0x2c = 1;
    state->vtable = g_unk_7101a7bfa8 + 2;
    state->field_0x30 = 0;
    state->field_0x38 = 0;
    state->field_0x40 = 0;
    state->field_0x48 = 0;
    state->field_0x50 = 0;
    state->field_0x58 = 0;
    state->field_0x60 = 0;
    state->field_0x68 = 0;
}

void fn_7100886138(UnkMorphBallLauncher_l886138* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a7bdf0 + 2;
    self->vtable_0x40 = g_unk_7101a7bdf0 + 52;

    InitState(&self->states[0], 1);
    InitState(&self->states[1], 2);
    InitState(&self->states[2], 3);
    InitState(&self->states[3], 4);

    UnkMblMachine_l886138& machine = self->machine;
    machine.vtable = g_unk_7101a7c058 + 2;
    machine.owner = nullptr;
    machine.current = nullptr;
    machine.previous = nullptr;
    machine.count = 0;
    machine.pending = 5;
    machine.busy = false;
    machine.field_0xa9 = false;

    UnkMblCallbacks_l886138& callbacks = self->callbacks;
    callbacks.vtable = g_unk_7101a20998 + 2;
    new (&callbacks.items) CRntVector<void*>(15);
    callbacks.field_0x30 = 0;
    callbacks.vtable = g_unk_7101a33fa0 + 2;
    callbacks.field_0x38 = 0;
    callbacks.id.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    callbacks.id.data = &g_unk_7101d47108->empty;

    self->field_0x300 = true;
    self->field_0x310 = 0;
    self->field_0x318 = 0;
    self->field_0x320 = 0;
    self->field_0x328 = 0;
    self->field_0x330 = 0;
    self->field_0x338 = 0;
    self->field_0x340 = 0;

    self->listener_0x348.vtable = g_unk_7101a22d48 + 2;
    self->listener_0x348.field_0x8 = 0;
    self->listener_0x348.field_0x10 = 0;
    self->listener_0x348.field_0x18 = 0;
    self->listener_0x368.vtable = g_unk_7101a23280 + 2;
    self->listener_0x368.field_0x8 = 0;
    self->listener_0x368.field_0x10 = 0;
    self->listener_0x368.field_0x18 = 0;
}
