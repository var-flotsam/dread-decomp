// The CActionInstance unit (tools/tu_map.py unit 2206, by its reflected class): a lookup of the
// entries filed under a key, first in an override set and then in the defaults (fn_7100c97868).
// Which objects these are is inferred from the code only; names and layouts are placeholders.
#include "types.h"

// An entry filed under a key (+0x8).
struct UnkActionEntry {
    u8 pad_0x0[0x8];
    u64 key;
};

// A vector of entries (pointer and count).
struct UnkActionEntries {
    UnkActionEntry** data;
    u32 count;
};

// The run of consecutive entries with a key (its first entry and length); empty when there is none.
// It is returned in memory, so its copy is not trivial.
struct UnkActionRange {
    UnkActionEntry** first;
    s64 count;

    UnkActionRange() : first(nullptr), count(0) {}
    UnkActionRange(const UnkActionRange& other) : first(other.first), count(other.count) {}
};

// The entries filed under `key` in `entries` (fn_710006b990).
UnkActionRange fn_710006b990(UnkActionEntries* entries, u64 key);

struct UnkActionDefaults {
    u8 pad_0x0[0x170];
    UnkActionEntries entries;
};
struct UnkActionOverrides {
    u8 pad_0x0[0xf8];
    UnkActionEntries entries;
};
struct UnkActionSet {
    u8 pad_0x0[0x10];
    UnkActionDefaults* defaults;
    u8 pad_0x18[0x98];
    UnkActionOverrides* overrides;
};

// The default entries under `key`, or none when the overrides (if any) have entries under it.
UnkActionRange fn_7100c97868(UnkActionSet* self, u64 key) {
    UnkActionRange range = self->overrides ? fn_710006b990(&self->overrides->entries, key) : UnkActionRange();
    if (range.count)
        return UnkActionRange();
    return fn_710006b990(&self->defaults->entries, key);
}
