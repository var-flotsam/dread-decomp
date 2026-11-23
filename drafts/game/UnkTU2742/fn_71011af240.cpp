// Unit 2742 (no reliable name): writes an 8-byte value to the in-memory file under a key: over the
// value written before under that key, or at the end, recording where it starts in a table of entries
// (kind 0x55). Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/UnkHashStream.h"
#include "types.h"

// Where a keyed value starts in the file, and what kind it is.
struct UnkValueEntry_l1af240 {
    u64 offset;
    u64 key;
    u32 kind;

    UnkValueEntry_l1af240(u64 offset, u64 key, u32 kind) : offset(offset), key(key), kind(kind) {}
    UnkValueEntry_l1af240(UnkValueEntry_l1af240&& other)
        : offset(other.offset), key(other.key), kind(other.kind) {}
};

struct UnkValueWriter_l1af240 {
    base::global::CRntVector<UnkValueEntry_l1af240> entries;
    base::global::CRntFile file;  // +0x28

    UnkValueEntry_l1af240* End() { return &entries.data[entries.count]; }

    // The entry with the key, or End().
    UnkValueEntry_l1af240* Find(const u64& key) {
        UnkValueEntry_l1af240* p = entries.data;
        for (u64 n = entries.count; n != 0; --n, ++p)
            if (p->key == key)
                return p;
        return p;
    }
};

void fn_71011af240(UnkValueWriter_l1af240* self, const u64* key, u64 value) {
    UnkValueEntry_l1af240* entry = self->Find(*key);
    u64 offset = fn_710007c1a4(&self->file);
    if (entry != self->End())
        offset = entry->offset;
    else
        self->entries.EmplaceBack(UnkValueEntry_l1af240(offset, *key, 0x55));
    CRntFile_Seek(&self->file, offset, 0);
    CRntFile_WriteElements(&self->file, &value, sizeof(value), 1);
}
