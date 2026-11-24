// CCaterzillaAIComponent's unit (tools/tu_map.py): sends the owner's (+0x30) animation (+0x220) a mode-1 request
// (as src/game_s2/UnkSpitSend.h) under one of four ids, picked by the two flags. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkCaterzillaOwner_l3f7034 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
};

struct UnkCaterzillaAI_l3f7034 {
    u8 pad_0x0[0x30];
    UnkCaterzillaOwner_l3f7034* owner;  // 0x30
};

void fn_71003f7034(UnkCaterzillaAI_l3f7034* self, bool first, bool second) {
    void* animation = self->owner->animation;
    if (!animation)
        return;
    if (first) {
        switch (static_cast<int>(second)) {
            // Declared before the first case label: no lifetime markers, so each keeps a slot of its own.
            base::reflection::UnkReflTypeIdArg mode_a, mask_a;
            base::reflection::UnkReflTypeIdArg mode_b, mask_b;
        case 1:
            mode_a.id = 1;
            mask_a.id = UnkAll_SpitSend().value;
            CAnimationComponent_SendActionToTrack(animation, 0x8b248af7c8f3f4af, 0xf4061ff25a2163d6, &mode_a, &g_unk_710164b958,
                          &g_unk_7172769740, &mask_a);
            break;
        default:
            mode_b.id = 1;
            mask_b.id = UnkAll_SpitSend().value;
            CAnimationComponent_SendActionToTrack(animation, 0x01717b42bd010fe4, 0xf4061ff25a2163d6, &mode_b, &g_unk_710164b958,
                          &g_unk_7172769740, &mask_b);
            break;
        }
    } else {
        switch (static_cast<int>(second)) {
            base::reflection::UnkReflTypeIdArg mode_c, mask_c;
            base::reflection::UnkReflTypeIdArg mode_d, mask_d;
        case 1:
            mode_c.id = 1;
            mask_c.id = UnkAll_SpitSend().value;
            CAnimationComponent_SendActionToTrack(animation, 0x15b54864b831a107, 0xf4061ff25a2163d6, &mode_c, &g_unk_710164b958,
                          &g_unk_7172769740, &mask_c);
            break;
        default:
            mode_d.id = 1;
            mask_d.id = UnkAll_SpitSend().value;
            CAnimationComponent_SendActionToTrack(animation, 0x3b608d17a9855d89, 0xf4061ff25a2163d6, &mode_d, &g_unk_710164b958,
                          &g_unk_7172769740, &mask_d);
            break;
        }
    }
}
