// behaviortree::CAutoolGoToRepairPointAction's unit (tools/tu_map.py): updates the action's state machine
// (+0x2d0, at most ten steps), then, unless its state 5 is current, sends the owner's navigation object the
// repair point to go to (a parameter block of the vtable-0x7101a25b38 kind). Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkStateMachineFind.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::UnkReflValue;

extern void* g_unk_7101a25b38[];
extern base::reflection::UnkTunable g_unk_71726beb18;

// The state machine.

struct UnkRepairState_l3c0d3c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkRepairState_l3c0d3c* __attribute__((aligned(4))) UnkRepairStatePtr_l3c0d3c;

enum : u32 { kLastState = 6, kNoState = 7 };

struct UnkRepairMachine_l3c0d3c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkRepairState_l3c0d3c* previous, UnkRepairState_l3c0d3c* next);
    virtual void vf_0x28(UnkRepairState_l3c0d3c* current, UnkRepairState_l3c0d3c* next);

    u64 field_0x8;
    UnkRepairState_l3c0d3c* current;
    UnkRepairState_l3c0d3c* previous;
    u32 count;
    UnkRepairStatePtr_l3c0d3c states[16];
    u32 pending;
    bool busy;
};

static inline bool FindState(UnkRepairMachine_l3c0d3c* self, u32 id, UnkRepairState_l3c0d3c*& out) {
    out = FindStateById(self->states, self->count, id);
    return out != nullptr;
}

