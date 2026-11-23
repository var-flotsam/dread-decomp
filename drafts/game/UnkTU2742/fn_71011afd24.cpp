// Unit 2742 (no reliable name): writes a 16-byte value to the in-memory file under a key: over the
// value written before under that key, or at the end, recording where it starts in a table of entries
// (kind 0x77). Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/UnkHashStream.h"
#include "types.h"

// Where a keyed value starts in the file, and what kind it is.
struct UnkValueEntry_l1afd24 {
    u64 offset;
    u64 key;
    u32 kind;

    UnkValueEntry_l1afd24(u64 offset, u64 key, u32 kind) : offset(offset), key(key), kind(kind) {}
    UnkValueEntry_l1afd24(UnkValueEntry_l1afd24&& other)
        : offset(other.offset), key(other.key), kind(other.kind) {}
};

struct UnkValueWriter_l1afd24 {
    base::global::CRntVector<UnkValueEntry_l1afd24> entries;
    base::global::CRntFile file;  // +0x28
};

// The entry with the key, or the end of the table.
inline UnkValueEntry_l1afd24* FindEntry(UnkValueEntry_l1afd24* begin, UnkValueEntry_l1afd24* end, const u64& key) {
    for (UnkValueEntry_l1afd24* p = begin; p != end; ++p)
        if (p->key == key)
            return p;
    return end;
}

void fn_71011afd24(UnkValueWriter_l1afd24* self, const u64* key, const void* value) {
    UnkValueEntry_l1afd24* entry =
        FindEntry(self->entries.data, &self->entries.data[self->entries.count], *key);
    u64 end_offset = fn_710007c1a4(&self->file);
    u64 offset;
    if (entry != &self->entries.data[self->entries.count]) {
        offset = entry->offset;
    } else {
        offset = end_offset;
        self->entries.EmplaceBack(UnkValueEntry_l1afd24(offset, *key, 0x77));
    }
    CRntFile_Seek(&self->file, offset, 0);
    CRntFile_WriteElements(&self->file, value, 0x10, 1);
}
