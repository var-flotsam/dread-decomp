// From the minimap manager's unit: whether the id is in the array of ids at +0xd642c8 (count at
// +0xd642d0) of the manager. Names and layouts are placeholders.
#include "types.h"

struct UnkMinimapIds {
    u8 pad_0x0[0xd642c8];
    u64* ids;
    u32 count;
};

bool fn_7100e9b914(UnkMinimapIds* self, const u64* id) {
    u64* it = self->ids;
    for (u64 n = self->count; n != 0; --n, ++it)
        if (*it == *id)
            return true;
    return false;
}
