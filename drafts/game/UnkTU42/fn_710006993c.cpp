// Unit 42: finds the first entry whose key is not below a value in a sorted key table and returns the matching value slot.
// Names and layouts are placeholders.
#include "types.h"
#include <algorithm>

struct UnkKeyedTable {
    u64* values;
    float* keys;
    u32 count;

    float* KeysEnd() { return keys + count; }
};

u64* fn_710006993c(UnkKeyedTable* table, f32 key) {
    if (table->count == 0) {
        return table->values;
    }
    float* it = std::lower_bound(table->keys, table->KeysEnd(), key);
    u32 index = it - table->keys;
    return table->values + index;
}
