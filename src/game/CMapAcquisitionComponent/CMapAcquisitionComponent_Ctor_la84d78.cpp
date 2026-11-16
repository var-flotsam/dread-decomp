// CMapAcquisitionComponent unit (tools/tu_map.py): the component's constructor. After the usable component's
// constructor it clears the acquired flag, sets an empty string id, builds the state machine with its four
// states, the registry map, two tunables and the timeline listener. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CMapAcquisitionComponent.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "game/CUsableComponent.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a921c0[];  // the states' vtable
void fn_7100a87f5c(char* self);  // base::refl_StrIdListCtor<..>, instantiated in src/clones

struct UnkMapDelegate_la84d78 {
    void* object;
    void (*function)();
};

struct UnkMapMachine_la84d78;

struct UnkMapState_la84d78 {
    void** vtable;
    UnkMapMachine_la84d78* machine;
    u32 id;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    UnkMapDelegate_la84d78 field_0x30;
    UnkMapDelegate_la84d78 field_0x40;
    UnkMapDelegate_la84d78 field_0x50;
    u64 field_0x60;
    u64 field_0x68;
};
static_assert(sizeof(UnkMapState_la84d78) == 0x70, "layout");

struct UnkMapMachine_la84d78 {
    void** vtable;
    void* owner;
    UnkMapState_la84d78* current;
    UnkMapState_la84d78* previous;
    u32 count;
    u32 states[0x20];  // UnkMapState_la84d78* entries, 4-byte aligned
    u32 pending;
    bool busy;
    bool field_0xa9;
};
static_assert(sizeof(UnkMapMachine_la84d78) == 0xb0, "layout");

struct UnkMapAcquisition_la84d78 {
    void** vtable;
    u8 pad_0x8[0x539 - 0x8];
    bool acquired;                     // 0x539
    CStrId name;                       // 0x540
    UnkMapMachine_la84d78 machine;     // 0x548
    UnkMapState_la84d78 states[4];     // 0x5f8
    bool field_0x7b8;
    bool field_0x7b9;
    u8 pad_0x7ba[0x7c0 - 0x7ba];
    base::reflection::UnkReflRegistryMap registry;  // 0x7c0
    u8 pad_0x7f0[0x800 - 0x7f0];
    u64 field_0x800;
    f32 field_0x808;
    u32 field_0x80c;
    u32 field_0x810;
    u32 field_0x814;
    f32 field_0x818;
    u32 field_0x81c;
    void** listener_vtable;  // 0x820
    u64 field_0x828;
    u64 field_0x830;
    u64 field_0x838;
};
static_assert(__builtin_offsetof(UnkMapAcquisition_la84d78, machine) == 0x548, "layout");
static_assert(__builtin_offsetof(UnkMapAcquisition_la84d78, registry) == 0x7c0, "layout");
static_assert(__builtin_offsetof(UnkMapAcquisition_la84d78, listener_vtable) == 0x820, "layout");

static inline void InitState(UnkMapState_la84d78* state, u32 id) {
    state->machine = nullptr;
    state->id = id;
    state->field_0x18 = 0;
    state->field_0x20 = 0;
    state->field_0x28 = 2;
    state->field_0x2c = 1;
    state->vtable = g_unk_7101a921c0 + 2;
    state->field_0x30 = {nullptr, nullptr};
    state->field_0x40 = {nullptr, nullptr};
    state->field_0x50 = {nullptr, nullptr};
    state->field_0x60 = 0;
    state->field_0x68 = 0;
}

void fn_7100a84d78(UnkMapAcquisition_la84d78* self) {
    fn_7100a9e244(self);
    self->vtable = g_unk_7101a91d50 + 2;
    self->acquired = false;

    self->name.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->name.data = &g_unk_7101d47108->empty;

    UnkMapMachine_la84d78& machine = self->machine;
    machine.owner = nullptr;
    machine.vtable = g_unk_7101a92270 + 2;
    machine.current = nullptr;
    machine.previous = nullptr;
    machine.count = 0;
    machine.pending = 6;
    machine.busy = false;
    machine.field_0xa9 = false;

    InitState(&self->states[0], 1);
    InitState(&self->states[1], 2);
    InitState(&self->states[2], 3);
    InitState(&self->states[3], 4);

    self->field_0x7b8 = false;
    self->field_0x7b9 = false;
    fn_7100a87f5c(reinterpret_cast<char*>(&self->registry));
    self->field_0x800 = 0;
    self->field_0x808 = 0.5f;
    self->field_0x80c = 0;
    self->field_0x814 = 0;
    self->field_0x818 = 1.0f;

    self->listener_vtable = g_unk_7101a77000 + 2;
    self->field_0x828 = 0;
    self->field_0x830 = 0;
    self->field_0x838 = 0;
}
