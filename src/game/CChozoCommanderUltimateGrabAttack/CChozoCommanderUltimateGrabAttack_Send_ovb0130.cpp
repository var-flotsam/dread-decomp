// CChozoCommanderUltimateGrabAttack's unit (tools/tu_map.py, unit 658): the attack's requests by kind to
// the owner's and the player's character classes, and its message handler (the state-machine template of
// src/game/UnkStateMachineSet_b61.cpp). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkGameManager.h"
#include "types.h"

extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT
extern const u64 g_unk_7101649600[] __attribute__((visibility("hidden")));  // a hash per kind

// Twelve bytes (copied as a whole: an 8-byte and a 4-byte move).
struct UnkUltimateGrabValue_ovb0130 {
    u32 field_0x0;
    u32 field_0x4;
    f32 value;
};

struct UnkUltimateGrabOwner_ovb0130 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkUltimateGrab_ovb0130 {
    u8 pad_0x0[0x8];
    UnkUltimateGrabOwner_ovb0130* owner;
    u8 pad_0x10[0x6d0 - 0x10];
    f32 field_0x6d0;
};

// fn_7100440140: sends the owner's character class (+0x220) the hash for a kind (kind 12 has its own
// while +0x6d0 is positive; kinds 0, 1, 2, 5 and 10 send nothing) with the value, or the default block
// for -1, as fn_710043272c (src/game/CChozoCommanderKiGrabAttack/CChozoCommanderKiGrabAttack_Hash_b59.cpp).
bool fn_7100440140(UnkUltimateGrab_ovb0130* self, s32 kind, f32 value) {
    u64 hash;
    if (kind == 12 && self->field_0x6d0 > 0.0f) {
        hash = 0x71981e37466484f0;
    } else {
        if ((0x427ul >> kind) & 1)
            return false;
        hash = g_unk_7101649600[kind];
    }
    void* target = self->owner->field_0x220;
    base::reflection::UnkReflTypeIdArg mode;
    UnkUltimateGrabValue_ovb0130 param;
    if (value != -1.0f) {
        param.field_0x0 = 0;
        param.field_0x4 = 0;
        param.value = value;
    } else {
        param = reinterpret_cast<const UnkUltimateGrabValue_ovb0130&>(g_unk_7172769740);
    }
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968, &param, &mask);
}

extern const u64 g_unk_7101649598[] __attribute__((visibility("hidden")));  // a hash per kind
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

// fn_710043ffa4: the same request sent to the player's character class (fn_7100dec788 of the game
// manager), with its own table and kind-12 hash; kinds 0 to 2 send nothing.
bool fn_710043ffa4(UnkUltimateGrab_ovb0130* self, s32 kind, f32 value) {
    u64 hash;
    if (kind == 12 && self->field_0x6d0 > 0.0f) {
        hash = 0xd5c4b35ace782264;
    } else {
        if ((u32)kind < 3)
            return false;
        hash = g_unk_7101649598[kind];
    }
    void* target = reinterpret_cast<UnkUltimateGrabOwner_ovb0130*>(fn_7100dec788(g_unk_7101d47310))->field_0x220;
    base::reflection::UnkReflTypeIdArg mode;
    UnkUltimateGrabValue_ovb0130 param;
    if (value != -1.0f) {
        param.field_0x0 = 0;
        param.field_0x4 = 0;
        param.value = value;
    } else {
        param = reinterpret_cast<const UnkUltimateGrabValue_ovb0130&>(g_unk_7172769740);
    }
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968, &param, &mask);
}

struct UnkSmState_ovb0130 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_ovb0130* __attribute__((aligned(4))) UnkSmStatePtr_ovb0130;

struct UnkSmMachine_ovb0130 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_ovb0130* previous, UnkSmState_ovb0130* next);
    virtual void vf_0x28(UnkSmState_ovb0130* current, UnkSmState_ovb0130* next);

    u64 field_0x8;
    UnkSmState_ovb0130* current;
    UnkSmState_ovb0130* previous;
    u32 count;
    UnkSmStatePtr_ovb0130 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_ovb0130 {
    u8 pad_0x0[Offset];
    UnkSmMachine_ovb0130 machine;
};

__attribute__((always_inline)) static inline bool FindState_ovb0130(UnkSmMachine_ovb0130* self, u32 id,
                                                               UnkSmState_ovb0130*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_ovb0130);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_ovb0130* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_ovb0130)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` (the b59 "apply pending" body) and clears the pending id.
template <u32 None>
__attribute__((always_inline)) static inline void SwitchState_ovb0130(UnkSmMachine_ovb0130* self, u32 id,
                                                                 UnkSmState_ovb0130* current) {
    if (!current || current->id != id) {
        UnkSmState_ovb0130* found;
        UnkSmState_ovb0130* next;
        u32 enter;
        if (FindState_ovb0130(self, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_ovb0130*, s32);
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
__attribute__((always_inline)) static inline void RequestState_ovb0130(UnkSmMachine_ovb0130* self, u32 id) {
    // Stored through a plain u32 pointer: the original rereads a state id the caller tested after this.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_ovb0130<None>(self, id, self->current);
}

struct UnkGrabMessageDesc_ovb0130 {
    u8 pad_0x0[0x30];
    u64 kind;  // 0x30
};

struct UnkGrabMessage_ovb0130 {
    s32 type;
    u8 pad_0x4[0xc];
    UnkGrabMessageDesc_ovb0130* desc;  // 0x10
};

// As src/game/CChozoCommanderAIComponent/CChozoCommanderAIComponent_Grab_ovb0073.cpp declares it.
template <u32 Offset>
struct UnkSmGrab_A1_b64;
void fn_7100440f5c(UnkSmGrab_A1_b64<0x88>*);

// fn_71004402c4: on a type-1 message of the kind 0xf4061ff25a2163d6, the attack's machine (+0x88) reacts by
// its current state: states 6, 11 and 12 call fn_7100440f5c, state 7 requests state 8.
bool fn_71004402c4(UnkSmOwner_ovb0130<0x88>* self, UnkGrabMessage_ovb0130* message) {
    if (message->desc->kind == 0xf4061ff25a2163d6 && message->type == 1) {
        if (UnkSmState_ovb0130* state = self->machine.current) {
            switch (state->id) {
            case 6:
                fn_7100440f5c(reinterpret_cast<UnkSmGrab_A1_b64<0x88>*>(self));
                break;
            case 7:
                RequestState_ovb0130<8, 0x7fffffff>(&self->machine, 8);
                break;
            case 11:
                fn_7100440f5c(reinterpret_cast<UnkSmGrab_A1_b64<0x88>*>(self));
                break;
            case 12:
                fn_7100440f5c(reinterpret_cast<UnkSmGrab_A1_b64<0x88>*>(self));
                break;
            }
        }
    }
    return true;
}
