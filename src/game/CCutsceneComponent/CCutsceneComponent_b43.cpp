// CCutsceneComponent unit (tools/tu_map.py). fn_71007a93cc: stops the component's player (+0x200)
// through fn_7101069138; with the flag set it first clears the player's byte at +0x504 and afterwards
// its word at +0x4c0. Names and layouts are placeholders.
#include "types.h"

struct UnkCutscenePlayer_b43a2 {
    u8 pad_0x0[0x4c0];
    u32 field_0x4c0;
    u8 pad_0x4c4[0x40];
    u8 field_0x504;
};

struct UnkCutsceneComponent_b43a2 {
    u8 pad_0x0[0x200];
    UnkCutscenePlayer_b43a2* player;  // 0x200
};

void fn_7101069138(UnkCutscenePlayer_b43a2* self, bool flag);

void fn_71007a93cc(UnkCutsceneComponent_b43a2* self, bool reset) {
    if (UnkCutscenePlayer_b43a2* player = self->player) {
        if (reset) {
            // A plain byte store (may alias anything): the original reads the player again after it.
            reinterpret_cast<u8*>(player)[0x504] = 0;
            fn_7101069138(self->player, false);
            self->player->field_0x4c0 = 0;
        } else {
            fn_7101069138(player, false);
        }
    }
}
