// From unit 2730 of tools/tu_map.py (0x7101196284-0x710119acb0, no reflected class): whether the
// object at +0x58 is in state 2, 3 or 6 (its word at +0x4b0); false when there is none.
#include "types.h"

struct UnkStateHolder_7101196fd8 {
    u8 pad_0x0[0x4b0];
    s32 state;
};

struct UnkStateOwner_7101196fd8 {
    u8 pad_0x0[0x58];
    UnkStateHolder_7101196fd8* holder;
};

bool fn_7101196fd8(void* self) {
    UnkStateHolder_7101196fd8* holder = static_cast<UnkStateOwner_7101196fd8*>(self)->holder;
    if (!holder)
        return false;
    switch (holder->state) {
    case 2:
    case 3:
    case 6:
        return true;
    default:
        return false;
    }
}
