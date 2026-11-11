// CSamusGunComponent unit (tools/tu_map.py, the name from reflection): when there is a current gun
// (fn_710083cd4c), a pending gun name (+0x168, not the empty string), the timer at +0x17c below 0.01
// and the gun accepts (virtual slot 0x140), switches to the pending gun (fn_710083f32c).
// Names are placeholders.
#include "base/global/CStrId.h"
#include "game/CGunComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkSamusGunComponent {
    u8 unk_0x0[0x168];
    CStrId pending;  // 0x168
    u8 unk_0x170[0xc];
    f32 timer;  // 0x17c
};

void fn_710083f32c(UnkSamusGunComponent* self, const CStrId* name, bool a, bool b, bool c);

void fn_7100858790(UnkSamusGunComponent* self) {
    char* gun = fn_710083cd4c(self);
    if (gun == nullptr)
        return;
    if (self->pending.data == &g_unk_7101d47108->empty)
        return;
    if (!(self->timer < 0.01f))
        return;
    using Fn = bool (*)(char*);
    if (!(*reinterpret_cast<Fn**>(gun))[0x140 / 8](gun))
        return;
    fn_710083f32c(self, &self->pending, true, true, false);
}
