// tools/tu_map.py unit 42 (see UnkUnit42_KeyValue.cpp): a table of 8-byte entries with a parallel
// sorted float array and a count; the range from an entry to the end. Names and layouts are placeholders.
#include "types.h"

struct UnkSearchTable42_b66a1 {
    u64* entries;  // 0x0
    f32* keys;     // 0x8
    u32 count;     // 0x10
};

struct UnkEntryRange42_b66a1 {
    u64* first;
    s64 count;
    UnkEntryRange42_b66a1(u64* f, s64 n) : first(f), count(n) {}
    ~UnkEntryRange42_b66a1() {}
};

UnkEntryRange42_b66a1 fn_7100069a04(UnkSearchTable42_b66a1* self, u64* from) {
    return UnkEntryRange42_b66a1(from, (self->entries + self->count) - from);
}
