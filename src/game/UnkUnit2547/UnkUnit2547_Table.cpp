// From unit 2547 of tools/tu_map.py (no reflected class): picks an entry of a static table of 8-byte entries:
// one of twelve from the row at +0x20 or, with the flag, the row at +0x80; out-of-range kinds get the entry at
// +0x18. Names and layouts are placeholders.
#include "types.h"

struct UnkUnit2547Table {
    u64 head[3];
    u64 fallback;  // 0x18
    u64 normal[12];  // 0x20
    u64 alternate[12];  // 0x80
};

extern UnkUnit2547Table g_unk_71727be208 __attribute__((visibility("hidden")));

const u64* fn_7100ddb6b0(u32 kind, bool alternate) {
    const UnkUnit2547Table* table = &g_unk_71727be208;
    const u64* entry = alternate ? &table->alternate[kind] : &table->normal[kind];
    return kind > 11 ? &table->fallback : entry;
}
