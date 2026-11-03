// Unit 109 (tools/tu_map.py): runs fn_7100119c0c on every 0x48-byte entry of an array (data +0x0,
// count +0x8) and returns whether all of them succeeded (every entry runs). Names and layouts are
// placeholders.
#include "types.h"

struct UnkUnit109Entry {
    u8 data[0x48];
};

struct UnkUnit109Array {
    UnkUnit109Entry* data;
    u32 count;
};

bool fn_7100119c0c(UnkUnit109Entry* entry);

bool fn_71001252dc(UnkUnit109Array* self) {
    bool ok = true;
    for (UnkUnit109Entry *it = self->data, *end = self->data + self->count; it != end; ++it)
        ok &= fn_7100119c0c(it);
    return ok;
}
