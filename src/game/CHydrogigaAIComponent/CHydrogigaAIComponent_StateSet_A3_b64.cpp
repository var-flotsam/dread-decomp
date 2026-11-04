// Callers in CHydrogigaAIComponent's unit (tools/tu_map.py) that request a state from the component's
// small state machine at +0x14ca8 (states 0..6, "none" 7; the template of
// src/game/UnkStateMachineSetA3_b61.cpp under this file's names, with the states array addressed from
// the owner). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"
struct UnkSmState_A3_b64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_A3_b64* __attribute__((aligned(4))) UnkSmStatePtr_A3_b64;

struct UnkSmMachine_A3_b64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_A3_b64* previous, UnkSmState_A3_b64* next);
    virtual void vf_0x28(UnkSmState_A3_b64* current, UnkSmState_A3_b64* next);

    u64 field_0x8;
    UnkSmState_A3_b64* current;
    UnkSmState_A3_b64* previous;
    u32 count;
    UnkSmStatePtr_A3_b64 states[16];
    u32 pending;
    bool busy;
};

template <bool Launder>
__attribute__((always_inline)) static inline bool FindState_A3_b64(UnkSmMachine_A3_b64* self, UnkSmStatePtr_A3_b64* states,
                                                               u32 id, UnkSmState_A3_b64*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_A3_b64);
    asm("" : "+r"(n));
    UnkSmStatePtr_A3_b64* p = states ? states : self->states;
    if (Launder)
        asm("" : "+r"(p));
    for (; n != 0; ++p, n -= sizeof(UnkSmStatePtr_A3_b64)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

template <u32 None, bool Launder = false>
__attribute__((always_inline)) static inline void SwitchState_A3_b64(UnkSmMachine_A3_b64* self, UnkSmStatePtr_A3_b64* states,
                                                                 u32 id, UnkSmState_A3_b64* current) {
    if (!current || current->id != id) {
        UnkSmState_A3_b64* found;
        UnkSmState_A3_b64* next;
        u32 enter;
        if (FindState_A3_b64<Launder>(self, states, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_A3_b64*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

template <u32 Last, u32 None, bool Launder = false>
__attribute__((always_inline)) static inline void RequestStateAt_A3_b64(UnkSmMachine_A3_b64* self,
                                                                    UnkSmStatePtr_A3_b64* states, u32 id) {
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_A3_b64<None, Launder>(self, states, id, self->current);
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void RequestState_A3_b64(UnkSmMachine_A3_b64* self, u32 id) {
    RequestStateAt_A3_b64<Last, None>(self, nullptr, id);
}

struct UnkHydrogigaKey_A3_b64;
extern UnkHydrogigaKey_A3_b64 g_unk_710164a0e8;  // reached through the GOT

struct UnkHydrogigaOwner_A3_b64 {
    u8 pad_0x0[0x2e8];
    UnkTimelineComponent* timeline;
};

struct UnkHydrogigaSm_A3_b64 {
    u8 pad_0x0[0x30];
    UnkHydrogigaOwner_A3_b64* owner;
    u8 pad_0x38[0x149c4 - 0x38];
    f32 timer;  // 0x149c4
    u8 pad_0x149c8[0x14ca8 - 0x149c8];
    UnkSmMachine_A3_b64 machine;
};

#define UNK_HYDROGIGA_REQUEST_A3_B64(owner, id)                                                               \
    RequestStateAt_A3_b64<6, 7, true>(                                                                         \
        &(owner)->machine, reinterpret_cast<UnkSmStatePtr_A3_b64*>(reinterpret_cast<char*>(owner) + 0x14ccc), id)

// Once the timer reaches 0.5, sets g_unk_710164a0e8 to the all-flags mask on the owner's timeline (+0x2e8)
// and asks for state 1.
void fn_71005c2e80(UnkHydrogigaSm_A3_b64* self, f32 dt) {
    self->timer += dt;
    if (!(self->timer >= 0.5f))
        return;
    if (UnkTimelineComponent* timeline = self->owner->timeline) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a0e8), &mask.id, false, -1.0f);
    }
    UNK_HYDROGIGA_REQUEST_A3_B64(self, 1);
}
