// Unit 180's name lookups (tools/tu_map.py unit 180): each copies the name it is given into a
// function-local scratch string and searches a table for it.
//
// The three functions are in one file, in this order, because their statics are laid out together:
// the compiler merges a unit's internal globals into one block sorted by size, so the three guards
// come first and the three strings after them, each in the order the functions appear here. Built
// apart, each string would sit right after its own guard. fn_710015e600 belongs to this unit for the
// same reason (its guard lies between fn_710015dc04's and the first string), although tu_map.py
// draws the boundary to unit 181 before it.
//
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "game/UnkTU180.h"
#include "game/UnkTU57.h"
#include "game/UnkUniformTables.h"
#include "types.h"

using base::global::CRntString;

// A CRntString built by its out-of-line default constructor.
struct UnkScratchString {
    alignas(8) u8 storage[sizeof(CRntString)];
    CRntString* Get() { return reinterpret_cast<CRntString*>(storage); }
    UnkScratchString() { fn_710007e6c4(storage); }
    ~UnkScratchString() { CRntString_Destroy(Get()); }
};

// Looks up the uniform range `name` selects in one of three tables (the scratch string's hash is the
// key) and writes its handle (an index plus 15) to `out`; false when absent or its block is empty.
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
    UnkUniformRangeEntry* entry = table->ranges_by_key.Find(s_name.Get()->key);
    if (entry == table->ranges_by_key.End())
        return false;
    if (!table->blocks[entry->block].field_0x38)
        return false;
    *out = entry->index + 15;
    return true;
}

// Looks a uniform block up by name: compares the name with each block's and writes the block's index
// plus 0xffff to `outIndex`.
bool fn_710015dc04(UnkUniformTable* table, const void* name, u32* outIndex) {
    static UnkScratchString s_name;
    CRntString_AssignCString(s_name.Get(), static_cast<const char*>(name));
    for (u64 i = 0; i < table->block_count; ++i) {
        if (fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&table->blocks[i]),
                          reinterpret_cast<const UnkCompared_0x30*>(s_name.Get()))) {
            *outIndex = i + 0xffff;
            return true;
        }
    }
    return false;
}

// One of the named values of fn_710015e600's table: the name, then the value found by it.
struct UnkNamedValue_0x38 {
    CRntString name;
    u32 value;
};
static_assert(sizeof(UnkNamedValue_0x38) == 0x38);

struct UnkNamedValueTable {
    u8 pad_0x00[0x70];
    u32 count;  // 0x70
    u8 pad_0x74[0x2c];
    UnkNamedValue_0x38* values;  // 0xa0
};
static_assert(__builtin_offsetof(UnkNamedValueTable, values) == 0xa0);

// Looks `name` up among the table's named values and writes the value found to `outValue`.
bool fn_710015e600(UnkNamedValueTable* table, const void* name, u32* outValue) {
    static UnkScratchString s_name;
    CRntString_AssignCString(s_name.Get(), static_cast<const char*>(name));
    for (u64 i = 0; i < table->count; ++i) {
        if (fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&table->values[i].name),
                          reinterpret_cast<const UnkCompared_0x30*>(s_name.Get()))) {
            *outValue = table->values[i].value;
            return true;
        }
    }
    return false;
}
