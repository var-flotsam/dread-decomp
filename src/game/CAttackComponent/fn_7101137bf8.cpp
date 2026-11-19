// CAttackComponent's unit (tools/tu_map.py unit 2706): plays the action `id` names on the owner's
// (+0x30) usual track (key 0xf4061ff25a2163d6, fn_7101178394) with `flag` as its mode; an empty id
// counts as played. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "types.h"

using base::global::CStrId;

struct UnkAnimComponent_ovb0009;
bool fn_7101178394(UnkAnimComponent_ovb0009* self, u64 key, void* id, const u32* flags);

struct UnkAttackComponent_l137bf8 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
bool fn_7101137bf8(UnkAttackComponent_l137bf8* self, const CStrId* id, bool flag) {
    if (id->data != &g_unk_7101d47108->empty) {
        alignas(8) u32 mode = flag;
        if (!fn_7101178394(static_cast<UnkAnimComponent_ovb0009*>(self->owner->field_0x220), 0xf4061ff25a2163d6,
                           reinterpret_cast<void*>(id->data->Key()), &mode))
            return false;
    }
    return true;
}
