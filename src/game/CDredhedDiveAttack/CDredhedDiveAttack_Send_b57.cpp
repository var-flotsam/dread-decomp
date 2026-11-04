// CDredhedDiveAttack's unit (tools/tu_map.py): sends the owner's (+0x8) sub-object at +0x220 a u32 under
// the hash passed in (refl_EmitU32, base/reflection/UnkReflStatics.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkDredhedSendOwner_b57 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkDredhedSend_b57 {
    u8 pad_0x0[0x8];
    UnkDredhedSendOwner_b57* owner;
};

void fn_71004e5c60(UnkDredhedSend_b57* self, u64 hash, const u32* value) {
    void* target = self->owner->field_0x220;
    if (target) {
        base::reflection::refl_EmitU32(target, hash, 0xf4061ff25a2163d6, *value, &g_unk_710164b958,
                                       &g_unk_7172769740);
    }
}
