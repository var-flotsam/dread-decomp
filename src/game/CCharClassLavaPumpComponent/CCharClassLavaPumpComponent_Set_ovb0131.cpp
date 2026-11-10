// CCharClassLavaPumpComponent unit (unit 1333 of tools/tu_map.py): sets the pump level. A positive level
// starts the pump (+0x90; the first time plays the timeline key g_unk_710164adf0) and is eased from the
// tunable's minimum (g_unk_717272a728, +0x24) towards 1; a level within 0.001 of zero while started
// switches to the timed run down (+0x91, the time +0x94 is the tunable's +0x24 times +0x28). Unless
// running down, the level goes to the floating prop (as fn_7100879fb0), then the key for full, partial
// or zero level is played. Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkWeakTarget;
UnkWeakTarget* fn_71007e4924(void* owner);
struct UnkActionSwitcherAnimation_b48;
void fn_710117ad18(UnkActionSwitcherAnimation_b48* self, bool flag, f32 value);

// As src/reflection/CCharClassLavaPumpComponent.cpp declares it.
extern base::reflection::UnkTunable g_unk_717272a728;

extern u64 g_unk_710164adf0;
extern u64 g_unk_710164adf8;
extern u64 g_unk_710164ae00;
extern u64 g_unk_710164ae08;

struct UnkLavaPumpTunable_ovb0131 {
    u8 unk_0x0[0x24];
    f32 minimum;   // 0x24
    f32 duration;  // 0x28
};

struct UnkLavaPump_ovb0131 {
    u8 unk_0x0[0x30];
    UnkActor* actor;  // 0x30
    u8 unk_0x38[0x50];
    UnkWeakTarget* prop;  // 0x88
    bool started;  // 0x90
    bool running_down;  // 0x91
    u8 unk_0x92[2];
    f32 time;  // 0x94
};

void fn_7100879e54(UnkLavaPump_ovb0131* self, f32 level) {
    UnkLavaPumpTunable_ovb0131* tunable = reinterpret_cast<UnkLavaPumpTunable_ovb0131*>(&g_unk_717272a728);
    if (level > 0.0f) {
        if (!self->started)
            fn_7100d7c828(self->actor, &g_unk_710164adf0, false);
        self->started = true;
        f32 eased = 1.0f;
        if (!(level >= 1.0f)) {
            eased = tunable->minimum;
            if (!(level < 0.0f))
                eased = (1.0f - level) * eased + level;
        }
        self->running_down = false;
        level = eased;
    } else if (level + 0.001f >= 0.0f && level - 0.001f <= 0.0f && self->started) {
        self->running_down = true;
        self->time = tunable->minimum * tunable->duration;
    }
    if (self->prop == nullptr)
        self->prop = self->actor ? fn_71007e4924(self->actor) : nullptr;
    if (!self->running_down && self->prop)
        fn_710117ad18(reinterpret_cast<UnkActionSwitcherAnimation_b48*>(self->prop), false, level);
    if (level >= 1.0f)
        fn_7100d7c828(self->actor, &g_unk_710164adf8, false);
    else if (!(level <= 0.0f))
        fn_7100d7c828(self->actor, &g_unk_710164ae08, false);
    else
        fn_7100d7c828(self->actor, &g_unk_710164ae00, false);
}
