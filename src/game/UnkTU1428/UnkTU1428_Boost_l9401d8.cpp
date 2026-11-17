// CMorphBallMovement's unit: the update. After the base step, counts down the timer at +0x2534 and,
// while the boost flag (+0x2542) is set, sets the boost (+0x2634) to how far the horizontal speed
// exceeds the mode's limit (capped by the config's +0xcc) along the normalised velocity, then runs
// the two follow-up steps. Names and layouts are placeholders.
#include <math.h>

#include "game/CAbilitySpeedBooster.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

namespace base { namespace reflection { struct UnkTunable; } }

struct UnkPlayer;
f32 fn_71010d6858();
extern base::reflection::UnkTunable g_unk_71726b0fc0;

// The slow-motion flags as bytes of the game manager's word at +0xc4.
struct UnkSlomoFlags {
    bool enabled;  // 0xc4
    u8 unk_0xc5;
    bool paused;  // 0xc6
};

struct UnkMorphBallTunable_01d8 {
    u8 pad_0x0[0x34];
    f32 speed_booster_scale;  // 0x34
};

struct UnkMorphBallConfig_01d8 {
    u8 pad_0x0[0xcc];
    f32 max_boost;  // 0xcc
};

struct UnkMorphBallMovement_01d8 {
    u8 pad_0x0[0x1d8];
    UnkVec3f velocity;  // 0x1d8
    u8 pad_0x1e4[0x2280 - 0x1e4];
    UnkMorphBallConfig_01d8* config;  // 0x2280
    s32 mode;                         // 0x2288
    f32 limit_mode1;                  // 0x228c
    f32 limit_mode2;                  // 0x2290
    f32 limit_mode3;                  // 0x2294
    u8 pad_0x2298[0x2534 - 0x2298];
    f32 timer;  // 0x2534
    u8 pad_0x2538[0x2542 - 0x2538];
    bool boosting;  // 0x2542
    u8 pad_0x2543[0x2634 - 0x2543];
    f32 boost;     // 0x2634
    UnkVec3f dir;  // 0x2638
};
static_assert(__builtin_offsetof(UnkMorphBallMovement_01d8, timer) == 0x2534, "layout");
static_assert(__builtin_offsetof(UnkMorphBallMovement_01d8, boost) == 0x2634, "layout");

void fn_71010d7b28(UnkMorphBallMovement_01d8* self, f32 dt);
void fn_7100940490(UnkMorphBallMovement_01d8* self, f32 dt);
void fn_7100942550(UnkMorphBallMovement_01d8* self, f32 dt);

void fn_71009401d8(UnkMorphBallMovement_01d8* self, f32 dt) {
    fn_71010d7b28(self, dt);
    UnkGameManager* manager = g_unk_7101d47310;
    if (!manager->field_0x608)
        return;

    if (self->timer > 0.0f) {
        self->timer -= dt;
        if (self->timer <= 0.0f)
            self->timer = 0.0f;
    }

    if (self->boosting) {
        self->boost = 0.0f;
        self->dir.x = 0.0f;
        self->dir.y = 0.0f;
        self->dir.z = 0.0f;
        f32 speed = fabsf(self->velocity.x);
        f32 limit;
        switch (self->mode) {
        case 1:
            limit = self->limit_mode1;
            break;
        case 2:
        case 4:
        case 5:
            limit = self->limit_mode2;
            break;
        case 3:
            limit = self->limit_mode3;
            break;
        default:
            limit = 0.0f;
            break;
        }
        if (manager->field_0xc8 != 0 || manager->field_0x10c) {
            UnkSlomoFlags* slomo = reinterpret_cast<UnkSlomoFlags*>(&manager->field_0xc4);
            if (slomo->enabled && !slomo->paused)
                limit *= fn_71010d6858();
        }
        UnkAbility* ability = fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
        if (ability && fn_710033cf20(ability)) {
            const auto* tunable = reinterpret_cast<const UnkMorphBallTunable_01d8*>(&g_unk_71726b0fc0);
            limit *= tunable->speed_booster_scale;
        }
        f32 excess = speed - fabsf(limit);
        if (excess > 0.0f) {
            f32 max = self->config->max_boost;
            self->boost = excess < max ? excess : max;
            self->dir = Normalized(self->velocity);
        }
    }
    fn_7100940490(self, dt);
    fn_7100942550(self, dt);
}
