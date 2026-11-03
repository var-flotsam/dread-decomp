// From unit 181 of tools/tu_map.py (0x710015e4b0-0x710015f0f8, no reflected class): a lookup through
// the table at +0 (fn_710015e600) whose result index is cached per slot in the word array at
// +0x110 (-1 when not found) and written to `out` on success. Names are placeholders.
#include "types.h"

struct UnkUnit181Table;
bool fn_710015e600(UnkUnit181Table* table, const void* key, u32* outIndex);

struct UnkUnit181Cache {
    UnkUnit181Table* table;
    u8 pad_0x8[0x108];
    u32 indices[1];
};

void fn_710015e5a0(UnkUnit181Cache* self, u32 slot, const void* key, u32* out) {
    u32 index = 0xffffffff;
    if (!self)
        return;
    if (fn_710015e600(self->table, key, &index))
        *out = index;
    self->indices[slot] = index;
}
