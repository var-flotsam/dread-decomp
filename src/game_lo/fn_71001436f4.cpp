// Runs fn_71001c2764 for each slot of the object at +0x138 (the count lives in its +0x78
// block), stopping the calls at the first failure; true when all succeeded.
#include "game/UnkSlots.h"
#include "types.h"

struct UnkStruct_71001436f4 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};

bool fn_71001436f4(UnkStruct_71001436f4* self, u32 a, const base::math::CVector4D* b, u32 c) {
    u32 n = self->slots->info->count;
    bool ok = true;
    for (u32 i = 0; i < n; ++i)
        ok = ok && fn_71001c2764(self->slots, i, a, b, c, true);
    return ok;
}
