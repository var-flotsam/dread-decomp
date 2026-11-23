// Unit 2742 (no reliable name): writes a 8-byte value to the in-memory file under a key: over the
// value written before under that key, or at the end, recording where it starts in a table of entries
// (kind 0x79). Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/UnkHashStream.h"
#include "../../../src/game_s7/UnkIter.h"
#include "types.h"

// Where a keyed value starts in the file, and what kind it is.
struct UnkValueEntry_l1af5e4 {
    u64 offset;
    u64 key;
    u32 kind;

    UnkValueEntry_l1af5e4(u64 offset, u64 key, u32 kind) : offset(offset), key(key), kind(kind) {}
    UnkValueEntry_l1af5e4(UnkValueEntry_l1af5e4&& other)
        : offset(other.offset), key(other.key), kind(other.kind) {}
};

struct UnkValueWriter_l1af5e4 {
    base::global::CRntVector<UnkValueEntry_l1af5e4> entries;
    base::global::CRntFile file;  // +0x28

    UnkValueEntry_l1af5e4* End() { return &entries.data[entries.count]; }

    // The entry with the key, or End().
    UnkValueEntry_l1af5e4* Find(const u64& key) {
        UnkIter<UnkValueEntry_l1af5e4> first = {entries.data};
        UnkIter<UnkValueEntry_l1af5e4> last = {End()};
        while (first != last && (*first).key != key)
            ++first;
        return first.p;
    }
};

void fn_71011af5e4(UnkValueWriter_l1af5e4* self, const u64* key, const void* value) {
    u64 offset;
    UnkValueEntry_l1af5e4* entry = self->Find(*key);
    offset = fn_710007c1a4(&self->file);
    if (entry == self->End()) {
        self->entries.EmplaceBack(UnkValueEntry_l1af5e4(offset, *key, 0x79));
    } else {
        offset = entry->offset;
    }
    CRntFile_Seek(&self->file, offset, 0);
    CRntFile_WriteElements(&self->file, value, 8, 1);
}
