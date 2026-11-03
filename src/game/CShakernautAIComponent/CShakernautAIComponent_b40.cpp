// CShakernautAIComponent unit (tools/tu_map.py unit 1047): the update (fn_71006ae2dc). After the base
// update it advances the two running timers at 0x14808 (those not below zero), then, for each of the two
// repeated actions that is active, its interval timer: when that reaches the interval of the tuning
// (+0x146f0 of the owner's AI component, when that is a CShakernautAIComponent) it tries the action
// (fn_71006ae480) and counts it, stopping after the tuning's count (or at once when it failed). Ends with
// fn_71006ae820. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkGobblerComponent;
void fn_7100369c44(UnkGobblerComponent*);
bool fn_71006ae480(void* self, bool first);
void fn_71006ae820(void* self, f32 dt);
extern const CClass g_unk_7172706520 __attribute__((visibility("hidden")));

struct UnkTuning_sh {
    u8 pad_0x0[0xf0];
    u32 count;  // 0xf0
    f32 interval;  // 0xf4
};

struct UnkAIComponent_sh {
    u8 pad_0x0[0x146f0];
    UnkTuning_sh* tuning;
};

struct UnkOwner_sh {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;
};

struct UnkTimers_sh {
    f32 timer[2];
    u8 pad_0x8[0x58];
    bool active[2];  // 0x60
    u8 pad_0x62[2];
    u32 done[2];  // 0x64
    f32 elapsed[2];  // 0x6c
};

struct UnkShakernautAIComponent {
    u8 pad_0x0[0x30];
    UnkOwner_sh* owner;
    u8 pad_0x38[0x147d0];
    UnkTimers_sh timers;  // 0x14808
};

static inline __attribute__((always_inline)) void Step_sh(UnkShakernautAIComponent* self, UnkTimers_sh* t,
                                                          UnkTuning_sh* tuning, u32 i, f32 dt) {
    if (!t->active[i])
        return;
    t->elapsed[i] += dt;
    if (t->elapsed[i] >= tuning->interval) {
        if (fn_71006ae480(self, i == 0))
            ++t->done[i];
        else
            t->done[i] = tuning->count;
        if (t->done[i] >= tuning->count) {
            t->active[i] = false;
            t->done[i] = 0;
        }
        t->elapsed[i] = 0.0f;
    }
}

void fn_71006ae2dc(UnkShakernautAIComponent* self, f32 dt) {
    UnkTimers_sh* t = &self->timers;
    fn_7100369c44(reinterpret_cast<UnkGobblerComponent*>(self));
    UnkReflObject* ai = self->owner->ai;
    const CClass* cls = ai->vf_0x0();
    fn_71006ad0a4();
    UnkAIComponent_sh* comp =
        fn_710009d50c(cls, &g_unk_7172706520) ? reinterpret_cast<UnkAIComponent_sh*>(ai) : nullptr;
    UnkTuning_sh* tuning = comp->tuning;
    if (t->timer[0] >= 0.0f)
        t->timer[0] += dt;
    if (t->timer[1] >= 0.0f)
        t->timer[1] += dt;
    Step_sh(self, t, tuning, 0, dt);
    Step_sh(self, t, tuning, 1, dt);
    fn_71006ae820(self, dt);
}
