// Unit 42: finds the first entry whose key is above a value in a sorted key table and returns the matching value slot.
// Names and layouts are placeholders.
#include "types.h"
#include <algorithm>

struct UnkKeyedTable {
    u64* values;
    float* keys;
    u32 count;
};

u64* fn_71000699a0(UnkKeyedTable* table, f32 key) {
    if (table->count == 0) {
        return table->values;
    }
    float* first = table->keys;
    float* it = std::upper_bound(first, first + table->count, key);
    u32 index = it - first;
    return table->values + index;
}
