// UnkTU1501 (CSamusMovement's unit, tools/tu_map.py unit 1501): sets up the movement's state machine
// (+0x2cf8): registers each of its fourteen states (each told the owner, the movement and its tuning)
// unless one with the same id is already there, then switches to state 2. Names and layouts are
// placeholders.
#include "types.h"

struct UnkSamusMovement2;
struct UnkSamusMovementTuning;
struct UnkMoveMachine_l9e0754;
struct UnkSamusMovement_l9e0754;

// The id of no state.
constexpr s32 kNoState_l9e0754 = 0xf;

struct UnkMoveState_l9e0754 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(s32 previous_id);  // 0x10
    virtual void Exit(s32 next_id);       // 0x18
    UnkMoveMachine_l9e0754* machine;      // 0x8
    s32 id;                               // 0x10
    u8 pad_0x14[0x1c];
    void* owner;                          // 0x30
    UnkSamusMovement_l9e0754* movement;   // 0x38
    UnkSamusMovementTuning* tuning;       // 0x40
};
static_assert(sizeof(UnkMoveState_l9e0754) == 0x48, "layout");

// A state pointer at a 4-byte aligned offset.
typedef UnkMoveState_l9e0754* UnkMoveStatePtr_l9e0754 __attribute__((aligned(4)));

// A fixed list of up to 16 states.
struct UnkMoveStateList_l9e0754 {
    u32 count;
    UnkMoveStatePtr_l9e0754 data[16];

    UnkMoveStatePtr_l9e0754* begin() { return data; }
    UnkMoveStatePtr_l9e0754* end() { return data + count; }
    void push_back(UnkMoveState_l9e0754* state) {
        data[count] = state;
        ++count;
    }
};

struct UnkMoveMachine_l9e0754 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEntered(UnkMoveState_l9e0754* previous, UnkMoveState_l9e0754* next);  // 0x20
    virtual void OnLeaving(UnkMoveState_l9e0754* current, UnkMoveState_l9e0754* next);   // 0x28
    UnkSamusMovement_l9e0754* owner;     // 0x8
    UnkMoveState_l9e0754* current;       // 0x10
    UnkMoveState_l9e0754* previous;      // 0x18
    UnkMoveStateList_l9e0754 states;     // 0x20
    s32 pending;                         // 0xa4
    bool locked;                         // 0xa8

    UnkMoveState_l9e0754* Find(s32 id) {
        for (UnkMoveState_l9e0754* state : states)
            if (state->id == id)
                return state;
        return nullptr;
    }

    void Add(UnkMoveState_l9e0754* state) {
        for (UnkMoveState_l9e0754* other : states)
            if (other->id == state->id)
                return;
        state->machine = this;
        states.push_back(state);
    }

    void SetState(s32 id) {
        pending = id;
        if (locked)
            return;
        UnkMoveState_l9e0754* cur = current;
        if (!cur || cur->id != id) {
            if (UnkMoveState_l9e0754* next = Find(id)) {
                if (cur) {
                    cur->Exit(id);
                    OnLeaving(current, next);
                    cur = current;
                }
                previous = cur;
                current = next;
                next->Enter(cur ? cur->id : kNoState_l9e0754);
                OnEntered(previous, next);
            }
        }
        pending = kNoState_l9e0754;
    }
};
static_assert(sizeof(UnkMoveMachine_l9e0754) == 0xb0, "layout");

struct UnkSamusMovement_l9e0754 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x2cf8 - 0x38];
    UnkMoveMachine_l9e0754 machine;  // 0x2cf8
    UnkMoveState_l9e0754 state_0x2da8;
    u8 pad_0x2df0[0x2fc8 - 0x2df0];
    UnkMoveState_l9e0754 state_0x2fc8;
    u8 pad_0x3010[0x3170 - 0x3010];
    UnkMoveState_l9e0754 state_0x3170;
    u8 pad_0x31b8[0x3420 - 0x31b8];
    UnkMoveState_l9e0754 state_0x3420;
    u8 pad_0x3468[0x35d0 - 0x3468];
    UnkMoveState_l9e0754 state_0x35d0;
    u8 pad_0x3618[0x4188 - 0x3618];
    UnkMoveState_l9e0754 state_0x4188;
    u8 pad_0x41d0[0x4370 - 0x41d0];
    UnkMoveState_l9e0754 state_0x4370;
    u8 pad_0x43b8[0x44f8 - 0x43b8];
    UnkMoveState_l9e0754 state_0x44f8;
    u8 pad_0x4540[0x4688 - 0x4540];
    UnkMoveState_l9e0754 state_0x4688;
    u8 pad_0x46d0[0x4810 - 0x46d0];
    UnkMoveState_l9e0754 state_0x4810;
    u8 pad_0x4858[0x4998 - 0x4858];
    UnkMoveState_l9e0754 state_0x4998;
    u8 pad_0x49e0[0x4b20 - 0x49e0];
    UnkMoveState_l9e0754 state_0x4b20;
    u8 pad_0x4b68[0x4ca8 - 0x4b68];
    UnkMoveState_l9e0754 state_0x4ca8;
    u8 pad_0x4cf0[0x4ff0 - 0x4cf0];
    UnkMoveState_l9e0754 state_0x4ff0;
    u8 pad_0x5038[0x5458 - 0x5038];
    UnkSamusMovementTuning* tuning;  // 0x5458

    void AddState(UnkMoveState_l9e0754* state, void* actor) {
        state->owner = actor;
        state->movement = this;
        state->tuning = tuning;
        machine.Add(state);
    }
};
static_assert(__builtin_offsetof(UnkSamusMovement_l9e0754, tuning) == 0x5458, "layout");

void fn_71009e0754(UnkSamusMovement2* movement) {
    UnkSamusMovement_l9e0754* self = reinterpret_cast<UnkSamusMovement_l9e0754*>(movement);
    self->machine.owner = self;
    void* owner = self->owner;
    self->AddState(&self->state_0x2da8, owner);
    self->AddState(&self->state_0x2fc8, owner);
    self->AddState(&self->state_0x3170, owner);
    self->AddState(&self->state_0x3420, owner);
    self->AddState(&self->state_0x35d0, owner);
    self->AddState(&self->state_0x4188, owner);
    self->AddState(&self->state_0x4370, owner);
    self->AddState(&self->state_0x44f8, owner);
    self->AddState(&self->state_0x4688, owner);
    self->AddState(&self->state_0x4810, owner);
    self->AddState(&self->state_0x4998, owner);
    self->AddState(&self->state_0x4b20, owner);
    self->AddState(&self->state_0x4ca8, owner);
    self->AddState(&self->state_0x4ff0, owner);
    UnkMoveMachine_l9e0754* machine = &self->machine;
    machine->SetState(2);
}
