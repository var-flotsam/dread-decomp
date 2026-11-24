// CYamplotXAIComponent's unit (tools/tu_map.py, unit 1109): after the base update, resets the
// component's state, hands the owner's object at +0x218 its callback and reads the ends of six
// timelines' motions. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkMotionRule.h"
#include "game/UnkStruct_71007142e8.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTimeline.h"
#include "types.h"

// The motion fn_71007142e8 initialises: the time it ends on at +0x2c.
struct UnkMotionTracks;

struct UnkYamplotXAI_l6ef568;
void fn_71006f3288(UnkYamplotXAI_l6ef568* self);

extern const base::global::CStrId g_unk_717270c490 __attribute__((visibility("hidden")));

// A function and the object it is called on.
struct UnkYamplotCallback_l6ef568 {
    UnkYamplotXAI_l6ef568* object;
    void (*function)(UnkYamplotXAI_l6ef568* self);
};

struct UnkYamplotListener_l6ef568 {
    u8 pad_0x0[0x19d0];
    UnkYamplotCallback_l6ef568 callback;  // 0x19d0
};

struct UnkYamplotTarget_l6ef568 {
    void** vtable;
    void vf_0x2b0(UnkCollisionObject* object) {
        reinterpret_cast<void (*)(UnkYamplotTarget_l6ef568*, UnkCollisionObject*)>(vtable[0x2b0 / 8])(this, object);
    }
};

struct UnkYamplotOwner_l6ef568 {
    u8 pad_0x0[0x1f8];
    UnkYamplotTarget_l6ef568* field_0x1f8;
    u8 pad_0x200[0x18];
    UnkYamplotListener_l6ef568* listener;  // 0x218
    UnkStruct_7101176ea8* actions;          // 0x220
    u8 pad_0x228[0x78];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkYamplotState_l6ef568 {
    UnkYamplotCallback_l6ef568 callback;
    f32 motionEnds[6];  // 0x10
    bool field_0x28;
    f32 field_0x2c;
    bool field_0x30;
    bool field_0x31;
    bool field_0x32;
    u32 field_0x34;
    bool field_0x38;
    bool field_0x39;
    u8 pad_0x3a[0x4e];
    bool field_0x88;
    f32 field_0x8c;
    f32 field_0x90;
    s32 mode;  // 0x94
};

struct UnkYamplotXAI_l6ef568 {
    u8 pad_0x0[0x30];
    UnkYamplotOwner_l6ef568* owner;  // 0x30
    u8 pad_0x38[0x14730 - 0x38];
    UnkYamplotState_l6ef568 state;  // 0x14730
};
static_assert(__builtin_offsetof(UnkYamplotOwner_l6ef568, collision) == 0x2a0, "layout");
static_assert(__builtin_offsetof(UnkYamplotState_l6ef568, field_0x88) == 0x88, "layout");
static_assert(__builtin_offsetof(UnkYamplotState_l6ef568, mode) == 0x94, "layout");

// The end of the motion over the whole of the timeline `key`, when the owner has it.
static inline void MotionEnd(UnkStruct_7101176ea8* actions, u64 key, f32* out) {
    UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176d34(actions, reinterpret_cast<void*>(key)));
    if (!timeline)
        return;
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    *out = fabsf(motion.xform.m[2][3]);
}

void fn_71006ef568(UnkYamplotXAI_l6ef568* self) {
    fn_7100369650(reinterpret_cast<char*>(self));
    UnkYamplotState_l6ef568& state = self->state;
    state.field_0x88 = false;
    if (state.mode == 1) {
        state.mode = 0;
        UnkCollisionObject* object = fn_71008640a4(self->owner->collision, &g_unk_717270c490);
        self->owner->field_0x1f8->vf_0x2b0(object);
    }
    state.mode = 0;
    UnkYamplotListener_l6ef568* listener = self->owner->listener;
    state.callback.object = self;
    state.callback.function = fn_71006f3288;
    listener->callback = state.callback;
    state.field_0x28 = false;
    state.field_0x30 = false;
    state.field_0x38 = false;
    state.field_0x39 = false;
    state.field_0x88 = false;
    state.field_0x31 = false;
    state.field_0x32 = false;
    state.field_0x34 = 0;
    state.field_0x2c = 1.0f;
    if (self->owner->actions) {
        MotionEnd(self->owner->actions, 0x4238236fad090a8eull, &state.motionEnds[0]);
        MotionEnd(self->owner->actions, 0xa76c5a129aa91000ull, &state.motionEnds[1]);
        MotionEnd(self->owner->actions, 0xf2363575070aac60ull, &state.motionEnds[2]);
        MotionEnd(self->owner->actions, 0xbca413863f42dc58ull, &state.motionEnds[3]);
        MotionEnd(self->owner->actions, 0xeacdeb01ece85851ull, &state.motionEnds[4]);
        MotionEnd(self->owner->actions, 0xad9215f288161046ull, &state.motionEnds[5]);
    }
    state.field_0x8c = 0.0f;
    state.field_0x90 = 0.0f;
}
