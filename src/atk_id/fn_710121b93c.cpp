// nn::atk::detail::SequenceSound vtable slot 0x28: true if forced, else state > 2 when started.
#include "types.h"

struct UnkSequenceSound {
    u8 unk_0x0[0xf0];
    bool flag_0xf0;
    u8 unk_0xf1[0x232 - 0xf1];
    bool flag_0x232;
    u8 unk_0x233[0x500 - 0x233];
    u8 state;
};

bool fn_710121b93c(const UnkSequenceSound* self) {
    if (self->flag_0x232)
        return true;
    if (!self->flag_0xf0)
        return false;
    return self->state > 2;
}
