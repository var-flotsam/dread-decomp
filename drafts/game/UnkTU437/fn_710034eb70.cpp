// UnkTU437's unit (tools/tu_map.py unit 437, CAimComponent): the update. After the base update and two
// steps of its own, it advances three timers, hands the aim over from the old target (+0x638) to the new
// one (+0x630), drops the held references, reads the aim's angle limits from the animation's track and
// runs the state machine at +0x40. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkStateMachineFind.h"
#include "types.h"

struct UnkAimState_l34eb70 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkAimState_l34eb70* __attribute__((aligned(4))) UnkAimStatePtr_l34eb70;

struct UnkAimMachine_l34eb70 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkAimState_l34eb70* previous, UnkAimState_l34eb70* next);
    virtual void vf_0x28(UnkAimState_l34eb70* current, UnkAimState_l34eb70* next);

    u64 field_0x8;
    UnkAimState_l34eb70* current;
    UnkAimState_l34eb70* previous;
    u32 count;
    UnkAimStatePtr_l34eb70 states[16];
    u32 pending;
    bool busy;
};

static const u32 kAimLast_l34eb70 = 4;
static const u32 kAimNone_l34eb70 = 5;

// Moves to the state `id` (none for id 0) unless it is current, then clears the request.
static inline void Switch_l34eb70(UnkAimMachine_l34eb70* self, u32 id) {
    UnkAimState_l34eb70* old = self->current;
    if (!old || old->id != id) {
        UnkAimState_l34eb70* found = FindStateById(self->states, self->count, id);
        UnkAimState_l34eb70* next;
        bool enter;
        if (found) {
            next = found;
            enter = true;
        } else if (id == 0) {
            next = nullptr;
            enter = false;
        } else {
            self->pending = kAimNone_l34eb70;
            return;
        }
        if (old) {
            old->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            next->vf_0x10(self->previous ? self->previous->id : kAimNone_l34eb70);
            self->vf_0x20(self->previous, next);
        }
    }
    self->pending = kAimNone_l34eb70;
}

static inline void Apply_l34eb70(UnkAimMachine_l34eb70* self) {
    u32 id = self->pending;
    if (id > kAimLast_l34eb70)
        return;
    Switch_l34eb70(self, id);
}

