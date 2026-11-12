// Minimap manager unit (UnkTU2566): per-frame update of the minimap fade. The target is 0 while the player is
// in a HUD-hiding track or a state that hides it (or the game manager target says so), otherwise 1;
// the shown value moves toward it at 4 per second. Names and layouts are placeholders.
#include "types.h"

#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"

struct UnkHideFlags {
    u8 pad_0x0[0x18];
    u8 field_0x18;
    u8 field_0x19;
};

struct UnkPlayerState {
    u8 pad_0x0[0x10];
    s32 field_0x10;
};

struct UnkPlayerMode {
    u8 pad_0x0[0x50];
    UnkPlayerState* field_0x50;
};

struct UnkPlayer {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x88];
    UnkPlayerMode* field_0x2b0;
};

struct UnkMinimapFade {
    u8 pad_0x0[0xcc];
    u8 field_0xcc;
    u8 pad_0xcd[0xd641bc - 0xcd];
    f32 field_0xd641bc;  // target
    f32 field_0xd641c0;  // shown
};

// Same function as the lifted fn_71010e5cfc; read here as the byte it returns.
u8 UnkIsHidden(char* p0) asm("fn_71010e5cfc");

void fn_7100e903ec(f32 dt, UnkMinimapFade* self) {
    u8 hidden = 0;
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (player) {
        UnkAnimationComponent* anim = player->field_0x220;
        if (anim) {
            UnkTrackQuery query = {fn_7100c21e7c(), nullptr, -1.0f};
            if (fn_710117b08c(anim, &query)) {
                const UnkHideFlags* flags = reinterpret_cast<const UnkHideFlags*>(query.result);
                hidden = flags->field_0x19 ? 1 : flags->field_0x18;
            }
        }
        UnkPlayerMode* mode = player->field_0x2b0;
        if (mode) {
            bool inState = false;
            if (mode->field_0x50) {
                inState = mode->field_0x50->field_0x10 == 2;
            }
            hidden = inState | (hidden & 1);
        }
        hidden &= 1;
    }
    UnkGameManagerTarget* target = fn_7100decab0(g_unk_7101d47310);
    if (target) {
        hidden |= UnkIsHidden(static_cast<char*>(target->field_0x248));
    }
    f32 goal = 1.0f;
    if (hidden & 1) goal = 0.0f;
    self->field_0xd641bc = goal;
    if (self->field_0xcc) {
        f32 cur = self->field_0xd641c0;
        f32 step = dt * 4.0f;
        if (cur <= goal) {
            cur = step + cur;
            self->field_0xd641c0 = cur;
            if (cur <= goal) return;
        } else {
            cur -= step;
            self->field_0xd641c0 = cur;
            if (goal <= cur) return;
        }
        self->field_0xd641c0 = goal;
    }
}
