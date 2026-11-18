// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): fn_71008d33a8 is the door's update.
// After the item life update and the fade step it plays the state's cue when one is due, runs the scenario's
// auto-open script once the door becomes openable, steps the state machine (+0x420) up to ten times, and every
// 1/15 s alternates the door's colours between two sets and shows them on the owner's graphics. Names and layouts are
// placeholders.
#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "game/UnkSlots.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/lua_lib.h"
#include "types.h"

using base::color::CColor4F;

// The colour sets next to g_unk_71727313a4, as twelve floats each.
struct UnkStruct_71727313d4 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
};
struct UnkStruct_7172731404 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    u64 field_0x18;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
};
extern UnkStruct_7172731404 g_unk_7172731404;

struct UnkItemLifeUpdate_ovb0114;
struct UnkDoor_b0251;
struct UnkDoorFlow_b0251;
void fn_71008e1174(UnkItemLifeUpdate_ovb0114* self, f32 dt);
void fn_71008d387c(UnkDoor_b0251* self, f32 dt);
void fn_71008d2690(UnkDoorFlow_b0251* self);

struct UnkDoorLife_l8d33a8;
bool fn_71008d0db4(UnkDoorLife_l8d33a8* self);

struct UnkDoorState_l8d33a8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // update
    u64 field_0x8;
    u32 id;
};

// The states are only 4-aligned after the count.
typedef UnkDoorState_l8d33a8* __attribute__((aligned(4))) UnkDoorStatePtr_l8d33a8;

struct UnkDoorStateMachine_l8d33a8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkDoorState_l8d33a8* previous, UnkDoorState_l8d33a8* next);  // after entering
    virtual void vf_0x28(UnkDoorState_l8d33a8* current, UnkDoorState_l8d33a8* next);   // after leaving

    u64 field_0x8;
    UnkDoorState_l8d33a8* current;
    UnkDoorState_l8d33a8* previous;
    u32 count;
    UnkDoorStatePtr_l8d33a8 states[16];
    u32 pending;
    bool busy;

};
static_assert(sizeof(UnkDoorStateMachine_l8d33a8) == 0xb0, "layout");

static const u32 kNoState = 0x7fffffff;
static const u32 kStateCount = 4;

// Switches to state `id` (to none for id 0 when it is not registered) and clears the pending id.
static inline void SwitchState(UnkDoorStateMachine_l8d33a8* self, u32 id, UnkDoorState_l8d33a8* from) {
    if (!from || from->id != id) {
        UnkDoorState_l8d33a8* found = FindStateById(self->states, self->count, id);
        UnkDoorState_l8d33a8* next = nullptr;
        bool enter = false;
        bool change = true;
        if (found) {
            next = found;
            enter = true;
        } else if (id != 0) {
            change = false;
        }
        if (change) {
            if (from) {
                from->vf_0x18(id);
                self->vf_0x28(self->current, next);
            }
            self->previous = self->current;
            self->current = found;
            if (enter) {
                next->vf_0x10(self->previous ? self->previous->id : kNoState);
                self->vf_0x20(self->previous, next);
            }
        }
    }
    self->pending = kNoState;
}

// Takes the pending state, then updates the current one while it asks for another, at most ten times.
static inline void UpdateMachine(UnkDoorStateMachine_l8d33a8* self, f32 dt) {
    self->busy = true;
    if (self->pending < kStateCount)
        SwitchState(self, self->pending, self->current);
    for (u32 i = 0; i < 10; ++i) {
        if (!self->current)
            break;
        self->current->vf_0x20(dt);
        u32 id = self->pending;
        if (id == kNoState)
            break;
        if (id < kStateCount)
            SwitchState(self, id, self->current);
    }
    self->busy = false;
}

struct UnkDoorGfx_l8d33a8 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};
struct UnkDoorHolder_l8d33a8 {
    u8 pad_0x0[0x50];
    UnkDoorGfx_l8d33a8* gfx;
};
struct UnkDoorOwner_l8d33a8 {
    u8 pad_0x0[0x228];
    UnkDoorHolder_l8d33a8* holder;
};

struct UnkDoorCue_l8d33a8 {
    u8 pad_0x0[0x12b];
    u8 field_0x12b;
    bool field_0x12c;
};

