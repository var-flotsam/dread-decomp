// animtree::CAnimatedNode's unit (tools/tu_map.py, unit 397): applies a motion rule to the node's track
// (its bone filter, +0x70): the weights, then the time, by the rule's mode (0 advances it by the
// owner's rate, 2 restarts it, others hold it), wrapping a looping track. Names and layouts are
// placeholders.
#include <math.h>
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkMotionRule.h"
#include "game/UnkNodeOwner_ovb0138.h"
#include "game/UnkTU1134.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "game/animtree/CAnimTreeElement.h"
#include "types.h"

struct UnkAnimatedNode_ovb0138;
struct UnkAnimComponent_a994;
struct UnkAnimTiming;
struct UnkTrackObject_710117adc0;

f32 fn_710117a994(UnkAnimComponent_a994* self, const UnkAnimTiming* timing, f32 seconds, f32 rate);
f32 fn_71010d6868();
bool fn_710070e3b8(UnkTrack* track, const UnkFlags_71007142e8* parts);
void fn_710070e3c8(UnkTrack* track, void* target, UnkFlags_71007142e8 parts, f32 frame, f32 frames);
void fn_710070e3ec(UnkTrack* track, void* target, UnkFlags_71007142e8 parts, f32 time, f32 frames);

// The track's timing (fn_710070d4e8).
struct UnkNodeTiming_l2d2cac {
    u8 pad_0x0[0x14];
    s32 loop;  // 1: the time is not snapped to the end
    u8 pad_0x18[0x10];
    f32 length;  // 0x28
};

// The node's track: its bone filter.
struct UnkNodeTrack_l2d2cac {
    u8 pad_0x0[0x38];
    u8 weights[0x5c];  // 0x38
    u32 field_0x94;
    u8 pad_0x98[0x10];
    f32 time;  // 0xa8
    u8 pad_0xac[0x4];
    u8 timer[0x8];  // 0xb0

    UnkTrack* AsTrack() { return reinterpret_cast<UnkTrack*>(this); }
};
static_assert(__builtin_offsetof(UnkNodeTrack_l2d2cac, timer) == 0xb0, "UnkNodeTrack_l2d2cac layout");

struct UnkAnimatedNode_l2d2cac {
    void** vtable;
    u8 pad_0x8[0x68];
    UnkNodeTrack_l2d2cac* track;  // 0x70
    u8 pad_0x78[6];
    bool allow_root_motion;  // 0x7e
};

// What the node is advanced in.
struct UnkNodeContext_l2d2cac {
    u8 pad_0x0[0x70];
    u8 field_0x70[0x6c];
    f32 dt;  // 0xdc
    u8 weights[0x8];  // 0xe0
};

// The rule (game/UnkMotionRule.h's, with its +0xc read as a rate).
struct UnkNodeRule_l2d2cac {
    UnkFlags_71007142e8 from;
    UnkFlags_71007142e8 to;
    s32 mode;
    f32 rate;
    bool weights_full;  // 0x10
    bool keep_time;     // 0x11
};

void fn_71002d2cac(UnkAnimatedNode_l2d2cac* self, UnkNodeContext_l2d2cac* ctx, const UnkNodeRule_l2d2cac* rule) {
    if (rule->to.bits != 0 && self->track->field_0x94 != 0) {
        u8* weights = self->track->weights;
        fn_7100714604(reinterpret_cast<UnkWeighted_7100714808*>(ctx->weights),
                      reinterpret_cast<UnkWeighted_7100714808*>(weights));
        if (rule->weights_full) {
            UnkFlags_71007142e8 all = UnkFlagsAll_71007142e8();
            fn_7100714eb4(weights, &all.bits);
        }
    }
    if (!self->allow_root_motion || !fn_710070d500(self->track->AsTrack()))
        return;
    UnkNodeTrack_l2d2cac* playing = self->track;
    UnkFlags_71007142e8 all = UnkFlagsAll_71007142e8();
    if (!fn_710070e3b8(playing->AsTrack(), &all))
        return;

    f32 frame;
    switch (rule->mode) {
    case 0: {
        if (fn_710002fb70(self->track->timer))
            return;
        f32 rate = rule->rate;
        UnkGameManager* manager = g_unk_7101d47310;
        const u8* state = reinterpret_cast<const u8*>(&manager->field_0xc4);
        if ((manager->field_0xc8 != 0 || manager->field_0x10c) && state[0] && !state[2]) {
            if (static_cast<void*>(fn_71002dbb7c(reinterpret_cast<UnkAnimatedNode_ovb0138*>(self))) ==
                fn_7100dec788(g_unk_7101d47310))
                rate *= fn_71010d6868();
        }
        f32 time = rule->keep_time ? self->track->time : fn_710070d890(self->track->AsTrack());
        UnkAnimComponent_a994* anim = reinterpret_cast<UnkAnimComponent_a994*>(
            fn_71002dbb7c(reinterpret_cast<UnkAnimatedNode_ovb0138*>(self))->field_0x220);
        f32 frames = fn_710117a994(anim, reinterpret_cast<const UnkAnimTiming*>(fn_710070d4e8(self->track->AsTrack())),
                                   time, rate);
        fn_710070e3ec(self->track->AsTrack(), ctx->field_0x70, rule->from, time, frames);
        if (!rule->keep_time)
            return;
        const UnkNodeTiming_l2d2cac* timing =
            reinterpret_cast<const UnkNodeTiming_l2d2cac*>(fn_710070d4e8(self->track->AsTrack()));
        f32 t = time + ctx->dt;
        if (timing->loop != 1) {
            // Within a thousandth of the end is the end.
            f32 end = timing->length;
            if (t - 0.001f <= end && t + 0.001f >= end)
                t = end;
        }
        UnkTrackObject_710117adc0* object = reinterpret_cast<UnkTrackObject_710117adc0*>(self->track);
        if (fn_710070d820(object) && t >= fn_710070d850(self->track->AsTrack()))
            t = fmodf(t, fn_710070d850(self->track->AsTrack()));
        self->track->time = t;
        return;
    }
    case 2:
        frame = 0.0f;
        break;
    default: {
        const UnkAnimTiming* timing = reinterpret_cast<const UnkAnimTiming*>(fn_710070d4e8(self->track->AsTrack()));
        frame = fn_7100b8aff0(self->track->time, timing);
        break;
    }
    }
    UnkNodeTrack_l2d2cac* track = self->track;
    u8* target = ctx->field_0x70;
    f32 frames = static_cast<s32>(fn_710070d840(track->AsTrack()));
    fn_710070e3c8(track->AsTrack(), target, rule->from, frame, frames);
    if (rule->keep_time) {
        UnkNodeTrack_l2d2cac* held = self->track;
        held->time = fn_710070d850(held->AsTrack());
    }
}
