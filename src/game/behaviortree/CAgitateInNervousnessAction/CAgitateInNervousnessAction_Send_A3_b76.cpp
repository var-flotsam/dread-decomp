// behaviortree::CAgitateInNervousnessAction (tools/tu_map.py unit 995), fn_7100674bf0: sends the owner's (+0x18)
// character class (+0x220), when there is one, a fixed hash with mode 1 (written out as src/game_s2/UnkSpitSend.h
// sends mode 0) and returns 7. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkAgitateSendAction_A3_b76 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
};

u32 fn_7100674bf0(UnkAgitateSendAction_A3_b76* self) {
    void* cls = self->owner->field_0x220;
    if (cls) {
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 1;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(cls, 0xe2ea001660c8ac66, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask);
    }
    return 7;
}
