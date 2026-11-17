// CGrappleBeamComponent unit (tools/tu_map.py, unit 1277): the component's constructor. After the base
// constructor (fn_7100ac7224) it builds the state machine and its four states, three empty string ids, the
// lightning slots sized from the beam's maximum length, and loads the lightning model (fn_7100817ffc).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CWeaponMovement.h"
#include "game/CGrappleBeamComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkTunable;

extern void* g_unk_7101a75230[];  // the class's vtable
extern void* g_unk_7101a75570[];  // the states' vtable
extern void* g_unk_7101a75620[];  // the state machine's vtable

struct UnkGrappleBeamLightning_ovb0047;
void fn_7100817ffc(UnkGrappleBeamLightning_ovb0047* self);

struct UnkGrappleTunable_l817c94 {
    u8 pad_0x0[0x2c];
    f32 max_length;  // 0x2c, fMaxLength
};

struct UnkGrappleDelegate_l817c94 {
    void* object;
    void (*function)();
};

struct UnkGrappleMachine_l817c94;

struct UnkGrappleState_l817c94 {
    void** vtable;
    UnkGrappleMachine_l817c94* machine;
    u32 id;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    UnkGrappleDelegate_l817c94 field_0x30;
    UnkGrappleDelegate_l817c94 field_0x40;
    UnkGrappleDelegate_l817c94 field_0x50;
    u64 field_0x60;
    u64 field_0x68;
};
static_assert(sizeof(UnkGrappleState_l817c94) == 0x70, "layout");

struct UnkGrappleMachine_l817c94 {
    void** vtable;
    void* owner;
    UnkGrappleState_l817c94* current;
    UnkGrappleState_l817c94* previous;
    u32 count;
    u32 states[0x20];  // UnkGrappleState_l817c94* entries, 4-byte aligned
    u32 pending;
    bool busy;
    bool field_0xa9;
};
static_assert(sizeof(UnkGrappleMachine_l817c94) == 0xb0, "layout");

struct UnkGrappleBeam_l817c94 {
    void** vtable;
    u8 pad_0x8[0x480 - 0x8];
    UnkGrappleMachine_l817c94 machine;      // 0x480
    u64 field_0x530;
    u64 field_0x538;
    u32 field_0x540;
    void* shared;                           // 0x548
    UnkGrappleState_l817c94 states[4];      // 0x550
    u32 field_0x710;
    bool field_0x714;
    bool field_0x715;
    f32 field_0x718;
    CStrId ids[3];                          // 0x720
    void* scene_object;                     // 0x738
    void** lightning;                       // 0x740
    u32 lightning_count;                    // 0x748
};
static_assert(__builtin_offsetof(UnkGrappleBeam_l817c94, shared) == 0x548, "layout");
static_assert(__builtin_offsetof(UnkGrappleBeam_l817c94, field_0x710) == 0x710, "layout");
static_assert(__builtin_offsetof(UnkGrappleBeam_l817c94, ids) == 0x720, "layout");
static_assert(__builtin_offsetof(UnkGrappleBeam_l817c94, lightning_count) == 0x748, "layout");

static inline void InitState(UnkGrappleState_l817c94* state, u32 id) {
    state->machine = nullptr;
    state->id = id;
    state->field_0x18 = 0;
    state->field_0x20 = 0;
    state->field_0x28 = 2;
    state->field_0x2c = 1;
    state->vtable = g_unk_7101a75570 + 2;
    state->field_0x30 = {nullptr, nullptr};
    state->field_0x40 = {nullptr, nullptr};
    state->field_0x50 = {nullptr, nullptr};
    state->field_0x60 = 0;
    state->field_0x68 = 0;
}

static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100817c94(UnkGrappleBeam_l817c94* self) {
    fn_7100ac7224(reinterpret_cast<UnkComponent_0x5b8*>(self));
    self->vtable = g_unk_7101a75230 + 2;

    UnkGrappleMachine_l817c94& machine = self->machine;
    machine.owner = nullptr;
    machine.vtable = g_unk_7101a75620 + 2;
    machine.current = nullptr;
    machine.previous = nullptr;
    machine.count = 0;
    machine.pending = 5;
    machine.busy = false;
    machine.field_0xa9 = false;

    self->field_0x530 = 0;
    self->field_0x538 = 0;
    self->field_0x540 = 0;
    self->shared = nullptr;

    InitState(&self->states[0], 1);
    InitState(&self->states[1], 2);
    InitState(&self->states[2], 3);
    InitState(&self->states[3], 4);

    self->field_0x710 = 0;
    self->field_0x714 = false;
    self->field_0x715 = false;
    self->field_0x718 = 0.65f;
    MakeEmpty(&self->ids[0]);
    MakeEmpty(&self->ids[1]);
    MakeEmpty(&self->ids[2]);
    self->scene_object = nullptr;
    self->lightning = nullptr;
    const UnkGrappleTunable_l817c94* tunable = reinterpret_cast<const UnkGrappleTunable_l817c94*>(&g_unk_71727232d8);
    self->lightning_count = static_cast<u32>(tunable->max_length / 20.0f + 20.0f);
    fn_7100817ffc(reinterpret_cast<UnkGrappleBeamLightning_ovb0047*>(self));
}
