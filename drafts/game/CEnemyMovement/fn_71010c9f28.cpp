// CEnemyMovement unit: the movement's update. After the base update it runs the movement state machine
// (+0x838), applying up to ten requested state changes, keeps the moving and stopped times, and tells
// the listeners (+0x7b8) what the movement's kind (+0x6f8) asks for. Names and layouts are placeholders.
#include "base/UnkCountedDeferredSet.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkMovementComponent.h"
#include "types.h"

struct UnkMovementStateMachine_i;
void fn_71010c9c34(UnkMovementStateMachine_i* self, f32 dt);
void fn_71010ce798(base::UnkCountedDeferredSet<void>* set, void* item);
void fn_71010ce92c(base::UnkCountedDeferredWalk<void>* walk);

struct UnkMoveAI_l0c9f28;
bool fn_7101105a94(UnkMoveAI_l0c9f28* ai);

static const u32 kNoState = 5;

struct UnkMoveState_l0c9f28 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(u32 from);  // vf_0x10
    virtual void Exit();           // vf_0x18
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void PreUpdate(f32 dt);  // vf_0x50
    virtual void Update(f32 dt);     // vf_0x58
    void* field_0x8;
    u32 id;  // 0x10
};

struct UnkMoveStateMachine_l0c9f28 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEnter(UnkMoveState_l0c9f28* from, UnkMoveState_l0c9f28* to);  // vf_0x20
    virtual void OnExit(UnkMoveState_l0c9f28* from, UnkMoveState_l0c9f28* to);   // vf_0x28
    u64 field_0x8;
    UnkMoveState_l0c9f28* current;   // 0x10
    UnkMoveState_l0c9f28* previous;  // 0x18
    u32 count;                       // 0x20
    UnkMoveState_l0c9f28* states[16] __attribute__((packed, aligned(4)));  // 0x24
    u32 requested;  // 0xa4
    bool updating;  // 0xa8

    u32 CurrentId() const { return current ? current->id : kNoState; }

    UnkMoveState_l0c9f28* Find(u32 id) {
        for (UnkMoveState_l0c9f28 **it = states, **end = states + count; it != end; ++it) {
            if ((*it)->id == id)
                return *it;
        }
        return nullptr;
    }

    void ChangeTo(UnkMoveState_l0c9f28* next) {
        UnkMoveState_l0c9f28* old = current;
        if (old) {
            old->Exit();
            OnExit(current, next);
            old = current;
        }
        previous = old;
        current = next;
        if (next) {
            next->Enter(old ? old->id : kNoState);
            OnEnter(previous, next);
        }
    }

    void PreUpdate(f32 dt) {
        updating = true;
        if (current)
            current->PreUpdate(dt);
    }

    void Update(f32 dt) {
        updating = true;
        if (current)
            current->Update(dt);
        updating = false;
    }
};
static_assert(__builtin_offsetof(UnkMoveStateMachine_l0c9f28, requested) == 0xa4, "layout");

struct UnkMoveStopper_l0c9f28 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void Stop(f32 dt);  // vf_0x98
};

struct UnkMoveOwner_l0c9f28 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkMoveAI_l0c9f28* ai;  // 0x240
    u8 pad_0x248[0x50];
    UnkMoveStopper_l0c9f28* stopper;  // 0x298
};

// A listener: told the movement's event, it says whether to keep it.
struct UnkMoveListener_l0c9f28 {
    void* field_0x0;
    void* context;                          // 0x8
    bool (*callback)(void* context, u32 event);  // 0x10
};

