// game::logic::collision::CBroadphaseObject's unit: one of two members (+0x6c or +0xac) by the flag at
// +0x68. Names are placeholders.
#include "types.h"

struct UnkBroadphaseObject_b65a6 {
    u8 pad_0x0[0x68];
    bool flag;
    u8 pad_0x69[3];
    u8 first[0x40];
    u8 second[0x40];
};

void* fn_7100d35fe8(UnkBroadphaseObject_b65a6* self) { return self->flag ? self->first : self->second; }
