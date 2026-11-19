// CDemolitionBlockSonarTargetComponent unit: appends the entries (0x20 bytes each, array at +0x48, count at
// +0x50) that pass a test to a list holding at most 0x40 pointers. Names and layouts are placeholders.
#include "types.h"

struct UnkSonarEntry {
    u8 pad_0x0[0x20];
};
bool fn_7100a0f37c(UnkSonarEntry* entry);

struct __attribute__((packed)) UnkSonarEntryList {
    u32 count;
    UnkSonarEntry* items[0x40];
};

struct UnkSonarEntryOwner {
    u8 pad_0x0[0x48];
    UnkSonarEntry* entries;
    u32 count;
};

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_7100a0fcc4(UnkSonarEntryOwner* self, u64 unused1, u64 unused2, UnkSonarEntryList* out) {
    for (UnkSonarEntry *it = self->entries, *end = it + self->count; it != end; ++it) {
        if (fn_7100a0f37c(it))
            out->items[out->count++] = it;
        if (out->count == 0x40)
            break;
    }
}
