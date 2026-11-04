// CAutsharpAIComponent's unit (tools/tu_map.py): sends the owner's (+0x30) sub-object at +0x220 a zero
// under the hash held at +0x30 of the object passed in (refl_EmitU32, base/reflection/UnkReflStatics.h).
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkAutsharpSendOwner_b57 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkAutsharpSend_b57 {
    u8 pad_0x0[0x30];
    UnkAutsharpSendOwner_b57* owner;
};

struct UnkAutsharpHashed_b57 {
    u8 pad_0x0[0x30];
    u64 hash;
};

void fn_71003c91cc(UnkAutsharpSend_b57* self, UnkAutsharpHashed_b57* const* item) {
    void* target = self->owner->field_0x220;
    if (target) {
        base::reflection::refl_EmitU32(target, (*item)->hash, 0xf4061ff25a2163d6, 0, &g_unk_710164b958,
                                       &g_unk_7172769740);
    }
}
