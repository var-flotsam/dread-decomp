// nn::atk::detail::AdvancedWaveSound vtable slot 0x28: a flag gated by another flag.
#include "types.h"

struct UnkAdvancedWaveSound {
    u8 unk_0x0[0xf0];
    bool flag_0xf0;
    u8 unk_0xf1[0x9a0 - 0xf1];
    u8 field_0x9a0;
};

bool fn_710120c3d0(const UnkAdvancedWaveSound* self) {
    if (!self->flag_0xf0)
        return false;
    return self->field_0x9a0 != 0;
}
