// From unit 180 of tools/tu_map.py (0x710015c520-0x710015e4ac, no reflected class): looks `key` up
// in one of three tables (kind 0, 1 or 2: the pointers at +0, +8, +0x10; none otherwise) with
// fn_710015dc04 and writes the index found to `out`, else 0. Names are placeholders.
#include "types.h"

struct UnkUnit180Table;
bool fn_710015dc04(UnkUnit180Table* table, const void* key, u32* outIndex);

struct UnkUnit180Tables {
    UnkUnit180Table* tables[3];
};

void fn_710015db90(UnkUnit180Tables* self, u32 kind, u64* out, const void* key) {
    *out = 0;
    if (!self || !key)
        return;
    UnkUnit180Table* table;
    switch (kind) {
    case 0:
        table = self->tables[0];
        break;
    case 1:
        table = self->tables[1];
        break;
    case 2:
        table = self->tables[2];
        break;
    default:
        table = nullptr;
        break;
    }
    u32 index = 0xffffffff;
    if (fn_710015dc04(table, key, &index))
        *out = index;
}
