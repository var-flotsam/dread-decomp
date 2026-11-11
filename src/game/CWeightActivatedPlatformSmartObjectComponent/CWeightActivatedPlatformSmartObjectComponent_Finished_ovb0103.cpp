// CWeightActivatedPlatformSmartObjectComponent's unit (tools/tu_map.py, unit 1516): whether the platform has
// finished, as the "Finished" property (g_unk_71727434e0) stored in the current scenario's blackboard section
// says (then fn_7100a0a08c runs), and the event handler that tells the linked smart object's user (its slot
// 0x178) the event's name and requests state 3 from the state machine at +0x180 (the template of
// src/game/UnkStateMachineSetA6_b62.cpp, copied here; last state 7, "none" 6). Names and layouts are
// placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CElectricReactionComponent.h"
#include "game/CWeightActivatedPlatformSmartObjectComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSlots.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkSmartObjectUserA;
struct UnkSmartObjectActor;

struct UnkSmState_ovb0103 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_ovb0103* __attribute__((aligned(4))) UnkSmStatePtr_ovb0103;

struct UnkSmMachine_ovb0103 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_ovb0103* previous, UnkSmState_ovb0103* next);
    virtual void vf_0x28(UnkSmState_ovb0103* current, UnkSmState_ovb0103* next);

    u64 field_0x8;
    UnkSmState_ovb0103* current;
    UnkSmState_ovb0103* previous;
    u32 count;
    UnkSmStatePtr_ovb0103 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_ovb0103 {
    u8 pad_0x0[Offset];
    UnkSmMachine_ovb0103 machine;
};

// Pin: the states array's address is not derived from another one the caller computed.
template <bool Pin>
__attribute__((always_inline)) static inline bool FindState_ovb0103(UnkSmMachine_ovb0103* self, u32 id,
                                                               UnkSmState_ovb0103*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_ovb0103);
    asm("" : "+r"(n));
    UnkSmStatePtr_ovb0103* p = self->states;
    if (Pin)
        asm("" : "+r"(p));
    for (; n != 0; ++p, n -= sizeof(UnkSmStatePtr_ovb0103)) {
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
__attribute__((always_inline)) static inline void SwitchState_ovb0103(UnkSmMachine_ovb0103* self, u32 id,
                                                                 UnkSmState_ovb0103* current) {
    if (!current || current->id != id) {
        UnkSmState_ovb0103* found;
        UnkSmState_ovb0103* next;
        u32 enter;
        if (FindState_ovb0103<Pin>(self, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_ovb0103*, s32);
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
__attribute__((always_inline)) static inline void RequestState_ovb0103(UnkSmMachine_ovb0103* self, u32 id) {
    // Stored through a plain u32 pointer, as in the template's original.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_ovb0103<None>(self, id, self->current);
}

struct UnkPlayer;
struct UnkPlatformUser_ovb0103 {
    virtual void vf_0x0();
    // The user's slot 0x178: told the player and the event's name.
    void vf_0x178(UnkPlayer* player, const base::global::CRntString* name) {
        reinterpret_cast<void (*)(UnkPlatformUser_ovb0103*, UnkPlayer*, const base::global::CRntString*)>(
            (*reinterpret_cast<void***>(this))[0x178 / 8])(this, player, name);
    }
    u8 pad_0x8[0xf8 - 0x8];
    bool busy;  // 0xf8
};

struct UnkPlatformLink_ovb0103 {
    u8 pad_0x0[0x10];
    UnkSmartObjectActor* object;
};

struct UnkPlatformSlotsOwner_ovb0103 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};
struct UnkPlatformSlotsComp_ovb0103 {
    u8 pad_0x0[0x50];
    UnkPlatformSlotsOwner_ovb0103* owner;
};
struct UnkPlatformEntity_ovb0103 {
    u8 pad_0x0[0x228];
    UnkPlatformSlotsComp_ovb0103* slots;
};

struct UnkPlatformValue_ovb0103 {
    CStrId name;
    u64 field_0x8;
};

struct UnkPlatform_ovb0103 {
    u8 pad_0x0[0x30];
    UnkPlatformEntity_ovb0103* entity;
    u8 pad_0x38[0x68 - 0x38];
    CStrId field_0x68;
    u8 pad_0x70[0xb0 - 0x70];
    UnkPlatformLink_ovb0103* link;  // 0xb0
    u8 pad_0xb8[0x174 - 0xb8];
    u32 slot;  // 0x174
    u8 pad_0x178[0x180 - 0x178];
    UnkSmMachine_ovb0103 machine;  // 0x180
    u8 pad_0x230[0x570 - 0x230];
    UnkPlatformValue_ovb0103* values;  // 0x570
    u64* keys;
    u32 count;
    u8 pad_0x584[0x5a0 - 0x584];
    u64 track;  // 0x5a0
};


CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);
extern UnkProperty g_unk_71727434e0;
struct UnkPlatformSm_b61;

void fn_7100a09f7c(UnkPlatform_ovb0103* self, CBlackboard* blackboard) {
    bool value = false;
    bool finished;
    {
        CStrId section = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
        u64 index = 0;
        u32 count = self->count;
        if (count) {
            u64* keys = self->keys;
            u64 key = (u64)g_unk_71727434e0.name.data;
            for (; index != count; ++index) {
                if (keys[index] == key)
                    break;
            }
        }
        UnkPlatformValue_ovb0103* values = self->values;
        CBlackboard::CSection* s = fn_7101047270(blackboard, section);
        bool ok = s && fn_710006d11c(s, &values[index].name, &value);
        finished = ok && value;
    }
    if (finished)
        fn_7100a0a08c(reinterpret_cast<UnkPlatformSm_b61*>(self));
}

UnkPlayer* fn_7100dec788(UnkGameManager*);
void fn_7100a046ec(UnkPlatform_ovb0103* self);

struct UnkPlatformEvent_ovb0103 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

bool fn_7100a0ab24(UnkPlatform_ovb0103* self, UnkPlatformEvent_ovb0103* event) {
    fn_7100a046ec(self);
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (!player || !self->link || !self->link->object)
        return true;
    auto* user = reinterpret_cast<UnkPlatformUser_ovb0103*>(fn_71007cfd8c(self->link->object));
    if (!user)
        return true;
    user->vf_0x178(player, &base::reflection::refl_RntStringOf(event->name));
    RequestState_ovb0103<7, 6>(&self->machine, 3);
    return true;
}