struct UnkEnemyMovement_l0c9f28 {
    u8 pad_0x0[0x30];
    UnkMoveOwner_l0c9f28* owner;  // 0x30
    u8 pad_0x38[0x396 - 0x38];
    bool field_0x396;
    u8 pad_0x397[0x6f8 - 0x397];
    u32 kind;  // 0x6f8
    u8 pad_0x6fc[0x7b8 - 0x6fc];
    base::UnkCountedDeferredSet<void> listeners;  // 0x7b8
    UnkMoveStateMachine_l0c9f28 machine;          // 0x838
    u8 pad_0x8e8[0x971 - 0x8e8];
    bool field_0x971;
    u8 pad_0x972[0x19e0 - 0x972];
    void* state_context;                                     // 0x19e0
    void (*on_state_changed)(void* context, u32 from, u32 to);  // 0x19e8
    f32 moving_time;   // 0x19f0
    f32 stopped_time;  // 0x19f4
    u8 pad_0x19f8[0x1be1 - 0x19f8];
    bool field_0x1be1;
    u8 pad_0x1be2[0x1c08 - 0x1be2];
    bool field_0x1c08;
};
static_assert(__builtin_offsetof(UnkEnemyMovement_l0c9f28, machine) == 0x838, "layout");
static_assert(__builtin_offsetof(UnkEnemyMovement_l0c9f28, field_0x1c08) == 0x1c08, "layout");

static inline void Notify(UnkEnemyMovement_l0c9f28* self, u32 event) {
    base::UnkCountedDeferredWalk<void> walk(&self->listeners);
    for (void* item : self->listeners.items) {
        auto* listener = static_cast<UnkMoveListener_l0c9f28*>(item);
        if (!listener || !listener->callback || !listener->callback(listener->context, event))
            fn_71010ce798(&self->listeners, listener);
    }
    fn_71010ce92c(&walk);
}

void fn_71010c9f28(UnkEnemyMovement_l0c9f28* self, f32 dt) {
    fn_71010d1a90(reinterpret_cast<UnkMovementComponent*>(self), dt);
    self->field_0x396 = false;
    if (!self->owner || !self->owner->animation)
        return;

    UnkMoveStateMachine_l0c9f28* machine = &self->machine;
    machine->Update(dt);
    u32 state = machine->CurrentId();
    u32 passes = 0;
    while (machine->requested != kNoState) {
        u32 from = machine->CurrentId();
        u32 requested = machine->requested;
        if (requested <= 4) {
            if (machine->CurrentId() != requested) {
                UnkMoveState_l0c9f28* next = machine->Find(requested);
                if (next || requested == 0)
                    machine->ChangeTo(next);
            }
            machine->requested = kNoState;
        }
        if (self->on_state_changed && machine->CurrentId() != from)
            self->on_state_changed(self->state_context, from, machine->CurrentId());

        if (state == 4 && machine->CurrentId() == 1) {
            if (UnkMoveStopper_l0c9f28* stopper = self->owner->stopper)
                stopper->Stop(0.0f);
            UnkMoveOwner_l0c9f28* owner = self->owner;
            bool busy = owner && owner->ai && fn_7101105a94(owner->ai);
            if (!(busy || machine->CurrentId() != 1)) {
                fn_71010c9c34(reinterpret_cast<UnkMovementStateMachine_i*>(machine), 0.0f);
                machine->PreUpdate(0.0f);
                machine->Update(0.0f);
            }
        }
        state = machine->CurrentId();
        if (++passes > 9)
            break;
    }

    if (fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(self))) {
        self->moving_time += dt;
        self->stopped_time = 0.0f;
    } else {
        self->stopped_time += dt;
        self->moving_time = 0.0f;
    }
    self->field_0x1c08 = machine->CurrentId() == 1 ? self->field_0x971 : self->field_0x1c08;

    // Kinds 3 to 5 send the same event.
    switch (self->kind) {
    case 1:
        Notify(self, 1);
        break;
    case 2:
        Notify(self, 2);
        break;
    case 3:
        Notify(self, 3);
        break;
    case 4:
        Notify(self, 3);
        break;
    case 5:
        Notify(self, 3);
        break;
    case 6:
        if (!fn_710117b348(self->owner->animation, fn_7100d13e08()))
            Notify(self, 6);
        break;
    }
    self->field_0x1be1 = false;
}
