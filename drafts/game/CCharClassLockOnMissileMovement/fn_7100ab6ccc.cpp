// CCharClassLockOnMissileMovement's unit: the movement's update. It releases the lock-on item once the
// target is marked, runs its state machine (at +0x760) and works out the velocity since the last update.
// Names and layouts are placeholders.
#include <math.h>

#include "game/UnkRenderManager.h"
#include "game/UnkStateMachineFind.h"
#include "types.h"

struct UnkSmState_B63a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_B63a4* __attribute__((aligned(4))) UnkSmStatePtr_B63a4;

struct UnkSmMachine_B63a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_B63a4* previous, UnkSmState_B63a4* next);
    virtual void vf_0x28(UnkSmState_B63a4* current, UnkSmState_B63a4* next);

    u64 field_0x8;
    UnkSmState_B63a4* current;
    UnkSmState_B63a4* previous;
    u32 count;
    UnkSmStatePtr_B63a4 states[16];
    u32 pending;
    bool busy;
};

static inline bool FindState_lab6ccc(UnkSmMachine_B63a4* self, u32 id, UnkSmState_B63a4*& out) {
    out = FindStateById(self->states, self->count, id);
    return out != nullptr;
}

// Switches to state `id` and clears the pending id.
template <u32 None>
static inline void SwitchState_lab6ccc(UnkSmMachine_B63a4* self, u32 id, UnkSmState_B63a4* current) {
    if (!current || current->id != id) {
        UnkSmState_B63a4* found;
        UnkSmState_B63a4* next;
        u32 enter;
        if (FindState_lab6ccc(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            next = nullptr;
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkSmState_B63a4*, s32);
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
static inline void Apply_lab6ccc(UnkSmMachine_B63a4* self) {
    u32 id = self->pending;
    if (id > Last)
        return;
    SwitchState_lab6ccc<None>(self, id, self->current);
}

template <u32 Last, u32 None>
static inline void Update_lab6ccc(UnkSmMachine_B63a4* self, f32 dt) {
    self->busy = true;
    if (self->pending != None)
        Apply_lab6ccc<Last, None>(self);
    u32 i = 0;
    while (UnkSmState_B63a4* current = self->current) {
        current->vf_0x20(dt);
        if (self->pending == None)
            break;
        Apply_lab6ccc<Last, None>(self);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}

struct UnkLockOnItem_b61 {
    u64 field_0x0;
    u8 item[8];
};

struct UnkLockOnMovement_ovb0121;
void CCharClassLockOnMissileMovement_HandLockToMultiLockonAbility(UnkLockOnMovement_ovb0121* self);

struct UnkVec3f_lab6ccc {
    f32 x;
    f32 y;
    f32 z;

    f32 Length() const { return sqrtf(x * x + y * y + z * z); }
};

inline UnkVec3f_lab6ccc operator-(const UnkVec3f_lab6ccc& a, const UnkVec3f_lab6ccc& b) {
    return UnkVec3f_lab6ccc{a.x - b.x, a.y - b.y, a.z - b.z};
}

inline UnkVec3f_lab6ccc operator*(const UnkVec3f_lab6ccc& v, f32 s) {
    return UnkVec3f_lab6ccc{s * v.x, s * v.y, s * v.z};
}

// A 3x4 transform; the last column is the position.
struct UnkMtx34_lab6ccc {
    f32 m[3][4];

    UnkVec3f_lab6ccc Position() const { return UnkVec3f_lab6ccc{m[0][3], m[1][3], m[2][3]}; }
};

struct UnkLockOnOwner_lab6ccc {
    u8 pad_0x0[0x38];
    UnkMtx34_lab6ccc transform;  // 0x38
};

// The movement's state, as far as its update reads it.
struct UnkLockOnMovement_lab6ccc {
    u8 pad_0x0[0x30];
    UnkLockOnOwner_lab6ccc* owner;  // 0x30
    u8 pad_0x38[0x9c - 0x38];
    UnkMtx34_lab6ccc last_transform;  // 0x9c
    u8 pad_0xcc[0x10c - 0xcc];
    UnkVec3f_lab6ccc velocity;  // 0x10c
    f32 speed;                  // 0x118
    u8 pad_0x11c[0x608 - 0x11c];
    bool field_0x608;
    u8 pad_0x609[0x760 - 0x609];
    UnkSmMachine_B63a4 machine;  // 0x760
    u8 pad_0x810[0x858 - 0x810];
    UnkLockOnItem_b61* field_0x858;
};
static_assert(__builtin_offsetof(UnkLockOnMovement_lab6ccc, machine) == 0x760, "layout");

// The lock-on missile's own movement, which extends it.
struct UnkLockOnMissile_lab6ccc : UnkLockOnMovement_lab6ccc {
    u8 pad_0x860[0x132f8 - 0x860];
    bool field_0x132f8;
};
static_assert(__builtin_offsetof(UnkLockOnMissile_lab6ccc, field_0x132f8) == 0x132f8, "layout");

// Gives the lock-on item back (to the MultiLockon ability once the target is marked) and drops it.
inline void ReleaseItem_lab6ccc(UnkLockOnMovement_lab6ccc* self) {
    if (static_cast<UnkLockOnMissile_lab6ccc*>(self)->field_0x132f8)
        CCharClassLockOnMissileMovement_HandLockToMultiLockonAbility(reinterpret_cast<UnkLockOnMovement_ovb0121*>(self));
    if (UnkLockOnItem_b61* item = self->field_0x858) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(item->item), 0);
        self->field_0x858 = nullptr;
    }
}

void fn_7100ab6ccc(UnkLockOnMovement_lab6ccc* self, f32 dt) {
    if (self->field_0x858 && self->field_0x608)
        ReleaseItem_lab6ccc(self);
    Update_lab6ccc<2, 0x7fffffff>(&self->machine, dt);
    if (dt > 0.0f) {
        UnkVec3f_lab6ccc moved = self->owner->transform.Position() - self->last_transform.Position();
        self->velocity = moved * (1.0f / dt);
        self->speed = self->velocity.Length();
    } else {
        self->velocity = UnkVec3f_lab6ccc{};
        self->speed = 0.0f;
    }
}