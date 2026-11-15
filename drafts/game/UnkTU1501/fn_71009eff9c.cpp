// CSamusMovement's unit (unit 1501): moves the value at +0x5324 towards its target at +0x5328 (or straight onto
// it when `snap` is set), at a rate that is lower while running with a target and the speed booster is up.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkActor_l9eff9c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;  // 0x220
};

struct UnkApproach_l9eff9c {
    f32 value;   // 0x5324
    f32 target;  // 0x5328
};

struct UnkMovement_l9eff9c {
    u8 pad_0x0[0x30];
    UnkActor_l9eff9c* actor;  // 0x30
    u8 pad_0x38[0xf0];
    UnkWeakHandle* field_0x128;  // 0x128
    u8 pad_0x130[0x51f4];
    UnkApproach_l9eff9c approach;  // 0x5324
};
static_assert(__builtin_offsetof(UnkMovement_l9eff9c, field_0x128) == 0x128, "layout");
static_assert(__builtin_offsetof(UnkMovement_l9eff9c, approach) == 0x5324, "layout");

static inline bool Near(f32 a, f32 b) {
    return b >= a - 0.001f && b <= a + 0.001f;
}

void fn_71009eff9c(UnkMovement_l9eff9c* self, bool snap, f32 dt) {
    UnkApproach_l9eff9c* approach = &self->approach;
    if (snap) {
        approach->value = approach->target;
        return;
    }
    if (Near(approach->value, approach->target)) {
        approach->value = approach->target;
        return;
    }
    bool running = fn_710117b348(self->actor->field_0x220, fn_7100d25ea8()) && self->field_0x128 && self->field_0x128->target;
    bool paused = fn_7100df5740(g_unk_7101d47310);
    const u8* bytes = reinterpret_cast<const u8*>(g_unk_7101d47310);
    bool booster = (g_unk_7101d47310->field_0xc8 != 0 || g_unk_7101d47310->field_0x10c) && bytes[0xc4] && !bytes[0xc6];
    f32 rate = 400.0f;
    if (running)
        rate = paused || booster ? 200.0f : 400.0f;
    f32 step = rate * dt;
    if (approach->target > approach->value) {
        f32 next = step + approach->value;
        approach->value = approach->target < next ? approach->target : next;
    } else {
        f32 next = approach->value - step;
        approach->value = approach->target > next ? approach->target : next;
    }
}
