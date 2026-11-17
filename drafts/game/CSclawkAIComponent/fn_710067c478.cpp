// CSclawkAIComponent's unit (tools/tu_map.py unit 1005): fn_710067c478 enters a state: it tells the owner's
// animation, picks a random wait (state 0) or starts or ends the time stored for state 2, and shows the
// owner's FX part while in state 2. Names and layouts are placeholders.
#include <stdlib.h>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "game/CFXComponent.h"
#include "game/UnkSwitchObject.h"
#include "types.h"

struct UnkSclawkTunables_l67c478 {
    u8 pad_0x0[0x50c];
    f32 wait_min;  // 0x50c
    f32 wait_max;  // 0x510
    f32 field_0x514;
    f32 field_0x518;
};

struct UnkSclawkPart_l67c478 {
    u8 pad_0x0[0xb];
    u8 id;
};

struct UnkSclawkOwner_l67c478 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x58];
    UnkWorld* world;  // 0x280
};

struct UnkSclawkAI_l67c478 {
    u8 pad_0x0[0x30];
    UnkSclawkOwner_l67c478* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    UnkSclawkTunables_l67c478* tunables;  // 0x146c8
    f32 wait;                             // 0x146d0
    f32 field_0x146d4;
    f32 field_0x146d8;
    u8 pad_0x146dc[4];
    u32 state;  // 0x146e0
    u8 pad_0x146e4[8];
    bool field_0x146ec;
    u8 pad_0x146ed[2];
    bool part_shown;  // 0x146ef
};

// Shows or hides the owner's FX part (the one whose name hashes to 0xe6de93263e1369e9).
static inline void ShowPart(UnkWorld* world, bool show) {
    UnkSclawkPart_l67c478* part =
        reinterpret_cast<UnkSclawkPart_l67c478*>(fn_7101152b90(world, 0xe6de93263e1369e9));
    if (part && !fn_710114e668(reinterpret_cast<UnkObject*>(world), part->id))
        part = nullptr;
    fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(part), show);
}

void fn_710067c478(UnkSclawkAI_l67c478* self, u32 state) {
    self->state = state;
    void* animation = self->owner->animation;
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
        fn_7101176690(animation, 0x6596032ae6e891b5, 0xf4061ff25a2163d6, &mode0, &g_unk_710164b958,
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
        fn_7101176690(animation, 0xd03b8b54ea583cb9, 0xf4061ff25a2163d6, &mode2, &g_unk_710164b958,
                      &g_unk_7172769740, &mask2);
        if (self->field_0x146ec)
            self->field_0x146d4 = self->tunables->field_0x514;
        else
            self->field_0x146d8 = self->tunables->field_0x518;
        UnkWorld* world = self->owner->world;
        if (world && !self->part_shown) {
            ShowPart(world, true);
            self->part_shown = true;
        }
        break;
    }
    case 3: {
        UnkWorld* world = self->owner->world;
        if (world && self->part_shown) {
            ShowPart(world, false);
            self->part_shown = false;
        }
        mode3.id = 0;
        mask3.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, 0x1cdf12aa84de383c, 0xf4061ff25a2163d6, &mode3, &g_unk_710164b958,
                      &g_unk_7172769740, &mask3);
        break;
    }
    }
}
