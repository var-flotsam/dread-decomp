// CEnemyMovement unit: once the owner (+0x1c18) is known, binds the movement's states to the actor and the owner
// and adds each one its machine does not have yet: six states to the machine at +0x700, four (which also
// know the movement) to the machine at +0x838. Names and layouts are placeholders.
#include "types.h"

struct UnkStateMachine_l0c8314;
struct UnkActor_l0c8314;
struct UnkOwner_l0c8314;
struct UnkEnemyMovement_l0c8314;

struct UnkState_l0c8314 {
    void** vtable;
    UnkStateMachine_l0c8314* machine;  // 0x8
    u32 id;                            // 0x10
    u8 pad_0x14[0x30 - 0x14];
    UnkActor_l0c8314* actor;  // 0x30
};

// A state of the machine at +0x700.
struct UnkMoveState_l0c8314 : UnkState_l0c8314 {
    u8 pad_0x38[0x8];
    UnkOwner_l0c8314* owner;  // 0x40

    void Bind(UnkActor_l0c8314* actor_, UnkOwner_l0c8314* owner_) {
        actor = actor_;
        owner = owner_;
    }
};
static_assert(sizeof(UnkMoveState_l0c8314) == 0x48, "layout");

// A state of the machine at +0x838: it knows the movement too.
struct UnkMoveStateB_l0c8314 : UnkState_l0c8314 {
    UnkEnemyMovement_l0c8314* movement;  // 0x38
    UnkOwner_l0c8314* owner;             // 0x40

    void Bind(UnkActor_l0c8314* actor_, UnkEnemyMovement_l0c8314* movement_, UnkOwner_l0c8314* owner_) {
        actor = actor_;
        movement = movement_;
        owner = owner_;
    }
};

typedef UnkState_l0c8314* __attribute__((aligned(4))) UnkStatePtr_l0c8314;

struct UnkStateMachine_l0c8314 {
    void** vtable;
    UnkEnemyMovement_l0c8314* owner;  // 0x8
    UnkState_l0c8314* current;        // 0x10
    UnkState_l0c8314* previous;       // 0x18
    u32 count;                        // 0x20
    UnkStatePtr_l0c8314 states[16];   // 0x24
    u32 pending;                      // 0xa4
    bool busy;                        // 0xa8

    // Adds a state unless one with its id is there already.
    void Add(UnkState_l0c8314* state) {
        u32 id = state->id;
        UnkStatePtr_l0c8314* end = states + count;
        for (UnkStatePtr_l0c8314* p = states; p < end; ++p) {
            if ((*p)->id == id)
                return;
        }
        state->machine = this;
        *end = state;
        count++;
    }
};
static_assert(sizeof(UnkStateMachine_l0c8314) == 0xb0, "layout");

struct UnkEnemyMovement_l0c8314 {
    u8 pad_0x0[0x30];
    UnkActor_l0c8314* actor;  // 0x30
    u8 pad_0x38[0x700 - 0x38];
    UnkStateMachine_l0c8314 machine;  // 0x700
    u8 pad_0x7b0[0x838 - 0x7b0];
    UnkStateMachine_l0c8314 machine_b;  // 0x838
    UnkMoveStateB_l0c8314 state_b0;  // 0x8e8
    u8 pad_0x930[0x15b0 - 0x930];
    UnkMoveStateB_l0c8314 state_b1;  // 0x15b0
    u8 pad_0x15f8[0x16d0 - 0x15f8];
    UnkMoveStateB_l0c8314 state_b2;  // 0x16d0
    u8 pad_0x1718[0x17a0 - 0x1718];
    UnkMoveStateB_l0c8314 state_b3;  // 0x17a0
    u8 pad_0x17e8[0x1a10 - 0x17e8];
    UnkMoveState_l0c8314 states[6];  // 0x1a10
    u8 pad_0x1bc0[0x1c18 - 0x1bc0];
    UnkOwner_l0c8314* owner;  // 0x1c18
};
static_assert(__builtin_offsetof(UnkEnemyMovement_l0c8314, state_b0) == 0x8e8, "layout");
static_assert(__builtin_offsetof(UnkEnemyMovement_l0c8314, states) == 0x1a10, "layout");
static_assert(__builtin_offsetof(UnkEnemyMovement_l0c8314, owner) == 0x1c18, "layout");

void fn_71010c8314(UnkEnemyMovement_l0c8314* self) {
    if (!self->owner)
        return;
    UnkActor_l0c8314* actor = self->actor;

    self->machine.owner = self;
    for (UnkMoveState_l0c8314& state : self->states) {
        state.Bind(actor, self->owner);
        self->machine.Add(&state);
    }

    self->machine_b.owner = self;
    UnkMoveStateB_l0c8314* states_b[] = {&self->state_b0, &self->state_b1, &self->state_b2, &self->state_b3};
    for (UnkMoveStateB_l0c8314* state : states_b) {
        state->Bind(actor, self, self->owner);
        self->machine_b.Add(state);
    }
}
