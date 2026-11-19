// UnkTU180: looks up the uniform range a key object names in one of three tables and writes its
// handle (an index plus 15) to `out`; false when absent or its block is empty. Names and layouts
// are placeholders.
#include "base/UnkCloneCalls.h"
#include "game/UnkUniformTables.h"
#include "types.h"

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
    UnkUniformRangeEntry* entry = table->ranges_by_key.Find(id);
    if (entry == table->ranges_by_key.End())
        return false;
    if (!table->blocks[entry->block].field_0x38)
        return false;
    *out = entry->index + 15;
    return true;
}
