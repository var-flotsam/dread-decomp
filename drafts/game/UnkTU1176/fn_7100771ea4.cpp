// Unit 1176 of tools/tu_map.py (CCentralUnitComponent's neighbours): once a frame, sets the scene's alarm
// colour: while an E.M.M.I. is alarmed, starts its alarm timeline (phase 1 when it hunts, else phase 2) and
// pulses the colour's w with a sine, else leaves it white. Names and layouts are placeholders.
#include <math.h>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAutoolAIComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU315.h"
#include "game/UnkTU39.h"
#include "game/UnkTU58.h"
#include "game/UnkTU811.h"
#include "game/UnkTimelineCalls.h"
#include "game_s2/UnkChargeComponent.h"
#include "types.h"

using base::math::UnkVector4D;
using base::reflection::UnkReflObject;

__attribute__((visibility("hidden"))) extern u32 g_unk_71727153e0;  // the frame the colour was last set
__attribute__((visibility("hidden"))) extern f32 g_unk_71727153dc;  // the alarm pulse's phase
__attribute__((visibility("hidden"))) extern f32 g_unk_7101a6edc8;  // the time left before the pulse stops

struct UnkPauseState_l771ea4 {
    u8 pad_0x0[0xb14];
    bool paused;  // 0xb14
};

// The tunable's alarm values: interval and speed while hunting, and otherwise.
struct UnkAlarmTunable_l771ea4 {
    u8 pad_0x0[0x3c];
    f32 hunt_interval;  // 0x3c
    f32 hunt_speed;     // 0x40
    f32 interval;       // 0x44
    f32 speed;          // 0x48
    u8 pad_0x4c[0x18];
    bool enabled;  // 0x64
};

struct UnkEmmyClass_l771ea4 : UnkReflObject {
    u8 pad_0x8[0x608];
    bool alarm;  // 0x610
};

struct UnkEmmyAI_l771ea4 : UnkReflObject {
    u8 pad_0x8[0x8];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x14c1d];
    bool field_0x14c35;
    u8 pad_0x14c36[0x12aea];
    bool hunting;  // 0x27720
};
static_assert(__builtin_offsetof(UnkEmmyAI_l771ea4, field_0x14c35) == 0x14c35, "layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l771ea4, hunting) == 0x27720, "layout");

struct UnkEmmyActor_l771ea4 {
    u8 pad_0x0[0x1b4];
    bool field_0x1b4;
    u8 pad_0x1b5[0x8b];
    UnkReflObject* ai;  // 0x240
};

struct UnkEmmyRef_l771ea4 {
    u8 pad_0x0[0x10];
    UnkEmmyActor_l771ea4* actor;  // 0x10
};

struct UnkStateHolder_l771ea4 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkCentralUnit_l771ea4 {
    u8 pad_0x0[0x18];
    UnkEvacPart_b61* part;  // 0x18
    u8 pad_0x20[0x20];
    UnkStateHolder_l771ea4* field_0x40;  // 0x40
    u8 pad_0x48[0x690];
    UnkEmmyRef_l771ea4* emmy;  // 0x6d8
    u8 pad_0x6e0[0x1c8];
    u8 timer[0x150];  // 0x8a8
    bool enabled;     // 0x9f8
};
static_assert(__builtin_offsetof(UnkCentralUnit_l771ea4, enabled) == 0x9f8, "layout");

inline f32 Clamp(f32 v, f32 lo, f32 hi) {
    if (v <= lo)
        return lo;
    if (v >= hi)
        return hi;
    return v;
}

// Starts the timeline `id` with every flag set.
inline void StartTimeline(UnkEvacTimeline_b61* timeline, const u64& id) {
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(reinterpret_cast<UnkTimelineComponent*>(timeline), &id, &mask.id, false, -1.0f);
}

void fn_7100771ea4(UnkCentralUnit_l771ea4* self, f32 dt) {
    if (static_cast<UnkPauseState_l771ea4*>(g_unk_7101d47330)->paused)
        return;

    UnkVector4D alarm_color(2.0f, 0.0f, 0.0f, 0.0f);
    UnkVector4D idle_color(1.0f, 1.0f, 1.0f, 0.0f);
    UnkVector4D color(1.0f, 1.0f, 1.0f, 0.0f);
    if (!self->enabled) {
        fn_710021a0b4(g_unk_7101d47300, &g_unk_710164aa90, &idle_color);
        return;
    }

    UnkEmmyRef_l771ea4* emmy = self->emmy;
    if (!emmy || !emmy->actor)
        return;
    UnkReflObject* ai_object = emmy->actor->ai;
    if (!ai_object)
        return;
    const UnkAlarmTunable_l771ea4& tunable = reinterpret_cast<const UnkAlarmTunable_l771ea4&>(g_unk_71726e0aa8);
    f32 interval = tunable.interval;
    f32 speed = tunable.speed;
    if (!fn_710009d50c(ai_object->vf_0x0(), fn_710053b934()))
        return;
    UnkEmmyAI_l771ea4* ai = static_cast<UnkEmmyAI_l771ea4*>(ai_object);
    UnkReflObject* class_object = ai->char_class;
    if (!class_object || !fn_710009d50c(class_object->vf_0x0(), fn_710051e010()))
        return;

    bool alarmed = false;
    bool phase_2 = true;
    if (static_cast<UnkEmmyClass_l771ea4*>(class_object)->alarm && tunable.enabled) {
        const bool& hunting = ai->hunting;
        if (fn_710002fb70(self->timer)) {
            alarmed = true;
            if (hunting) {
                phase_2 = false;
                interval = tunable.hunt_interval;
                speed = tunable.hunt_speed;
            }
        } else if (self->emmy->actor->field_0x1b4) {
            alarmed = ai->field_0x14c35 ? fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(ai)) : false;
            if (hunting) {
                phase_2 = false;
                interval = tunable.hunt_interval;
                speed = tunable.hunt_speed;
            }
        } else {
            s32 state = 8;
            if (self->field_0x40)
                state = self->field_0x40->state;
            alarmed = state == 5 || state == 6;
        }
    }

    u32 frame = g_unk_7101d58e48;
    if (g_unk_71727153e0 == frame)
        return;
    g_unk_71727153e0 = frame;

    if (alarmed) {
        if (g_unk_71727153dc == 0.0f) {
            if (UnkEvacTimeline_b61* timeline = fn_71003c6c3c(self->part)) {
                if (phase_2) {
                    u64 id = fn_7100001570("emmy_alarm_phase_2", 18);
                    StartTimeline(timeline, id);
                } else {
                    u64 id = fn_7100001570("emmy_alarm_phase_1", 18);
                    StartTimeline(timeline, id);
                }
            }
        }
        base::math::Assign(color, alarm_color);
        g_unk_71727153dc = speed * dt + g_unk_71727153dc;
        if (g_unk_7101a6edc8 < 0.0f) {
            color.w = Clamp(sinf(g_unk_71727153dc) * 0.65f, -1.0f, 1.0f);
            if (color.w < 0.0f) {
                g_unk_7101a6edc8 = interval;
                color.w = 0.0f;
            }
        } else {
            g_unk_7101a6edc8 -= dt;
            if (interval > 0.0f && g_unk_7101a6edc8 < 0.0f)
                g_unk_71727153dc = 0.0f;
        }
    } else {
        g_unk_71727153dc = 0.0f;
    }
    fn_710021a0b4(g_unk_7101d47300, &g_unk_710164aa90, &color);
}