static inline void Update_l34eb70(UnkAimMachine_l34eb70* self, f32 dt) {
    self->busy = true;
    if (self->pending != kAimNone_l34eb70)
        Apply_l34eb70(self);
    u32 i = 0;
    while (UnkAimState_l34eb70* state = self->current) {
        state->vf_0x20(dt);
        if (self->pending == kAimNone_l34eb70)
            break;
        Apply_l34eb70(self);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}

// A counted holder of an object (what the handles at +0x630, +0x638 and +0x680 refer to).
struct UnkAimHeld_l34eb70 : base::UnkRefCounted {
    void* object;  // 0x10
};

struct UnkAimTargetView_l34eb70 {
    virtual void vf_0x0();
    void Virtual(u32 slot, bool on) {
        reinterpret_cast<void (*)(UnkAimTargetView_l34eb70*, bool)>((*reinterpret_cast<void***>(this))[slot / 8])(
            this, on);
    }
};

struct UnkAimTunable_l34eb70 {
    u8 pad_0x0[0x78];
    f32 delay;  // 0x78
};

struct UnkAimAnglesTrack_l34eb70 {
    u8 pad_0x0[0x1c];
    f32 from;  // 0x1c
    u32 pad_0x20;
    f32 to;  // 0x24
};

struct UnkAimTrackInfo_l34eb70 {
    u8 pad_0x0[0x24];
    s32 steps;  // 0x24
};

struct UnkAimOwner_l34eb70 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkAim_l34eb70 {
    u8 pad_0x0[0x30];
    UnkAimOwner_l34eb70* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkAimMachine_l34eb70 machine;  // 0x40
    u8 pad_0xf0[0x2b0 - 0xf0];
    UnkAimTunable_l34eb70* tunable;  // 0x2b0
    u8 pad_0x2b8[0x2c0 - 0x2b8];
    f32 angle_from;   // 0x2c0
    f32 angle_to;     // 0x2c4
    bool field_0x2c8;
    f32 angle_range;  // 0x2cc
    u8 pad_0x2d0[0x2d8 - 0x2d0];
    f32 field_0x2d8;
    u8 pad_0x2dc[0x4cc - 0x2dc];
    f32 timer;       // 0x4cc
    f32 field_0x4d0;
    f32 cooldown;    // 0x4d4
    bool field_0x4d8;
    u8 pad_0x4d9[0x62c - 0x4d9];
    bool keep_target;               // 0x62c
    base::UnkRefHandle next_target;  // 0x630
    base::UnkRefHandle target;       // 0x638
    u8 pad_0x640[0x8];
    base::global::CRntVector<base::UnkRefPtrMember> held;  // 0x648
    u8 pad_0x670[0x680 - 0x670];
    base::UnkRefHandle field_0x680;
    u8 pad_0x688[0x691 - 0x688];
    bool field_0x691;
};
static_assert(__builtin_offsetof(UnkAim_l34eb70, machine.pending) == 0xe4, "layout");
static_assert(__builtin_offsetof(UnkAim_l34eb70, timer) == 0x4cc, "layout");
static_assert(__builtin_offsetof(UnkAim_l34eb70, held) == 0x648, "layout");
static_assert(__builtin_offsetof(UnkAim_l34eb70, field_0x691) == 0x691, "layout");

void fn_7100344904(UnkAim_l34eb70* self, f32 dt);
void fn_7100347540(UnkAim_l34eb70* self);
void fn_710034f220(UnkAim_l34eb70* self);
UnkAimTargetView_l34eb70* fn_710035379c(void* object);

static inline void* HeldObject_l34eb70(const base::UnkRefHandle& handle) {
    auto* held = static_cast<UnkAimHeld_l34eb70*>(handle.ptr);
    return held ? held->object : nullptr;
}

void fn_710034eb70(UnkAim_l34eb70* self, f32 dt) {
    fn_71010c4234(reinterpret_cast<char*>(self));
    fn_7100344904(self, dt);
    fn_7100347540(self);

    f32 timer = self->timer;
    if (timer >= 0.0f) {
        UnkAimTunable_l34eb70* tunable = self->tunable;
        bool before = timer < tunable->delay;
        self->timer = timer + dt;
        if (before && self->timer >= tunable->delay)
            self->field_0x2d8 = 0.001f;
    }
    if (self->field_0x4d0 >= 0.0f)
        self->field_0x4d0 += dt;
    if (self->cooldown > 0.0f) {
        f32 left = self->cooldown - dt;
        self->cooldown = left;
        if (left < 0.0f)
            self->cooldown = 0.0f;
    }

    if (self->next_target.ptr && HeldObject_l34eb70(self->next_target) && self->keep_target) {
        if (UnkAimTargetView_l34eb70* view = fn_710035379c(HeldObject_l34eb70(self->next_target)))
            view->Virtual(0x168, true);
    } else if (self->target.ptr && HeldObject_l34eb70(self->target)) {
        if (UnkAimTargetView_l34eb70* view = fn_710035379c(HeldObject_l34eb70(self->target)))
            view->Virtual(0x168, false);
    }
    fn_710031d208(&self->target, &self->next_target);
    if (base::UnkRefCounted* p = self->next_target.ptr) {
        if (--p->ref_count == 0)
            delete p;
        self->next_target.ptr = nullptr;
    }
    self->held.Clear();

    UnkTrackQuery query;
    query.cls = fn_7100c42738();
    query.result = nullptr;
    query.value = -1.0f;
    if (UnkAnimationComponent* animation = self->owner->animation) {
        fn_710117b08c(animation, &query);
        if (auto* track = reinterpret_cast<UnkAimAnglesTrack_l34eb70*>(query.result)) {
            f32 from = track->from;
            f32 to = track->to;
            auto* info = static_cast<UnkAimTrackInfo_l34eb70*>(
                fn_7101177d2c(self->owner->animation, 0xf4061ff25a2163d6));
            const f32 kTurn = 6.2831855f;
            f32 steps = static_cast<f32>(info->steps);
            f32 step = kTurn / steps;
            from = from / kTurn * steps * step;
            to = to / kTurn * steps * step;
            self->field_0x2c8 = false;
            self->angle_to = to;
            self->angle_from = from;
            self->angle_range = (to - from) * kTurn / 1.5707964f;
        }
    }

    Update_l34eb70(&self->machine, dt);
    self->field_0x4d8 = false;
    fn_710034f220(self);
    if (self->field_0x691 && !HeldObject_l34eb70(self->field_0x680))
        self->field_0x691 = false;
}
