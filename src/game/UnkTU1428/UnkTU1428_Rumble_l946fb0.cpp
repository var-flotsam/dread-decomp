// Unit 1428 (CMorphBallMovement's neighbours): picks a kind (1 to 3) from the angle of the ball's
// velocity when none is pending, then, unless a timer runs or the character class ignores it, passes
// that kind to the game manager's fn_7100df31b8; always clears the pending kind and two flags.
// Names and layouts are placeholders.
#include <math.h>
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU437.h"
#include "types.h"

using base::global::CStrId;

// The two angle limits in the aim tunables (g_unk_71726b1438).
struct UnkAimAngles_l946fb0 {
    u8 pad_0x0[0xa8];
    f32 upper;  // +0xa8
    u8 pad_0xac[0xb8 - 0xac];
    f32 lower;  // +0xb8
};

struct UnkBallState_l946fb0 {
    u8 pad_0x0[0x70];
    s32 mode;  // +0x70
};

struct UnkBallActor_l946fb0 {
    u8 pad_0x0[0x1b8];
    UnkBallState_l946fb0* state;  // +0x1b8
    u8 pad_0x1c0[0x220 - 0x1c0];
    UnkAnimationComponent* char_class;  // +0x220
};

struct UnkBallFlags_l946fb0 {
    bool blocked;  // +0x0
    bool field_0x1;
    u8 pad_0x2[0x24 - 0x2];
    bool field_0x24;
    u8 pad_0x25[0xf8 - 0x25];
    bool armed;  // +0xf8
};

struct UnkBallMovement_l946fb0 {
    u8 pad_0x0[0x30];
    UnkBallActor_l946fb0* actor;  // +0x30
    u8 pad_0x38[0x6e8 - 0x38];
    f32 speed_x;  // +0x6e8
    f32 speed_y;  // +0x6ec
    u8 pad_0x6f0[0x2530 - 0x6f0];
    s32 kind;      // +0x2530
    f32 timer;     // +0x2534
    UnkBallFlags_l946fb0 flags;  // +0x2538
};

void fn_7100946fb0(UnkBallMovement_l946fb0* self) {
    UnkBallState_l946fb0* state = self->actor->state;
    if (!state || state->mode == 2)
        return;

    if (self->kind == 0 && self->flags.armed && self->speed_y >= 0.65f) {
        const UnkAimAngles_l946fb0* angles = reinterpret_cast<const UnkAimAngles_l946fb0*>(&g_unk_71726b1438);
        f32 angle = atan2f(self->speed_y, fabsf(self->speed_x));
        if (angle > angles->lower)
            self->kind = angle < angles->upper ? 3 : 1;
        else
            self->kind = 1;
    }

    if (self->kind != 0) {
        UnkAnimationComponent* char_class = self->actor->char_class;
        if (self->timer > 0.0f || (char_class && fn_710117b348(char_class, fn_7100c2a89c())))
            self->kind = 0;
    }

    if (self->kind != 0 && !self->flags.blocked) {
        switch (self->kind) {
        case 1:
            fn_7100df31b8(g_unk_7101d47310, true, false, 1, g_unk_7101d470e0, false);
            break;
        case 2:
            fn_7100df31b8(g_unk_7101d47310, true, false, 2, g_unk_7101d470e0, false);
            break;
        case 3:
            fn_7100df31b8(g_unk_7101d47310, true, false, 3, g_unk_7101d470e0, false);
            break;
        }
    }

    self->flags.field_0x24 = false;
    self->flags.field_0x1 = false;
    self->kind = 0;
}
