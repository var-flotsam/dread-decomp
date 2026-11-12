// CWeightActivatedPlatformSmartObjectComponent unit (tools/tu_map.py, unit 1516): requests of state 3 or 4 from the
// component's state machine at +0x180 (the template of src/game/UnkStateMachineSetA6_b62.cpp, copied here under
// this file's names; last state 7, "none" 6), each after asking the linked smart object's user. Names are
// placeholders.
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityShinespark.h"
#include "game/CElectricReactionComponent.h"
#include "game/CSmartObjectComponent.h"
#include "game/CWeightActivatedPlatformSmartObjectComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "types.h"
struct UnkSmState_A5_b72 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_A5_b72* __attribute__((aligned(4))) UnkSmStatePtr_A5_b72;

struct UnkSmMachine_A5_b72 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_A5_b72* previous, UnkSmState_A5_b72* next);
    virtual void vf_0x28(UnkSmState_A5_b72* current, UnkSmState_A5_b72* next);

    u64 field_0x8;
    UnkSmState_A5_b72* current;
    UnkSmState_A5_b72* previous;
    u32 count;
    UnkSmStatePtr_A5_b72 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_A5_b72 {
    u8 pad_0x0[Offset];
    UnkSmMachine_A5_b72 machine;
};

// Pin: the states array's address is not derived from another one the caller computed.
template <bool Pin>
__attribute__((always_inline)) static inline bool FindState_A5_b72(UnkSmMachine_A5_b72* self, u32 id,
                                                               UnkSmState_A5_b72*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_A5_b72);
    asm("" : "+r"(n));
    UnkSmStatePtr_A5_b72* p = self->states;
    if (Pin)
        asm("" : "+r"(p));
    for (; n != 0; ++p, n -= sizeof(UnkSmStatePtr_A5_b72)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` (the b59 "apply pending" body) and clears the pending id.
template <u32 None, bool Pin = true>
__attribute__((always_inline)) static inline void SwitchState_A5_b72(UnkSmMachine_A5_b72* self, u32 id,
                                                                 UnkSmState_A5_b72* current) {
    if (!current || current->id != id) {
        UnkSmState_A5_b72* found;
        UnkSmState_A5_b72* next;
        u32 enter;
        if (FindState_A5_b72<Pin>(self, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_A5_b72*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void RequestState_A5_b72(UnkSmMachine_A5_b72* self, u32 id) {
    // Stored through a plain u32 pointer, as in the template's original.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_A5_b72<None>(self, id, self->current);
}

struct UnkSmartObjectUserA;
struct UnkSmartObjectActor;
struct UnkEntityState;
struct UnkPlatformSm_b61;

namespace {
struct UnkPlatformUser_A5_b72 {
    virtual void vf_0x0();
    bool vf_0x180() { return reinterpret_cast<bool (*)(UnkPlatformUser_A5_b72*)>((*reinterpret_cast<void***>(this))[0x180 / 8])(this); }
    void vf_0x288() { reinterpret_cast<void (*)(UnkPlatformUser_A5_b72*)>((*reinterpret_cast<void***>(this))[0x288 / 8])(this); }
};
}  // namespace

struct UnkPlatformLink_A5_b72 {
    u8 pad_0x0[0x10];
    UnkSmartObjectActor* object;
};

struct UnkPlatformSm_A5_b72 {
    u8 pad_0x0[0x30];
    UnkEntityState* entity;
    u8 pad_0x38[0xb0 - 0x38];
    UnkPlatformLink_A5_b72* link;
    UnkPlatformLink_A5_b72* link_b8;
    u8 pad_0xc0[0x180 - 0xc0];
    UnkSmMachine_A5_b72 machine;
    u8 pad_0x230[0x559 - 0x230];
    bool field_0x559;
};
static_assert(__builtin_offsetof(UnkPlatformSm_A5_b72, field_0x559) == 0x559, "layout");

static inline UnkSmartObjectActor* LinkedObject_A5_b72(UnkPlatformLink_A5_b72* link) {
    return link ? link->object : nullptr;
}

void fn_7100a0c4e4(UnkPlatformSm_A5_b72* self) {
    if (UnkSmartObjectActor* object = LinkedObject_A5_b72(self->link)) {
        UnkPlatformUser_A5_b72* user = reinterpret_cast<UnkPlatformUser_A5_b72*>(fn_71007cfd8c(object));
        if (!user->vf_0x180())
            RequestState_A5_b72<7, 6>(&self->machine, 3);
    }
}

void fn_7100a0cb5c(UnkPlatformSm_A5_b72* self) {
    fn_7100232864(self->entity, true);
    if (UnkSmartObjectActor* object = LinkedObject_A5_b72(self->link)) {
        if (UnkPlatformUser_A5_b72* user = reinterpret_cast<UnkPlatformUser_A5_b72*>(fn_71007cfd8c(object)))
            user->vf_0x288();
        RequestState_A5_b72<7, 6>(&self->machine, 3);
    }
}

void fn_7100a0bab4(UnkPlatformSm_A5_b72* self) {
    if (self->field_0x559) {
        fn_7100a0a08c(reinterpret_cast<UnkPlatformSm_b61*>(self));
        return;
    }
    if (UnkSmartObjectActor* object = LinkedObject_A5_b72(self->link_b8)) {
        if (reinterpret_cast<UnkPlatformUser_A5_b72*>(object)->vf_0x180() && !fn_7100a04768(self))
            RequestState_A5_b72<7, 6>(&self->machine, 4);
    }
}

// The update: state 4 unless fn_7100a04768 holds and the player's ability (fn_7100327d14 in the ability set at
// +0x2e0) is not in state 6 (state 7 reads its sub-state at +0x20, +0x10); otherwise the timer at +0x14c runs and
// state 2 is requested once it reaches +0x148.
struct UnkPlayer;
struct UnkAbilitySet;
struct UnkAbility;

struct UnkPlatformSubState_A5_b72 {
    u8 pad_0x0[0x10];
    s32 state;
};
struct UnkPlatformAbility_A5_b72 {
    u8 pad_0x0[0x20];
    UnkPlatformSubState_A5_b72* sub;
    u8 pad_0x28[0xb4 - 0x28];
    s32 state;
};
struct UnkPlatformPlayer_A5_b72 {
    u8 pad_0x0[0x2e0];
    UnkAbilitySet* abilities;
};
struct UnkPlatformTimed_A5_b72 {
    u8 pad_0x0[0x148];
    f32 limit;
    f32 timer;
    u8 pad_0x150[0x180 - 0x150];
    UnkSmMachine_A5_b72 machine;
};

static inline bool PlayerHolds_A5_b72() {
    auto* player = reinterpret_cast<UnkPlatformPlayer_A5_b72*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !player->abilities)
        return false;
    auto* ability =
        reinterpret_cast<UnkPlatformAbility_A5_b72*>(fn_71003013ec(player->abilities, fn_7100327d14()));
    if (!ability)
        return false;
    s32 state = ability->state;
    if (state == 7) {
        if (!ability->sub)
            return false;
        state = ability->sub->state;
    }
    return state == 6;
}

void fn_7100a0af50(UnkPlatformTimed_A5_b72* self, f32 dt) {
    if (!fn_7100a04768(reinterpret_cast<UnkPlatformSm_A5_b72*>(self)) || PlayerHolds_A5_b72()) {
        RequestState_A5_b72<7, 6>(&self->machine, 4);
        return;
    }
    self->timer += dt;
    if (self->timer >= self->limit)
        RequestState_A5_b72<7, 6>(&self->machine, 2);
}