struct UnkDoorLife_l8d33a8 {
    u8 pad_0x0[0x30];
    UnkDoorOwner_l8d33a8* owner;  // 0x30
    u8 pad_0x38[0x408 - 0x38];
    bool cue_pending;  // 0x408
    u8 pad_0x409[3];
    s32 field_0x40c;
    u8 pad_0x410[0x41c - 0x410];
    bool field_0x41c;
    u8 pad_0x41d[2];
    bool field_0x41f;
    UnkDoorStateMachine_l8d33a8 machine;  // 0x420
    u8 pad_0x4d0[0x6b0 - 0x4d0];
    f32 field_0x6b0;
    f32 colour_timer;     // 0x6b4
    CColor4F colours[3];  // 0x6b8
    CColor4F shown[3];    // 0x6e8
    u8 pad_0x718[0x7c4 - 0x718];
    bool openable;  // 0x7c4
    u8 pad_0x7c5[0x9f8 - 0x7c5];
    UnkDoorCue_l8d33a8* cue;  // 0x9f8
    u8 pad_0xa00[0xa10 - 0xa00];
    u32 colour_slot;  // 0xa10
    u8 pad_0xa14[0xaf0 - 0xa14];
    bool field_0xaf0;
};
static_assert(__builtin_offsetof(UnkDoorLife_l8d33a8, machine) == 0x420, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8d33a8, colours) == 0x6b8, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8d33a8, openable) == 0x7c4, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8d33a8, field_0xaf0) == 0xaf0, "layout");

static inline bool IsNear(f32 value, f32 target) {
    f32 low = value - 0.001f;
    f32 high = value + 0.001f;
    return low <= target && target <= high;
}

static inline const CColor4F* FirstColours() {
    return reinterpret_cast<const CColor4F*>(&g_unk_71727313d4);
}
static inline const CColor4F* SecondColours() {
    return reinterpret_cast<const CColor4F*>(&g_unk_7172731404);
}

void fn_71008d33a8(UnkDoorLife_l8d33a8* self, f32 dt) {
    fn_71008e1174(reinterpret_cast<UnkItemLifeUpdate_ovb0114*>(self), dt);
    fn_71008d387c(reinterpret_cast<UnkDoor_b0251*>(self), dt);
    if (!self->field_0x41c || self->field_0x41f)
        return;

    if (self->cue_pending) {
        UnkDoorCue_l8d33a8* cue = self->cue;
        u32 kind = cue->field_0x12b;
        u32 mode = cue->field_0x12c ? 5 : kind * 4;
        if (fn_7100d7bea0(reinterpret_cast<UnkActor*>(self->owner), mode) && !self->field_0xaf0) {
            fn_71008d2690(reinterpret_cast<UnkDoorFlow_b0251*>(self));
            self->cue_pending = false;
        }
    } else if (self->field_0x40c != 0x7fffffff) {
        fn_71008d2690(reinterpret_cast<UnkDoorFlow_b0251*>(self));
    }

    bool openable = fn_71008d0db4(self);
    if (!self->openable && openable) {
        if (fn_71010c0bb0(g_unk_7172a01d18, "CurrentScenario.OnDoorAutoOpenUnlocked")) {
            fn_7100fbc4e0(g_unk_7172a01d18, self->owner);
            char result[0x100];
            fn_71010c0d80(g_unk_7172a01d18, 1, result, sizeof(result), nullptr, nullptr);
        }
    }
    self->openable = openable;
    UpdateMachine(&self->machine, dt);

    f32 fade = self->field_0x6b0;
    if (!(fade > 0.0f) && IsNear(self->colours[0].r, g_unk_71727313d4.field_0x0))
        return;
    self->field_0x6b0 = fade - dt;
    self->colour_timer -= dt;
    if (self->colour_timer > 0.0f)
        return;
    UnkDoorHolder_l8d33a8* holder = self->owner->holder;
    if (!holder || !holder->gfx)
        return;
    if (IsNear(self->colours[0].r, g_unk_71727313d4.field_0x0)) {
        self->colours[0] = SecondColours()[0];
        self->colours[1] = SecondColours()[1];
        self->colours[2] = SecondColours()[2];
        self->colours[1].a = 0.0f;
    } else {
        self->colours[0] = FirstColours()[0];
        self->colours[1] = FirstColours()[1];
        self->colours[2] = FirstColours()[2];
    }
    if (self->colour_slot != 0xffffffff)
        fn_71001c2764(self->owner->holder->gfx->slots, self->colour_slot, 0,
                      reinterpret_cast<const base::math::CVector4D*>(&self->colours[0]), 1, true);
    self->shown[0] = self->colours[0];
    self->shown[1] = self->colours[1];
    self->shown[2] = self->colours[2];
    self->colour_timer = 1.0f / 15.0f;
}
