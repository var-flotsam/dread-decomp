// CAutsharpSpawnPointComponent unit (tools/tu_map.py): sends the owner's (+0x30) sub-object at +0x220 a
// fixed hash with mode 0 (as UnkSpitSend, src/game_s2/UnkSpitSend.h), then clears the flag at +0x400.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkAutsharpSendOwner_b56 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkAutsharpSend_b56 {
    u8 pad_0x0[0x30];
    UnkAutsharpSendOwner_b56* owner;
    u8 pad_0x38[0x400 - 0x38];
    bool field_0x400;
};

void fn_7100a145a8(UnkAutsharpSend_b56* self) {
    void* target = self->owner->field_0x220;
    if (target) {
        UnkSpitSend(target, 0x6a17390fc912b900, 0xf4061ff25a2163d6);
        self->field_0x400 = false;
    }
}
