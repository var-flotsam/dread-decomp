// CAcidBlobsLaunchPattern's unit (tools/tu_map.py unit 902): the value at (row, column) of the
// pattern's rows (0x28-byte entries at +0x110, count at +0x118; each row's own count at +8), read
// through fn_71005d6c90; 0 out of range. Names are placeholders.
#include "types.h"

struct UnkAcidBlobsRow_b45a2 {
    void* data;
    u32 count;
    u8 pad_0xc[0x1c];
};

u64* fn_71005d6c90(UnkAcidBlobsRow_b45a2* row, u32 column);

struct UnkAcidBlobsPattern_b45a2 {
    u8 pad_0x0[0x110];
    UnkAcidBlobsRow_b45a2* rows;
    u32 count;
};

u64 fn_71005d6c38(UnkAcidBlobsPattern_b45a2* self, u32 row, u32 column) {
    if (row < self->count && column < self->rows[row].count)
        return *fn_71005d6c90(&self->rows[row], column);
    return 0;
}
