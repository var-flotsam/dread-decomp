// CWeightActivablePropComponent unit (tools/tu_map.py): when the touching actor's collision component (its owner's
// +0x1f8 object, flag +0x3a8) says so, requests state 2 from the component's state machine at +0x130 (the
// template of src/game/UnkStateMachineSetA6_b62.cpp, copied here; last state 2, "none" 3), then runs the unit's
// own update (fn_7100aca57c). Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"
struct UnkSmState_P5_b72 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_P5_b72* __attribute__((aligned(4))) UnkSmStatePtr_P5_b72;

struct UnkSmMachine_P5_b72 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_P5_b72* previous, UnkSmState_P5_b72* next);
    virtual void vf_0x28(UnkSmState_P5_b72* current, UnkSmState_P5_b72* next);

    u64 field_0x8;
    UnkSmState_P5_b72* current;
    UnkSmState_P5_b72* previous;
    u32 count;
    UnkSmStatePtr_P5_b72 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_P5_b72 {
    u8 pad_0x0[Offset];
    UnkSmMachine_P5_b72 machine;
};

// Pin: the states array's address is not derived from another one the caller computed.
template <bool Pin>
__attribute__((always_inline)) static inline bool FindState_P5_b72(UnkSmMachine_P5_b72* self, u32 id,
                                                               UnkSmState_P5_b72*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_P5_b72);
    asm("" : "+r"(n));
    UnkSmStatePtr_P5_b72* p = self->states;
    if (Pin)
        asm("" : "+r"(p));
    for (; n != 0; ++p, n -= sizeof(UnkSmStatePtr_P5_b72)) {
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
__attribute__((always_inline)) static inline void SwitchState_P5_b72(UnkSmMachine_P5_b72* self, u32 id,
                                                                 UnkSmState_P5_b72* current) {
    if (!current || current->id != id) {
        UnkSmState_P5_b72* found;
        UnkSmState_P5_b72* next;
        u32 enter;
        if (FindState_P5_b72<Pin>(self, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_P5_b72*, s32);
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
__attribute__((always_inline)) static inline void RequestState_P5_b72(UnkSmMachine_P5_b72* self, u32 id) {
    // Stored through a plain u32 pointer, as in the template's original.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_P5_b72<None>(self, id, self->current);
}

template <u32 Offset>
struct UnkSmOwner_b61;
void fn_7100aca57c(UnkSmOwner_b61<0x130>*);

struct UnkPropFlags_P5_b72 {
    u8 pad_0x0[0x3a8];
    bool active;
};
struct UnkPropOwner_P5_b72 {
    u8 pad_0x0[0x1f8];
    UnkPropFlags_P5_b72* flags;
};
struct UnkPropLink_P5_b72 {
    u8 pad_0x0[0x10];
    UnkPropLink_P5_b72* next;
};
struct UnkPropArg_P5_b72 {
    u8 pad_0x0[0x10];
    UnkPropLink_P5_b72* link;
};

struct UnkWeightProp_P5_b72 {
    u8 pad_0x0[0x130];
    UnkSmMachine_P5_b72 machine;
};

void fn_7100aca6e0(UnkWeightProp_P5_b72* self, UnkPropArg_P5_b72* arg) {
    auto* component = reinterpret_cast<base::reflection::UnkReflObject*>(arg->link->next->next);
    if (component && fn_710009d50c(component->vf_0x0(), fn_710085fc44())) {
        auto* owner = *reinterpret_cast<UnkPropOwner_P5_b72**>(reinterpret_cast<char*>(component) + 0x30);
        if (owner && owner->flags && owner->flags->active) {
            RequestState_P5_b72<2, 3>(&self->machine, 2);
            fn_7100aca57c(reinterpret_cast<UnkSmOwner_b61<0x130>*>(self));
        }
    }
}
