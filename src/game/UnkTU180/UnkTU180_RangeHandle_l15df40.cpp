// UnkTU180: looks up the uniform range a key object names in one of three tables and writes its
// handle (an index plus 15) to `out`; false when absent or its block is empty. Names and layouts
// are placeholders.
#include "base/UnkCloneCalls.h"
#include "types.h"

struct UnkUniformBlock {
    u8 pad_0x00[0x38];
    u64 field_0x38;  // size in bytes
    u8 pad_0x40[0x98];
};
static_assert(sizeof(UnkUniformBlock) == 0xd8);


struct UnkUniformRangeEntry {
    u8 pad_0x00[0x38];
    u32 block;
    u32 index;
};
static_assert(sizeof(UnkUniformRangeEntry) == 0x40);

struct UnkUniformRangeTag;

struct UnkUniformTable {
    u8 pad_0x00[0x78];
    base::UnkKeyedArray<UnkUniformRangeTag, u64, UnkUniformRangeEntry> ranges;
    u8 pad_0x94[0x1c];
    UnkUniformBlock* blocks;
};
static_assert(__builtin_offsetof(UnkUniformTable, blocks) == 0xb0);

struct UnkUniformTables {
    UnkUniformTable* tables[3];
};

struct UnkUniformKey {
    u8 pad_0x00[0x30];
    u64 id;
};

bool fn_710015df40(UnkUniformTables* self, u32 kind, u64* out, UnkUniformKey** key) {
    *out = 0;
    if (!self)
        return false;
    u64 id = (*key)->id;
    if (!id)
        return false;
    UnkUniformTable* table;
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
    if (!table)
        return false;
    UnkUniformRangeEntry* entry = table->ranges.Find(id);
    if (entry == table->ranges.End())
        return false;
    if (!table->blocks[entry->block].field_0x38)
        return false;
    *out = entry->index + 15;
    return true;
}
