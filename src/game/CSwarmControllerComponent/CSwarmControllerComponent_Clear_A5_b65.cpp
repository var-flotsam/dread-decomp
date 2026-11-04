// CSwarmControllerComponent's unit: clears the field at +0x28 of every member of the list at +0x208
// (count at +0x210), then empties the list. Names are placeholders.
#include "types.h"

struct UnkSwarmMember_A5_b65 {
    u8 pad_0x0[0x28];
    u32 field_0x28;
};

struct UnkSwarmList_A5_b65 {
    u8 pad_0x0[0x208];
    UnkSwarmMember_A5_b65** members;
    u32 count;
};

void fn_710074c304(UnkSwarmList_A5_b65* self) {
    for (UnkSwarmMember_A5_b65** it = self->members, **end = self->members + self->count; it != end; ++it)
        (*it)->field_0x28 = 0;
    self->count = 0;
}
