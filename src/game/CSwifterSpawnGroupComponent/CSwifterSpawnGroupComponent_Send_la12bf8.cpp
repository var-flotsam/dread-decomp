// CSwifterSpawnGroupComponent's unit (tools/tu_map.py): sends the target at +0x438 one of four ids with mode 2
// (as src/game_s2/UnkSpitSend.h), picked by a coin flip and whether the kind at +0x424 is 1.
// Names and layouts are placeholders.
#include <stdlib.h>
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkSwifterSpawnGroup_la12bf8 {
    u8 pad_0x0[0x424];
    s32 kind;  // 0x424
    u8 pad_0x428[0x438 - 0x428];
    void* target;  // 0x438
};

void fn_7100a12bf8(UnkSwifterSpawnGroup_la12bf8* self) {
    bool flip = rand() & 1;
    void* target = self->target;
    if (flip) {
        switch (self->kind) {
            // Declared before the first case label: no lifetime markers, so each keeps a slot of its own.
            base::reflection::UnkReflTypeIdArg mode_a, mask_a;
            base::reflection::UnkReflTypeIdArg mode_b, mask_b;
        case 1:
            mode_a.id = 2;
            mask_a.id = UnkAll_SpitSend().value;
            fn_7101176690(target, 0xf6947380f5e30a61, 0xf4061ff25a2163d6, &mode_a, &g_unk_710164b958,
                          &g_unk_7172769740, &mask_a);
            break;
        default:
            mode_b.id = 2;
            mask_b.id = UnkAll_SpitSend().value;
            fn_7101176690(target, 0xf0f91d1596e6b6d3, 0xf4061ff25a2163d6, &mode_b, &g_unk_710164b958,
                          &g_unk_7172769740, &mask_b);
            break;
        }
    } else {
        switch (self->kind) {
            base::reflection::UnkReflTypeIdArg mode_c, mask_c;
            base::reflection::UnkReflTypeIdArg mode_d, mask_d;
        case 1:
            mode_c.id = 2;
            mask_c.id = UnkAll_SpitSend().value;
            fn_7101176690(target, 0xb13e09695e04f555, 0xf4061ff25a2163d6, &mode_c, &g_unk_710164b958,
                          &g_unk_7172769740, &mask_c);
            break;
        default:
            mode_d.id = 2;
            mask_d.id = UnkAll_SpitSend().value;
            fn_7101176690(target, 0xb75367fc3d0149e7, 0xf4061ff25a2163d6, &mode_d, &g_unk_710164b958,
                          &g_unk_7172769740, &mask_d);
            break;
        }
    }
}