// Switches to state `id` and clears the pending id.
static inline void SwitchState(UnkRepairMachine_l3c0d3c* self, u32 id, UnkRepairState_l3c0d3c* from) {
    if (!from || from->id != id) {
        UnkRepairState_l3c0d3c* found;
        UnkRepairState_l3c0d3c* next;
        u32 enter;
        if (FindState(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;  // as the engine's other state machines: an unknown id only clears the pending one
            enter = false;
            next = nullptr;
        }
        if (from) {
            from->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkRepairState_l3c0d3c*, s32);
            s32 previous_id = self->previous ? self->previous->id : kNoState;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = kNoState;
}

static inline void ApplyPending(UnkRepairMachine_l3c0d3c* self) {
    u32 id = self->pending;
    if (id > kLastState)
        return;
    SwitchState(self, id, self->current);
}

// Applies the pending state, then updates the current one and applies what it asked for, at most ten
// times, while marked busy.
static inline void UpdateMachine(UnkRepairMachine_l3c0d3c* self, f32 dt) {
    self->busy = true;
    if (self->pending != kNoState)
        ApplyPending(self);
    u32 i = 0;
    while (UnkRepairState_l3c0d3c* state = self->current) {
        state->vf_0x20(dt);
        if (self->pending == kNoState)
            break;
        ApplyPending(self);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}

// The parameter block.

struct UnkPoint_l3c0d3c {
    f32 x, y, z;
};

template <class T>
struct UnkOptional_l3c0d3c {
    T value;
    bool set;

    UnkOptional_l3c0d3c& operator=(const T& v) {
        value = v;
        set = true;
        return *this;
    }
};

struct UnkGoToParams_l3c0d3c {
    void** vtable;
    UnkOptional_l3c0d3c<UnkPoint_l3c0d3c> target;  // 0x8
    UnkOptional_l3c0d3c<f32> speed;                // 0x18
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[0x3];
    u32 field_0x34;
    u8 pad_0x38[0x4];
    bool field_0x3c;
    u8 pad_0x3d;
    bool field_0x3e;
    u8 pad_0x3f;
    bool field_0x40;
    u8 pad_0x41[0x7];
    u64 field_0x48[2];
    bool field_0x58;
    u8 pad_0x59[0xf];
    bool field_0x68;
    u8 pad_0x69;
    bool field_0x6a;
    u8 pad_0x6b[0x5];
    UnkReflValue value;  // 0x70
    bool field_0x88;

    UnkGoToParams_l3c0d3c() {
        speed.set = false;
        vtable = g_unk_7101a25b38 + 2;
        target.set = false;
        field_0x3c = false;
        value = base::reflection::refl_EmptyValue();
        field_0x3e = false;
        field_0x40 = false;
        field_0x68 = false;
        field_0x6a = false;
        field_0x58 = false;
        field_0x88 = false;
        field_0x34 = 1;
        field_0x30 = true;
        __builtin_memset(field_0x48, 0, 16);
        __builtin_memset(field_0x20, 0, 16);
    }
    ~UnkGoToParams_l3c0d3c() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&value);
    }
};
static_assert(__builtin_offsetof(UnkGoToParams_l3c0d3c, value) == 0x70, "layout");

// The owner.

// CTunableCharClassAutoolAIComponent's distance a repair point is gone to at full speed from (+0x3c).
struct UnkAutoolTunable_l3c0d3c {
    u8 pad_0x0[0x3c];
    f32 near_distance;
};

struct UnkRepairPoint_l3c0d3c {
    f32 distance;  // 0x0
    u8 pad_0x4[0x4];
    f32 speed;  // 0x8
    u8 pad_0xc[0x4];
    UnkPoint_l3c0d3c position;  // 0x10
};

struct UnkObject_l3c0d3c {
    virtual const base::reflection::CClass* GetClass();
};

struct UnkAutoolAI_l3c0d3c : UnkObject_l3c0d3c {
    u8 pad_0x8[0x8];
    UnkObject_l3c0d3c* char_class;  // 0x10
    u8 pad_0x18[0x14778 - 0x18];
    UnkRepairPoint_l3c0d3c repair_point;  // 0x14778
};
static_assert(__builtin_offsetof(UnkAutoolAI_l3c0d3c, repair_point) == 0x14778, "layout");

struct UnkNavObject_l3c0d3c {
    u8 pad_0x0[0x10];
    struct {
        u8 pad_0x0[0x10];
        u32 kind;  // 0x10
    }* field_0x10;
};

struct UnkNavTargets_l3c0d3c {
    UnkNavObject_l3c0d3c* field_0x0;
    void* field_0x8;
};

struct UnkNavigation_l3c0d3c {
    u8 pad_0x0[0xb0];
    UnkNavTargets_l3c0d3c* targets;  // 0xb0
};

struct UnkActor_l3c0d3c {
    u8 pad_0x0[0x240];
    UnkObject_l3c0d3c* ai;  // 0x240
    u8 pad_0x248[0x298 - 0x248];
    UnkNavigation_l3c0d3c* navigation;  // 0x298
};

struct UnkGoToRepairPoint_l3c0d3c {
    u8 pad_0x0[0x18];
    UnkActor_l3c0d3c* actor;  // 0x18
    u8 pad_0x20[0x2d0 - 0x20];
    UnkRepairMachine_l3c0d3c machine;  // 0x2d0
};
static_assert(__builtin_offsetof(UnkGoToRepairPoint_l3c0d3c, machine) == 0x2d0, "layout");
static_assert(__builtin_offsetof(UnkGoToRepairPoint_l3c0d3c, machine.busy) == 0x378, "layout");

template <const base::reflection::CClass* (*Getter)(), class T>
static inline T* CastTo(UnkObject_l3c0d3c* obj) {
    if (!obj)
        return nullptr;
    return fn_710009d50c(obj->GetClass(), Getter()) ? static_cast<T*>(obj) : nullptr;
}

s32 fn_71003c0d3c(UnkGoToRepairPoint_l3c0d3c* self, f32 dt) {
    UpdateMachine(&self->machine, dt);
    if (self->machine.current && self->machine.current->id == 5)
        return 5;

    UnkAutoolAI_l3c0d3c* ai = static_cast<UnkAutoolAI_l3c0d3c*>(self->actor->ai);
    if (ai && !fn_710009d50c(ai->GetClass(), fn_71003c2a94()))
        ai = nullptr;
    if (!ai)
        return 6;
    UnkNavigation_l3c0d3c* navigation = self->actor->navigation;
    if (!navigation)
        return 6;
    UnkObject_l3c0d3c* char_class = CastTo<fn_71003c855c, UnkObject_l3c0d3c>(ai->char_class);
    (void)char_class;

    UnkNavTargets_l3c0d3c* targets = navigation->targets;
    if (targets->field_0x0 && targets->field_0x0->field_0x10 && targets->field_0x0->field_0x10->kind == 1 &&
        targets->field_0x8) {
        const UnkAutoolTunable_l3c0d3c* tunable = reinterpret_cast<const UnkAutoolTunable_l3c0d3c*>(&g_unk_71726beb18);
        const UnkRepairPoint_l3c0d3c& point = ai->repair_point;
        UnkGoToParams_l3c0d3c params;
        params.field_0x30 = false;
        params.target = point.position;
        params.speed = point.speed;
        if (!(point.distance > tunable->near_distance))
            params.speed = 0.0f;
        fn_71006f4b0c(self->actor->navigation, &params, 0);
    }
    return 2;
}
