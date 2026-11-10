// Unit 283 (tools/tu_map.py, no name hint): sets the shown flag of a toggle item (as fn_710020ac34, game/UnkToggle.h)
// and, when it changes, reports the new state to the listener; switching it off also hands the style
// source's block (+0x80) to fn_710016ae30, and the float at +0x124 is cleared. Names and layouts are
// placeholders.
#include "engine/scene/UnkToggle.h"
#include "types.h"

struct UnkPart132;
void fn_710016ae30(void* self);

void fn_7100206ecc(UnkPart132* part, bool on) {
    UnkToggle* self = reinterpret_cast<UnkToggle*>(part);
    if (on != self->flag_0xf9) {
        self->flag_0xf9 = on;
        if (self->listener_0x8) {
            fn_71001eef58(self->listener_0x8, self, self->flag_0xf8 && on);
            fn_71001eef74(self->listener_0x8, self, self->flag_0xf9 && self->flag_0xf6);
        }
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(self) + 0x124) = 0;
        if (!on)
            fn_710016ae30(reinterpret_cast<u8*>(self->src_0x108) + 0x80);
    }
}
