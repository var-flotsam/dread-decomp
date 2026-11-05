// CNailongThornsAttack's unit. fn_710060b274: the attack can start when CAttack's test passes and, unless
// the owner's flag at +0xec is set, a freshly picked string id (fn_710060b344, moved into +0x78) is not the
// global empty id. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkSelf;
u32 fn_7100360b84(UnkSelf* self);

struct UnkThornsOwner_b68a4 {
    u8 pad_0x0[0xec];
    bool field_0xec;
};

struct UnkThornsAttack_b68a4 {
    u8 pad_0x0[0x28];
    UnkThornsOwner_b68a4* owner;  // 0x28
    u8 pad_0x30[0x48];
    CStrId id;  // 0x78
};

CStrId fn_710060b344(UnkThornsAttack_b68a4* self);

bool fn_710060b274(UnkThornsAttack_b68a4* self) {
    bool ok = reinterpret_cast<bool (*)(UnkThornsAttack_b68a4*)>(fn_7100360b84)(self);
    if (!ok || self->owner->field_0xec) {
        asm("" : "+r"(ok));
        return ok;
    }
    self->id = fn_710060b344(self);
    return self->id.data != g_unk_7101d470e0.data;
}
