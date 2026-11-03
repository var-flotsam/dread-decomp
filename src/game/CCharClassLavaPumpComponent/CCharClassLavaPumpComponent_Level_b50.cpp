// CCharClassLavaPumpComponent unit (tools/tu_map.py, the name from reflection): passes the level to
// the floating prop found for the actor (+0x30, fn_71007e4924; cached at +0x88) through
// fn_710117ad18, then plays the actor's timeline key g_unk_710164ae00 at a level of 1 or
// more, or g_unk_710164adf8 at 0 or less (fn_7100d7c828). Names are placeholders.
#include "game/UnkActor.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkWeakTarget;
UnkWeakTarget* fn_71007e4924(void* owner);
struct UnkActionSwitcherAnimation_b48;
void fn_710117ad18(UnkActionSwitcherAnimation_b48* self, bool flag, f32 value);

extern u64 g_unk_710164adf8;
extern u64 g_unk_710164ae00;

struct UnkLavaPump {
    u8 unk_0x0[0x30];
    UnkActor* actor;  // 0x30
    u8 unk_0x38[0x50];
    UnkWeakTarget* prop;  // 0x88
};

void fn_7100879fb0(UnkLavaPump* self, f32 level) {
    UnkWeakTarget* prop = self->prop;
    if (prop == nullptr) {
        prop = self->actor ? fn_71007e4924(self->actor) : nullptr;
        self->prop = prop;
    }
    if (prop)
        fn_710117ad18(reinterpret_cast<UnkActionSwitcherAnimation_b48*>(prop), false, level);
    if (level >= 1.0f)
        fn_7100d7c828(self->actor, &g_unk_710164ae00, false);
    else if (level <= 0.0f)
        fn_7100d7c828(self->actor, &g_unk_710164adf8, false);
}
