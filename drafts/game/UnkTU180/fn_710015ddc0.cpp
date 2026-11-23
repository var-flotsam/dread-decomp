// UnkTU180: looks up the uniform range a name selects in one of three tables (the name is copied into
// a function-local scratch string whose hash is the key) and writes its handle (an index plus 15) to
// `out`; false when absent or its block is empty. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/UnkCloneCalls.h"
#include "base/global/CRntString.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

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

// A CRntString built by its out-of-line default constructor.
struct UnkScratchString {
    alignas(8) u8 storage[sizeof(CRntString)];
    CRntString* Get() { return reinterpret_cast<CRntString*>(storage); }
    UnkScratchString() { fn_710007e6c4(storage); }
    ~UnkScratchString() { CRntString_Destroy(Get()); }
};

bool fn_710015ddc0(UnkUniformTables* self, u32 kind, u64* out, const char* name) {
    *out = 0;
    if (!self || !name)
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
    static UnkScratchString s_name;
    CRntString_AssignCString(s_name.Get(), name);
    UnkUniformRangeEntry* entry = table->ranges.Find(s_name.Get()->key);
    if (entry == table->ranges.End())
        return false;
    if (!table->blocks[entry->block].field_0x38)
        return false;
    *out = entry->index + 15;
    return true;
}
