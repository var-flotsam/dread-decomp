// CMagmaKraidPistonPlatformComponent's unit (tools/tu_map.py): sends the owner's (+0x30)
// sub-object at +0x220 a fixed hash with mode 0, as UnkSpitSend (src/game_s2/UnkSpitSend.h) with no
// null checks, then clears the word at +0x2b0. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkPistonOwner_b54 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkPistonPlatform_b54 {
    u8 pad_0x0[0x30];
    UnkPistonOwner_b54* owner;
    u8 pad_0x38[0x2b0 - 0x38];
    u32 field_0x2b0;
};

void fn_7100918c98(UnkPistonPlatform_b54* self) {
    UnkSpitSend(self->owner->field_0x220, 0xb43f302c7cd8f802, 0xf4061ff25a2163d6);
    self->field_0x2b0 = 0;
}
