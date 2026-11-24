// Unit 1454 (tools/tu_map.py): fn_7100984928 switches the state machine at +0xe0 to state 2 when the target is
// out of reach (fn_7100984d24), else to state 1, sends the owner's animation component the matching id and
// resets the values at +0x3c0. Names and layouts are placeholders.
// NON_MATCHING(major): the two sends' arguments share stack slots here; the original gives each its own.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1450.h"
#include "types.h"

struct UnkNpc_b0248;
bool fn_7100984d24(UnkNpc_b0248* self);

struct UnkIdHolder_l984928 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkMachine_l984928;

struct UnkState_l984928 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void OnEnter(s32 previous);
    virtual void OnExit(s32 next);

    UnkMachine_l984928* machine;
    u32 id;
};

struct UnkMachine_l984928 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEntered(UnkState_l984928* previous, UnkState_l984928* next);
    virtual void OnExited(UnkState_l984928* current, UnkState_l984928* next);

    void* owner;
    UnkState_l984928* current;
    UnkState_l984928* previous;
    u32 count;
    u32 states[0x20];  // UnkState_l984928* entries, 4-byte aligned
    u32 pending;
    bool busy;
    u8 pad_0xa9[0xb0 - 0xa9];

    UnkState_l984928** States() { return reinterpret_cast<UnkState_l984928**>(states); }

    // Looks up the registered state with `id`.
    bool Find(u32 id, UnkState_l984928*& out) {
        UnkState_l984928** p = States();
        UnkState_l984928** end = p + count;
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
        UnkState_l984928* next;
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
static_assert(sizeof(UnkMachine_l984928) == 0xb0, "layout");

struct UnkOwner_l984928 {
    u8 pad_0x0[0x220];
    void* animation;
};

struct UnkLink_l984928 {
    u8 pad_0x0[0x30];
    UnkOwner_l984928* owner;
};

struct UnkVec2_l984928 {
    f32 x, y;
};

struct Unk_l984928 {
    u8 pad_0x0[0xe0];
    UnkMachine_l984928 machine;  // 0xe0
    u8 pad_0x190[0x3c0 - 0x190];
    bool field_0x3c0;
    UnkVec2_l984928 field_0x3c4;
    f32 field_0x3cc;
    u8 pad_0x3d0[0x3dc - 0x3d0];
    bool out_of_reach;
};
static_assert(__builtin_offsetof(Unk_l984928, out_of_reach) == 0x3dc, "layout");

void fn_7100984928(Unk_l984928* self) {
    base::reflection::UnkReflTypeIdArg reach_mode;
    base::reflection::UnkReflTypeIdArg reach_mask;
    base::reflection::UnkReflTypeIdArg near_mode;
    base::reflection::UnkReflTypeIdArg near_mask;
    UnkLink_l984928* link = reinterpret_cast<UnkLink_l984928*>(fn_710097f198(reinterpret_cast<UnkSmBase_b60*>(self)));
    void* animation = link->owner->animation;
    if (fn_7100984d24(reinterpret_cast<UnkNpc_b0248*>(self))) {
        self->machine.Request(2);
        self->out_of_reach = false;
        reach_mode.id = 0;
        reach_mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(animation, 0x21b0cb7fad9ee1b6, 0xf4061ff25a2163d6, &reach_mode, &g_unk_710164b958,
                      &g_unk_7172769740, &reach_mask);
    } else {
        self->machine.Request(1);
        self->out_of_reach = true;
        u64 hash =
            (*reinterpret_cast<UnkIdHolder_l984928**>(reinterpret_cast<char*>(&GameManagerIdTable()) + 0x1280))->hash;
        near_mode.id = 0;
        near_mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(animation, hash, 0xf4061ff25a2163d6, &near_mode, &g_unk_710164b958, &g_unk_7172769740,
                      &near_mask);
    }
    self->field_0x3c0 = true;
    self->field_0x3c4.x = 0.0f;
    self->field_0x3c4.y = 0.0f;
    self->field_0x3cc = 0.0f;
}
