// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): fn_71008cff64 runs the base state
// setup, shows the door's two side effects (flipping one of them by which side is set), resets its three
// colours, and registers its three states on the machine (+0x420) with their callbacks bound to the door.
// Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "game/CFXComponent.h"
#include "game/UnkActor.h"
#include "game/UnkStruct_71727313a4.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU285.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;

void fn_710115fe18(char*);

void fn_71008dd044();
void fn_71008dd050();
void fn_71008dd054();
void fn_71008dd08c();
void fn_71008dd098();
void fn_71008dd0a4();
void fn_71008dd0bc();
void fn_71008dd0c0();
void fn_71008dd0c4();

extern CStrId g_unk_7172731738 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172731740 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172731748 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172731750 __attribute__((visibility("hidden")));

struct UnkDoorFx_l8cff64 {
    u8 pad_0x0[0x28];
    UnkXformNode node;
};

struct UnkDoorMachine_l8cff64;

struct UnkDoorDelegate_l8cff64 {
    void* object;
    void (*function)();
};

struct UnkDoorState_l8cff64 {
    void** vtable;
    UnkDoorMachine_l8cff64* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkDoorDelegate_l8cff64 field_0x30;
    UnkDoorDelegate_l8cff64 field_0x40;
    UnkDoorDelegate_l8cff64 field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkDoorState_l8cff64) == 0x70, "layout");

struct UnkDoorMachine_l8cff64 {
    void** vtable;
    void* owner;
    UnkDoorState_l8cff64* current;
    UnkDoorState_l8cff64* previous;
    u32 count;
    u32 states[0x20];  // UnkDoorState_l8cff64* entries, 4-byte aligned
    UnkDoorState_l8cff64** States() { return reinterpret_cast<UnkDoorState_l8cff64**>(states); }
    u8 pad_0xa4[0xb0 - 0xa4];

    // Registers `state` unless one with its id is registered already.
    void Add(UnkDoorState_l8cff64* state) {
        UnkDoorState_l8cff64** begin = States();
        UnkDoorState_l8cff64** end = begin + count;
        for (UnkDoorState_l8cff64** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};
static_assert(sizeof(UnkDoorMachine_l8cff64) == 0xb0, "layout");

struct UnkDoor_l8cff64 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x420 - 0x38];
    UnkDoorMachine_l8cff64 machine;  // 0x420
    UnkDoorState_l8cff64 states[3];  // 0x4d0
    u8 pad_0x620[0x654 - 0x620];
    u32 field_0x654;
    u32 field_0x658;
    u8 pad_0x65c[0x6e8 - 0x65c];
    CColor4F colours[3];  // 0x6e8
};
static_assert(__builtin_offsetof(UnkDoor_l8cff64, field_0x654) == 0x654, "layout");
static_assert(__builtin_offsetof(UnkDoor_l8cff64, colours) == 0x6e8, "layout");

static inline void Bind(UnkDoor_l8cff64* self, UnkDoorState_l8cff64* state, void (*a)(), void (*b)(), void (*c)()) {
    self->machine.Add(state);
    state->field_0x30 = {self, a};
    state->field_0x50 = {self, b};
    state->field_0x40 = {self, c};
}

void fn_71008cff64(UnkDoor_l8cff64* self) {
    fn_710115fe18(reinterpret_cast<char*>(self));
    if (self->field_0x654 != 0 || self->field_0x658 != 0) {
        UnkVec3f flip(-1.0f, 1.0f, 1.0f);
        UnkDoorFx_l8cff64* fx = reinterpret_cast<UnkDoorFx_l8cff64*>(fn_71011520a8(
            static_cast<char*>(self->owner->field_0x280), 0, reinterpret_cast<char*>(&g_unk_7172731738), 1));
        if (fx) {
            UnkActor* owner = self->owner;
            UnkVec3f position(owner->field_0x44, owner->field_0x54, owner->field_0x64);
            fn_7100146d84(reinterpret_cast<UnkFxObject_ovb0041*>(fx), &position);
            fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(fx), false);
            CFXComponent_AddNamedEffect(static_cast<UnkRinkaControl_ovb0090*>(self->owner->field_0x280),
                          reinterpret_cast<UnkFxObject_ovb0041*>(fx),
                          reinterpret_cast<UnkRinkaEntry_ruc**>(&g_unk_7172731748), nullptr, 0);
            if (self->field_0x654 == 0)
                fn_7100208fa4(&fx->node, &flip);
        }
        fx = reinterpret_cast<UnkDoorFx_l8cff64*>(fn_71011520a8(
            static_cast<char*>(self->owner->field_0x280), 0, reinterpret_cast<char*>(&g_unk_7172731740), 1));
        if (fx) {
            UnkActor* owner = self->owner;
            UnkVec3f position(owner->field_0x44, owner->field_0x54, owner->field_0x64);
            fn_7100146d84(reinterpret_cast<UnkFxObject_ovb0041*>(fx), &position);
            fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(fx), false);
            CFXComponent_AddNamedEffect(static_cast<UnkRinkaControl_ovb0090*>(self->owner->field_0x280),
                          reinterpret_cast<UnkFxObject_ovb0041*>(fx),
                          reinterpret_cast<UnkRinkaEntry_ruc**>(&g_unk_7172731750), nullptr, 0);
            if (self->field_0x654 != 0)
                fn_7100208fa4(&fx->node, &flip);
        }
    }
    self->colours[0] = g_unk_71727313a4.colours[0];
    self->colours[1] = g_unk_71727313a4.colours[1];
    self->colours[2] = g_unk_71727313a4.colours[2];

    self->machine.owner = self;
    Bind(self, &self->states[0], fn_71008dd044, fn_71008dd050, fn_71008dd054);
    Bind(self, &self->states[1], fn_71008dd08c, fn_71008dd098, fn_71008dd0a4);
    Bind(self, &self->states[2], fn_71008dd0bc, fn_71008dd0c0, fn_71008dd0c4);
}
