// behaviortree::CBatalloonGoToTargetAction's unit (tools/tu_map.py): the action's update. The time
// spent (+0x12aec) grows by `dt` and the delay at +0x12af0 counts down while above 0.001. Done (5)
// once the flag at +0x12ae8 is set; otherwise slot 0x78 runs once the time passes the AI's (+0x240 of
// the owner, +0x18) limit, the one at +0x13d8 when the flag at +0x3b is set and that limit is in use,
// else the one at +0x113c. Then the navigation (+0x298 of the owner; checked with fn_71003dc89c first
// when +0x3d is set) answers: running (2) while its path (+0xb0) has a target of kind 2 (+0x10 kept) or
// kind 4 (+0x20 kept), failed (6) otherwise. Names and layouts are placeholders.
#include "types.h"

struct UnkBatalloonKind_ovb0133 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkBatalloonEntry_ovb0133 {
    u8 pad_0x0[0x10];
    UnkBatalloonKind_ovb0133* target;  // 0x10
};

struct UnkBatalloonPath_ovb0133 {
    UnkBatalloonEntry_ovb0133* entry;
    u8 pad_0x8[8];
    void* field_0x10;
    u8 pad_0x18[8];
    void* field_0x20;
};

struct UnkBatalloonNav_ovb0133 {
    u8 pad_0x0[0xb0];
    UnkBatalloonPath_ovb0133* path;  // 0xb0
};

struct UnkBatalloonAI_ovb0133 {
    u8 pad_0x0[0x113c];
    f32 limit;  // 0x113c
    u8 pad_0x1140[0x13d8 - 0x1140];
    f32 alt_limit;  // 0x13d8
};

struct UnkBatalloonOwner_ovb0133 {
    u8 pad_0x0[0x240];
    UnkBatalloonAI_ovb0133* ai;  // 0x240
    u8 pad_0x248[0x50];
    UnkBatalloonNav_ovb0133* nav;  // 0x298
};

struct UnkBatalloonTimers_ovb0133 {
    bool done;  // 0x0
    u8 pad_0x1[3];
    f32 time;  // 0x4
    f32 delay;  // 0x8
};

struct UnkBatalloonGoToTarget_ovb0133 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkBatalloonOwner_ovb0133* owner;  // 0x18
    u8 pad_0x20[0x1b];
    bool use_alt_limit;  // 0x3b
    u8 pad_0x3c;
    bool check_nav;  // 0x3d
    u8 pad_0x3e[0x12ae8 - 0x3e];
    UnkBatalloonTimers_ovb0133 timers;  // 0x12ae8

    void vf_0x78() { reinterpret_cast<void (*)(UnkBatalloonGoToTarget_ovb0133*)>(vtable[0x78 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkBatalloonGoToTarget_ovb0133, timers) == 0x12ae8, "layout");

bool fn_71003dc89c(UnkBatalloonGoToTarget_ovb0133* self);

static inline bool Passed(f32 limit, f32 time) {
    return limit > 0.001f && time > limit;
}

static inline bool UseAlt(UnkBatalloonGoToTarget_ovb0133& self, UnkBatalloonAI_ovb0133* ai) {
    return ai && self.use_alt_limit;
}

s32 fn_71003dcf14(UnkBatalloonGoToTarget_ovb0133* self, f32 dt) {
    UnkBatalloonTimers_ovb0133* timers = &self->timers;
    f32 time = timers->time + dt;
    timers->time = time;
    if (timers->delay > 0.001f)
        timers->delay = timers->delay - dt;
    if (timers->done)
        return 5;
    UnkBatalloonNav_ovb0133* nav = self->owner->nav;
    UnkBatalloonAI_ovb0133* ai = self->owner->ai;
    if ((UseAlt(*self, ai) && Passed(ai->alt_limit, time)) || Passed(ai->limit, time))
        self->vf_0x78();
    if (self->check_nav) {
        bool ok = fn_71003dc89c(self);
        if (!nav || !ok)
            return 6;
    } else if (!nav) {
        return 6;
    }
    UnkBatalloonPath_ovb0133* path = nav->path;
    if (!path->entry)
        return 6;
    UnkBatalloonKind_ovb0133* target = path->entry->target;
    if (!target)
        return 6;
    if (target->kind == 2 && path->field_0x10)
        return 2;
    // The original reads the kind again; a relaxed atomic load (a plain ldr) keeps the second read.
    if (__atomic_load_n(&target->kind, __ATOMIC_RELAXED) != 4)
        return 6;
    return path->field_0x20 ? 2 : 6;
}
