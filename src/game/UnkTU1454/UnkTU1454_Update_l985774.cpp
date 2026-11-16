// Unit 1454 (tools/tu_map.py): fn_7100985774 returns false while the owner's animation component plays an
// ignore-stop-grapple-impulse track; otherwise, when the requested value at +0x3ec changed, sends the animation
// component an id (when the grapple tunable plays swing animations), takes the new value and switches the
// state machine at +0xe0 to state 1. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1450.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

// CSamusGrappleMovementState::CTunableSamusGrappleMovementState, the tunable g_unk_717273ba88 is.
struct UnkGrappleTunable_l985774 {
    u8 pad_0x0[0x80];
    bool bSwingPlayAnimations;
};
extern base::reflection::UnkTunable g_unk_717273ba88;

static inline const UnkGrappleTunable_l985774& GrappleTunable() {
    return reinterpret_cast<const UnkGrappleTunable_l985774&>(g_unk_717273ba88);
}

struct UnkIdHolder_l985774 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkMachine_l985774;

struct UnkState_l985774 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void OnEnter(s32 previous);
    virtual void OnExit(s32 next);

    UnkMachine_l985774* machine;
    u32 id;
};

struct UnkMachine_l985774 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEntered(UnkState_l985774* previous, UnkState_l985774* next);
    virtual void OnExited(UnkState_l985774* current, UnkState_l985774* next);

    void* owner;
    UnkState_l985774* current;
    UnkState_l985774* previous;
    u32 count;
    u32 states[0x20];  // UnkState_l985774* entries, 4-byte aligned
    u32 pending;
    bool busy;
    u8 pad_0xa9[0xb0 - 0xa9];

    UnkState_l985774** States() { return reinterpret_cast<UnkState_l985774**>(states); }

    // Looks up the registered state with `id`.
    bool Find(u32 id, UnkState_l985774*& out) {
        UnkState_l985774** p = States();
        UnkState_l985774** end = p + count;
        while (p != end && (*p)->id != id)
            ++p;
        if (p == end)
            return false;
        out = *p;
        return out != nullptr;
    }

    void SwitchTo(u32 id) {
        if (current && current->id == id)
            return;
        UnkState_l985774* next;
        if (!Find(id, next))
            return;
        if (current) {
            current->OnExit(id);
            OnExited(current, next);
        }
        previous = current;
        current = next;
        next->OnEnter(previous ? previous->id : 3);
        OnEntered(previous, next);
    }

    void Request(u32 id) {
        pending = id;
        if (busy)
            return;
        SwitchTo(id);
        pending = 3;
    }
};
static_assert(sizeof(UnkMachine_l985774) == 0xb0, "layout");

struct UnkOwner_l985774 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkLink_l985774 {
    u8 pad_0x0[0x30];
    UnkOwner_l985774* owner;
};

struct Unk_l985774 {
    u8 pad_0x0[0xe0];
    UnkMachine_l985774 machine;  // 0xe0
    u8 pad_0x190[0x3d8 - 0x190];
    u32 current_value;
    u8 pad_0x3dc[0x3e9 - 0x3dc];
    bool field_0x3e9;
    u8 pad_0x3ea[0x3ec - 0x3ea];
    u32 requested_value;
};
static_assert(__builtin_offsetof(Unk_l985774, requested_value) == 0x3ec, "layout");

bool fn_7100985774(Unk_l985774* self) {
    UnkLink_l985774* link = reinterpret_cast<UnkLink_l985774*>(fn_710097f198(reinterpret_cast<UnkSmBase_b60*>(self)));
    UnkAnimationComponent* animation = link->owner->animation;
    if (animation && fn_710117b348(animation, fn_7100d138f8()))
        return false;
    if (self->requested_value != self->current_value) {
        if (GrappleTunable().bSwingPlayAnimations && animation) {
            u64 hash = reinterpret_cast<UnkIdHolder_l985774*>(GameManagerIdTable().field_0x1260.data)->hash;
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 1;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
        }
        self->current_value = self->requested_value;
        self->machine.Request(1);
    }
    self->field_0x3e9 = false;
    return true;
}
