// A destructor in CMinimapData's unit: frees two arrays of words at 0x40 and 0x18, then releases the
// string id at 0x10 (the shape of fn_710019506c, src/game_s1/fn_710019506c.cpp). Names and the
// layout are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkMinimapIdArrays {
    u8 pad_0x0[0x10];
    base::UnkStrIdMember id;
    base::UnkVector<u32> values_0x18;
    base::UnkVector<u32> values_0x40;

    __attribute__((always_inline)) ~UnkMinimapIdArrays() {
        values_0x40.Free();
        values_0x18.Free();
    }
};

void fn_7100e51664(UnkMinimapIdArrays* self) { self->~UnkMinimapIdArrays(); }
