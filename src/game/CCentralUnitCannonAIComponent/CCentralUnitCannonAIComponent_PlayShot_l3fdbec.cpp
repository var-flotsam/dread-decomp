// CCentralUnitCannonAIComponent's unit (tools/tu_map.py): sends the owner's (+0x30) character class (+0x220) the shot
// track of the area kind at +0x146cc ("shot_cave", "shot_magma", "shot_lab", "shot_forest", "shot_sanc") with mode 2
// (as src/game_s2/UnkSpitSend.h). Returns what the send returns, false without a class or for another kind.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkCannonAI_l3fdbec {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x146cc - 0x38];
    u32 area;  // 0x146cc
};

bool fn_71003fdbec(UnkCannonAI_l3fdbec* self) {
    void* cls = self->owner->field_0x220;
    if (!cls)
        return false;
    u64 hash;
    switch (self->area) {
        // Declared before the first case label: no lifetime markers, so each keeps a slot of its own.
        base::reflection::UnkReflTypeIdArg mode_cave, mask_cave;
        base::reflection::UnkReflTypeIdArg mode_magma, mask_magma;
        base::reflection::UnkReflTypeIdArg mode_lab, mask_lab;
        base::reflection::UnkReflTypeIdArg mode_forest, mask_forest;
        base::reflection::UnkReflTypeIdArg mode_sanc, mask_sanc;
    case 0:
        hash = StringCrc64("shot_cave", 9);
        mode_cave.id = 2;
        mask_cave.id = UnkAll_SpitSend().value;
        return fn_7101176690(cls, hash, 0xf4061ff25a2163d6, &mode_cave, &g_unk_710164b958, &g_unk_7172769740,
                             &mask_cave);
    case 1:
        hash = StringCrc64("shot_magma", 10);
        mode_magma.id = 2;
        mask_magma.id = UnkAll_SpitSend().value;
        return fn_7101176690(cls, hash, 0xf4061ff25a2163d6, &mode_magma, &g_unk_710164b958, &g_unk_7172769740,
                             &mask_magma);
    case 2:
        hash = StringCrc64("shot_lab", 8);
        mode_lab.id = 2;
        mask_lab.id = UnkAll_SpitSend().value;
        return fn_7101176690(cls, hash, 0xf4061ff25a2163d6, &mode_lab, &g_unk_710164b958, &g_unk_7172769740,
                             &mask_lab);
    case 3:
        hash = StringCrc64("shot_forest", 11);
        mode_forest.id = 2;
        mask_forest.id = UnkAll_SpitSend().value;
        return fn_7101176690(cls, hash, 0xf4061ff25a2163d6, &mode_forest, &g_unk_710164b958, &g_unk_7172769740,
                             &mask_forest);
    case 4:
        hash = StringCrc64("shot_sanc", 9);
        mode_sanc.id = 2;
        mask_sanc.id = UnkAll_SpitSend().value;
        return fn_7101176690(cls, hash, 0xf4061ff25a2163d6, &mode_sanc, &g_unk_710164b958, &g_unk_7172769740,
                             &mask_sanc);
    }
    return false;
}
