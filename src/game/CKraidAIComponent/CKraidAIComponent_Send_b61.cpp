// CKraidAIComponent's unit (tools/tu_map.py): once the flag at +0x14e01 is set (it is cleared here),
// sends track 0xf27c02c5ee576047 with mode 0 (as src/game_s2/UnkSpitSend.h) to the owner's (+0x30)
// character class and to the player's, then calls vf_0x1e8(false) on the player's +0x208 object.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkPlayer;

struct UnkKraidPlayerPart_b61 {
    virtual void vf_0x0();
    void vf_0x1e8(bool on) {
        reinterpret_cast<void (*)(UnkKraidPlayerPart_b61*, bool)>((*reinterpret_cast<void***>(this))[0x1e8 / 8])(
            this, on);
    }
};

struct UnkKraidAI_b61 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x14e01 - 0x38];
    bool field_0x14e01;
};

void fn_7100601a74(UnkKraidAI_b61* self) {
    if (!self->field_0x14e01)
        return;
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg mode1;
    base::reflection::UnkReflTypeIdArg mask1;
    base::reflection::UnkReflTypeIdArg mode2;
    base::reflection::UnkReflTypeIdArg mask2;
start:
    self->field_0x14e01 = false;
    const u64 track = 0xf27c02c5ee576047;
    if (void* cls = self->owner->field_0x220) {
        mode1.id = 0;
        mask1.id = UnkAll_SpitSend().value;
        fn_7101176690(cls, track, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958, &g_unk_7172769740, &mask1);
    }
    UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    if (void* cls = player->field_0x220) {
        mode2.id = 0;
        mask2.id = UnkAll_SpitSend().value;
        fn_7101176690(cls, track, 0xf4061ff25a2163d6, &mode2, &g_unk_710164b958, &g_unk_7172769740, &mask2);
    }
    if (UnkKraidPlayerPart_b61* part = static_cast<UnkKraidPlayerPart_b61*>(player->field_0x208))
        part->vf_0x1e8(false);
}
