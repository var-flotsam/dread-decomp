// CGrappleBeamComponent unit (tools/tu_map.py, unit 1277): requests on the beam's state machine at +0x480
// (the machine of src/game/UnkStateMachineSet_b60.cpp: states 0 to 4, 5 for none). Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::UnkTunable;

struct UnkGrappleSmState_ovb0047 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkGrappleSmState_ovb0047* __attribute__((aligned(4))) UnkGrappleSmStatePtr_ovb0047;

struct UnkGrappleSm_ovb0047 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkGrappleSmState_ovb0047* previous, UnkGrappleSmState_ovb0047* next);
    virtual void vf_0x28(UnkGrappleSmState_ovb0047* current, UnkGrappleSmState_ovb0047* next);

    u64 field_0x8;
    UnkGrappleSmState_ovb0047* current;
    UnkGrappleSmState_ovb0047* previous;
    u32 count;
    UnkGrappleSmStatePtr_ovb0047 states[16];
    u32 pending;
    bool busy;
};

struct UnkGrappleSmOwnerObj_ovb0047 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkGrappleSmLink_ovb0047 {
    u8 pad_0x0[0x10];
    UnkGrappleSmOwnerObj_ovb0047* owner;  // 0x10
};

struct UnkGrappleSmOwner_ovb0047 {
    u8 pad_0x0[0x230];
    UnkGrappleSmLink_ovb0047* link;  // 0x230
    u8 pad_0x238[0x480 - 0x238];
    UnkGrappleSm_ovb0047 machine;
    u8 pad_0x530[0x538 - 0x530];
    f32 length;  // 0x538
    u8 pad_0x53c[0x4];
    f32 field_0x540;
    u8 pad_0x544[0x714 - 0x544];
    bool field_0x714;
    bool field_0x715;
};
static_assert(__builtin_offsetof(UnkGrappleSmOwner_ovb0047, field_0x714) == 0x714, "layout");

__attribute__((always_inline)) static inline bool FindState_ovb0047(UnkGrappleSm_ovb0047* self, u32 id,
                                                                   UnkGrappleSmState_ovb0047*& out) {
    u64 n = (u64)self->count * sizeof(UnkGrappleSmStatePtr_ovb0047);
    asm("" : "+r"(n));
    for (UnkGrappleSmStatePtr_ovb0047* p = self->states; n != 0; ++p, n -= sizeof(UnkGrappleSmStatePtr_ovb0047)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` and clears the pending id (5).
__attribute__((always_inline)) static inline void SwitchState_ovb0047(UnkGrappleSm_ovb0047* self, u32 id,
                                                                     UnkGrappleSmState_ovb0047* current) {
    if (!current || current->id != id) {
        UnkGrappleSmState_ovb0047* found;
        UnkGrappleSmState_ovb0047* next;
        u32 enter;
        if (FindState_ovb0047(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            asm("" : "+r"(enter));
            next = nullptr;
            asm("" : "+r"(next));
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkGrappleSmState_ovb0047*, s32);
            s32 previous_id = self->previous ? self->previous->id : 5;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = 5;
}

__attribute__((always_inline)) static inline void RequestState_ovb0047(UnkGrappleSm_ovb0047* self, u32 id,
                                                                      UnkGrappleSmState_ovb0047* current) {
    // Stored through a plain u32 pointer: the original rereads the state id after this.
    u32* pending = &self->pending;
    *pending = id;
    if (id > 4)
        return;
    if (self->busy)
        return;
    SwitchState_ovb0047(self, id, current);
}

__attribute__((always_inline)) static inline void Request_ovb0047(UnkGrappleSm_ovb0047* self, u32 id) {
    u32* pending = &self->pending;
    *pending = id;
    if (id > 4)
        return;
    if (self->busy)
        return;
    SwitchState_ovb0047(self, id, self->current);
}

static inline u32 StateId_ovb0047(UnkGrappleSmState_ovb0047* state) {
    return *reinterpret_cast<u32*>(reinterpret_cast<char*>(state) + 0x10);
}

// Clears both flags and, from state 2 or 3 (id | 1 == 3), requests state 4.
void fn_710081b808(UnkGrappleSmOwner_ovb0047* self) {
    *reinterpret_cast<u16*>(&self->field_0x714) = 0;
    UnkGrappleSmState_ovb0047* current = self->machine.current;
    if (current && (StateId_ovb0047(current) | 1) == 3)
        RequestState_ovb0047(&self->machine, 4, current);
}

// In state 3 sets the flag at +0x715; in state 2 requests state 4.
void fn_71008187e8(UnkGrappleSmOwner_ovb0047* self) {
    UnkGrappleSmState_ovb0047* current = self->machine.current;
    if (!current)
        return;
    switch (StateId_ovb0047(current)) {
    case 2:
        RequestState_ovb0047(&self->machine, 4, current);
        break;
    case 3:
        self->field_0x715 = true;
        break;
    }
}

// The tunables (CTunableGrappleBeamComponent, src/reflection/CGrappleBeamComponent_CTunableGrappleBeamComponent.cpp).
extern UnkTunable g_unk_71727232d8;
struct UnkGrappleSmTunable_ovb0047 {
    u8 pad_0x0[0x24];
    f32 advance_speed;  // fAdvanceSpeed
    f32 retract_speed;  // fRetractSpeed
};
static inline const UnkGrappleSmTunable_ovb0047* Tunable_ovb0047() {
    return reinterpret_cast<const UnkGrappleSmTunable_ovb0047*>(&g_unk_71727232d8);
}

static inline bool CantFire_ovb0047(UnkGrappleSmOwner_ovb0047* self) {
    return fn_710117b348(self->link->owner->animation, fn_7100cf0d38());
}

// Retracting: back to state 1 once the length reaches zero (or at once while the owner cannot fire).
void fn_710081b624(UnkGrappleSmOwner_ovb0047* self, f32 dt) {
    if (CantFire_ovb0047(self)) {
        Request_ovb0047(&self->machine, 1);
        return;
    }
    f32 length = self->length - Tunable_ovb0047()->retract_speed * dt;
    self->length = length;
    self->field_0x540 = length;
    if (!(length <= 0.0f))
        return;
    Request_ovb0047(&self->machine, 1);
}
