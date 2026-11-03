// CSamusGunComponent (tools/tu_map.py; the name from reflection): with the flag at +0x340 set,
// whether the kind is 0, 1 or 6. Names are placeholders.
#include "types.h"

struct UnkSamusGun_b45a2 {
    u8 pad_0x0[0x340];
    bool field_0x340;
};

bool fn_710085aa34(UnkSamusGun_b45a2* self, u32 kind) {
    if (!self->field_0x340)
        return false;
    asm("");  // keeps the early return a branch, as in the original
    return kind < 2 || kind == 6;
}
