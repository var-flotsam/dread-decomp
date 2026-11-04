// CMorphBallLauncherComponent's unit: a message handler. When the message's object is a collision
// component whose owner has its +0x210 set and the launcher's state machine (+0x208, last state 4, "none" 5)
// is in state 1, the launcher keeps a weak handle to that owner (+0x310) and requests state 2. It always
// answers false. The request template is src/game/UnkStateMachineSet_b60.cpp's, copied under this file's
// names. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSmState_A5m_b64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_A5m_b64* __attribute__((aligned(4))) UnkSmStatePtr_A5m_b64;

struct UnkSmMachine_A5m_b64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_A5m_b64* previous, UnkSmState_A5m_b64* next);
    virtual void vf_0x28(UnkSmState_A5m_b64* current, UnkSmState_A5m_b64* next);

    u64 field_0x8;
    UnkSmState_A5m_b64* current;
    UnkSmState_A5m_b64* previous;
    u32 count;
    UnkSmStatePtr_A5m_b64 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_A5m_b64 {
    u8 pad_0x0[Offset];
    UnkSmMachine_A5m_b64 machine;
};

__attribute__((always_inline)) static inline bool FindState_A5m_b64(UnkSmMachine_A5m_b64* self, u32 id,
                                                               UnkSmState_A5m_b64*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_A5m_b64);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_A5m_b64* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_A5m_b64)) {
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
__attribute__((always_inline)) static inline void SwitchState_A5m_b64(UnkSmMachine_A5m_b64* self, u32 id,
                                                                 UnkSmState_A5m_b64* current) {
    if (!current || current->id != id) {
        UnkSmState_A5m_b64* found;
        UnkSmState_A5m_b64* next;
        u32 enter;
        if (FindState_A5m_b64(self, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_A5m_b64*, s32);
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
__attribute__((always_inline)) static inline void RequestState_A5m_b64(UnkSmMachine_A5m_b64* self, u32 id) {
    // Stored through a plain u32 pointer: the original rereads a state id the caller tested after this.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_A5m_b64<None>(self, id, self->current);
}

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);

struct UnkLauncherTarget_A5_b64 {
    u8 pad_0x0[0x210];
    void* field_0x210;
};

struct UnkLauncherCollision_A5_b64 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x28];
    UnkLauncherTarget_A5_b64* owner;  // 0x30
};

struct UnkLauncherMessageBody_A5_b64 {
    u8 pad_0x0[0x10];
    UnkLauncherCollision_A5_b64* object;
};

struct UnkLauncherMessageData_A5_b64 {
    u8 pad_0x0[0x10];
    UnkLauncherMessageBody_A5_b64* body;
};

struct UnkLauncherMessage_A5_b64 {
    u8 pad_0x0[0x10];
    UnkLauncherMessageData_A5_b64* data;
};

struct UnkLauncherOwner_A5_b64 {
    u8 pad_0x0[0x220];
    void* field_0x220;  // what the request goes to
};

struct UnkLauncher_A5_b64 {
    u8 pad_0x0[0x30];
    UnkLauncherOwner_A5_b64* owner;
    u8 pad_0x38[0x208 - 0x38];
    UnkSmMachine_A5m_b64 machine;
    u8 pad_0x2b8[0x2f8 - 0x2b8];
    base::global::CStrId field_0x2f8;
    u8 pad_0x300[0x310 - 0x300];
    UnkWeakHandle* target;
};

// The b59 "apply pending" (src/game/UnkStateMachineUpdate_b59.cpp) of the same machine.
template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void ApplyPending_A5m_b64(UnkSmMachine_A5m_b64* self) {
    u32 id = self->pending;
    if (id > Last)
        return;
    SwitchState_A5m_b64<None>(self, id, self->current);
}

extern const u64 g_unk_710164aec0 __attribute__((visibility("hidden")));  // a timeline key

bool fn_71008888d8(UnkLauncher_A5_b64* self, UnkLauncherMessage_A5_b64* message) {
    UnkLauncherCollision_A5_b64* object = message->data->body->object;
    if (!object || !fn_710009d50c(object->vf_0x0(), fn_710085fc44()))
        return false;
    UnkLauncherTarget_A5_b64* owner = object->owner;
    if (!owner || !owner->field_0x210)
        return false;
    UnkSmState_A5m_b64* current = self->machine.current;
    if (!current || current->id != 1)
        return false;
    AssignRef(&self->target, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(owner)));
    RequestState_A5m_b64<4, 5>(&self->machine, 2);
    return false;
}

// Starts the launcher: sends the owner's request (two hashes, mode 0), and when the id at +0x2f8 is empty
// requests state 1 and applies it even while the machine is busy; then clears the flag at +0x104 of the
// linked actor's +0x210 object and gives the actor the timeline key g_unk_710164aec0.
void fn_7100888524(UnkLauncher_A5_b64* self) {
    UnkSpitSend(self->owner->field_0x220, 0x0cdb7b8dc45963ec, 0x71b15e29dc542a11);
    if (self->field_0x2f8.data == &g_unk_7101d47108->empty) {
        RequestState_A5m_b64<4, 5>(&self->machine, 1);
        ApplyPending_A5m_b64<4, 5>(&self->machine);
    }
    if (self->target && self->target->target) {
        char* actor = reinterpret_cast<char*>(self->target->target);
        (*reinterpret_cast<u8**>(actor + 0x210))[0x104] = 0;
        UnkWeakTarget* t = self->target ? self->target->target : nullptr;
        fn_7100d7c828(reinterpret_cast<UnkActor*>(t), &g_unk_710164aec0, false);
    }
}
