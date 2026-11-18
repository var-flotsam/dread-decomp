// Best graded form (91.8%) was this with the release written on the members, not through Watch().
// behaviortree::CInPathPatrolAction's unit (tools/tu_map.py, unit 488): the start step (fn_710038b470). Runs the
// base start, resets the wait, takes the owner's AI (+0x240), navigation (+0x298) and animation (+0x220), checks
// the two animation ids, sets up its callbacks, picks a wait and watches the animation. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern "C" int rand();

char* fn_710038d50c(char* p0);
struct UnkHandler;
struct UnkMessage;
u32 fn_710038d520(UnkHandler* self, UnkMessage* message);

struct UnkInPathPatrolAI_l38b470 {
    u8 pad_0x0[0x1468a];
    bool field_0x1468a;
};

struct UnkInPathPatrolOwner_l38b470 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkInPathPatrolAI_l38b470* ai;  // 0x240
    u8 pad_0x248[0x298 - 0x248];
    void* navigation;  // 0x298
};

// A listener: its vtable, the object and the function it calls, and the handle of what it watches.
struct UnkInPathPatrolListener_l38b470 {
    void** vtable;
    void* object;
    void* callback;
    UnkWeakHandle* watched;

    // Stops watching what it watched and watches `target` instead.
    void Watch(UnkWeakTarget* target) {
        if (UnkWeakHandle* handle = watched) {
            if (UnkWeakTarget* old = handle->target)
                fn_710117b3f0(old, this);
            ResetRef(&watched);
        }
        fn_7100316018(target, &watched);
        fn_710117b3e8(watched ? watched->target : nullptr, this);
    }
};

struct UnkInPathPatrol_l38b470 {
    u8 pad_0x0[0x18];
    UnkInPathPatrolOwner_l38b470* owner;  // 0x18
    u8 pad_0x20[0x10];
    UnkInPathPatrolAI_l38b470* ai;  // 0x30
    void* navigation;               // 0x38
    void* callback_object;          // 0x40
    void* callback;                 // 0x48
    bool field_0x50;
    u8 pad_0x51[3];
    u32 state;  // 0x54
    u8 pad_0x58[0x10];
    base::global::CStrId anim;       // 0x68
    base::global::CStrId next_anim;  // 0x70
    u8 min_property[0x8];            // 0x78, a string id
    u8 max_property[0x8];            // 0x80, a string id
    u8 pad_0x88[0x12];
    bool has_anim;  // 0x9a
    u8 pad_0x9b;
    f32 wait;   // 0x9c
    f32 timer;  // 0xa0
    bool dirty;  // 0xa4
    bool field_0xa5;
    u8 pad_0xa6[2];
    UnkInPathPatrolListener_l38b470 listener;  // 0xa8
};
static_assert(__builtin_offsetof(UnkInPathPatrol_l38b470, listener.watched) == 0xc0, "layout");
static_assert(__builtin_offsetof(UnkInPathPatrol_l38b470, wait) == 0x9c, "layout");

static inline bool IsAbout(f32 v, f32 target) {
    return v - 0.001f <= target && v + 0.001f >= target;
}

// Picks the wait at random between the two blackboard values (both positive, or both -1); -1 otherwise.
static inline void PickWait(UnkInPathPatrol_l38b470* self) {
    f32 lo = -2.0f;
    fn_7100394e2c(reinterpret_cast<UnkSelf*>(self), self->min_property, &lo);
    f32 hi = -2.0f;
    fn_7100394e2c(reinterpret_cast<UnkSelf*>(self), self->max_property, &hi);
    f32 min = lo;
    f32 max = hi;
    f32 wait;
    if ((min > 0.0f && max > 0.0f) || (IsAbout(min, -1.0f) && IsAbout(max, -1.0f))) {
        f32 r = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
        wait = min + (max - min) * r;
    } else {
        wait = -1.0f;
    }
    self->wait = wait;
    self->dirty = false;
    self->timer = -1.0f;
}

void fn_710038b470(UnkInPathPatrol_l38b470* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    self->wait = -1.0f;
    self->timer = -1.0f;
    self->has_anim = self->anim.data != &g_unk_7101d47108->empty;
    UnkInPathPatrolOwner_l38b470* owner = self->owner;
    if (!(self->ai = owner->ai))
        return;
    if (!(self->navigation = owner->navigation))
        return;
    UnkStruct_7101176ea8* animation = owner->animation;
    if (!animation)
        return;
    if (self->has_anim) {
        if (!fn_7101176e78(animation, reinterpret_cast<void*>(self->anim.data->Key()), nullptr))
            return;
        if (!fn_7101176e78(animation, reinterpret_cast<void*>(self->next_anim.data->Key()), nullptr))
            return;
    }
    UnkInPathPatrolAI_l38b470* ai = self->ai;
    self->field_0x50 = false;
    self->callback_object = self;
    self->callback = reinterpret_cast<void*>(fn_710038d50c);
    self->state = 2;
    if (ai->field_0x1468a) {
        self->state = 3;
        ai->field_0x1468a = false;
    }
    if (self->has_anim)
        PickWait(self);
    self->listener.object = self;
    self->listener.callback = reinterpret_cast<void*>(fn_710038d520);
    if (UnkWeakHandle* handle = self->listener.watched) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, &self->listener);
        ResetRef(&self->listener.watched);
    }
    fn_7100316018(reinterpret_cast<UnkWeakTarget*>(animation), &self->listener.watched);
    fn_710117b3e8(self->listener.watched ? self->listener.watched->target : nullptr, &self->listener);
    self->field_0xa5 = false;
}
