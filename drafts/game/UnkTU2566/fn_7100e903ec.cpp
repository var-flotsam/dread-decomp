// Minimap manager unit (UnkTU2566): per-frame update of the minimap fade. The target is 0 while the player is
// in a HUD-hiding track or a state that hides it (or the game manager target says so), otherwise 1;
// the shown value moves toward it at 4 per second. Names and layouts are placeholders.
#include "types.h"

#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"

struct UnkHideHUDTrackRecord {
    u8 pad_0x0[0x18];
    bool field_0x18;
    bool field_0x19;
};

struct UnkMinimapPlayerState {
    u8 pad_0x0[0x10];
    s32 field_0x10;
};

struct UnkMinimapPlayerMode {
    u8 pad_0x0[0x50];
    UnkMinimapPlayerState* field_0x50;
};

struct UnkMinimapPlayer {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x88];
    UnkMinimapPlayerMode* field_0x2b0;
};

struct UnkMinimapFade {
    u8 pad_0x0[0xcc];
    bool field_0xcc;
    u8 pad_0xcd[0xd641bc - 0xcd];
    f32 field_0xd641bc;  // target
    f32 field_0xd641c0;  // shown
};

bool fn_71010e5cfc(char* p0);

void fn_7100e903ec(UnkMinimapFade* self, f32 dt) {
    bool hidden = false;
    auto* player = reinterpret_cast<UnkMinimapPlayer*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        if (UnkAnimationComponent* anim = player->field_0x220) {
            UnkTrackQuery query = {fn_7100c21e7c(), nullptr, -1.0f};
            if (fn_710117b08c(anim, &query)) {
                auto* record = reinterpret_cast<const UnkHideHUDTrackRecord*>(query.result);
                hidden = record->field_0x19 || record->field_0x18;
            }
        }
        UnkMinimapPlayerMode* mode = player->field_0x2b0;
        if (mode && mode->field_0x50)
            hidden |= mode->field_0x50->field_0x10 == 2;
    }
    if (UnkGameManagerTarget* target = fn_7100decab0(g_unk_7101d47310))
        hidden |= fn_71010e5cfc(static_cast<char*>(target->field_0x248));
    f32 goal = hidden ? 0.0f : 1.0f;
    self->field_0xd641bc = goal;
    if (self->field_0xcc) {
        f32 cur = self->field_0xd641c0;
        f32 step = dt * 4.0f;
        if (!(cur <= goal)) {
            cur -= step;
            self->field_0xd641c0 = cur;
            if (cur < goal)
                self->field_0xd641c0 = goal;
        } else {
            cur = step + cur;
            self->field_0xd641c0 = cur;
            if (cur > goal)
                self->field_0xd641c0 = goal;
        }
    }
}
