// CSclawkAIComponent's unit (tools/tu_map.py unit 1005): fn_710067c478 enters a state: it tells the owner's
// animation, picks a random wait (state 0) or starts or ends the time stored for state 2, and shows the
// owner's FX part while in state 2. Names and layouts are placeholders.
#include <stdlib.h>

#include "../../game_s2/UnkSpitSend.h"
#include "game/CSclawkAIComponent.h"
#include "game/UnkSclawkAIComponent.h"
#include "types.h"

void fn_710067c478(UnkSclawkAIComponent* self, u32 state) {
    self->state = state;
    void* animation = self->owner->field_0x220;
    switch (state) {
        base::reflection::UnkReflTypeIdArg mode0;
        base::reflection::UnkReflTypeIdArg mask0;
        base::reflection::UnkReflTypeIdArg mode2;
        base::reflection::UnkReflTypeIdArg mask2;
        base::reflection::UnkReflTypeIdArg mode3;
        base::reflection::UnkReflTypeIdArg mask3;
    case 0: {
        mode0.id = 1;
        mask0.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(animation, 0x6596032ae6e891b5, 0xf4061ff25a2163d6, &mode0, &g_unk_710164b958,
                      &g_unk_7172769740, &mask0);
        f32 min = self->tunables->wait_min;
        f32 max = self->tunables->wait_max;
        f32 r = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
        self->wait = min + (max - min) * r;
        break;
    }
    case 2: {
        mode2.id = 0;
        mask2.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(animation, 0xd03b8b54ea583cb9, 0xf4061ff25a2163d6, &mode2, &g_unk_710164b958,
                      &g_unk_7172769740, &mask2);
        if (self->field_0x146ec)
            self->field_0x146d4 = self->tunables->field_0x514;
        else
            self->field_0x146d8 = self->tunables->field_0x518;
        void* parts = self->owner->field_0x280;
        if (parts && !self->part_shown) {
            ShowSclawkPart(parts, true);
            self->part_shown = true;
        }
        break;
    }
    case 3: {
        void* parts = self->owner->field_0x280;
        if (parts && self->part_shown) {
            ShowSclawkPart(parts, false);
            self->part_shown = false;
        }
        mode3.id = 0;
        mask3.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(animation, 0x1cdf12aa84de383c, 0xf4061ff25a2163d6, &mode3, &g_unk_710164b958,
                      &g_unk_7172769740, &mask3);
        break;
    }
    }
}
