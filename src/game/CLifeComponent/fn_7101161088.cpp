// In CLifeComponent's unit (unit 2715 of tools/tu_map.py, the name from reflection): whether the
// component is alive, that is, it is flagged so (+0x64) or its life (+0x68) is above zero. Names
// and layouts are placeholders.
#include "types.h"

struct UnkLifeComponent {
    u8 pad_0x0[0x64];
    bool field_0x64;
    u8 pad_0x65[0x3];
    f32 life;  // 0x68
};

// NON_MATCHING(minor): the original branches to two returns where Clang selects (cset); same
// behaviour.
bool fn_7101161088(void* component) {
    UnkLifeComponent* self = static_cast<UnkLifeComponent*>(component);
    if (self->field_0x64)
        return true;
    if (self->life > 0.0f)
        return true;
    return false;
}
