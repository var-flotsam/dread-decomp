// Unit 2742 (no reliable name): writes a 12-byte value to the in-memory file under a key: over the
// value written before under that key, or at the end, recording where it starts in a table of entries
// (kind 0x76). Names and layouts are placeholders.
#include <algorithm>

#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/UnkHashStream.h"
#include "../../../src/game_s7/UnkIter.h"
#include "types.h"

// Where a keyed value starts in the file, and what kind it is.
struct UnkValueEntry_l1af984 {
    u64 offset;
    u64 key;
    u32 kind;

    UnkValueEntry_l1af984(u64 offset, u64 key, u32 kind) : offset(offset), key(key), kind(kind) {}
    UnkValueEntry_l1af984(UnkValueEntry_l1af984&& other)
        : offset(other.offset), key(other.key), kind(other.kind) {}
};

struct UnkValueWriter_l1af984 {
    base::global::CRntVector<UnkValueEntry_l1af984> entries;
    base::global::CRntFile file;  // +0x28

    UnkValueEntry_l1af984* End() { return &entries.data[entries.count]; }

    // The entry with the key, or End().
    UnkValueEntry_l1af984* Find(const u64& key) {
        UnkIter<UnkValueEntry_l1af984> first = {entries.data};
        UnkIter<UnkValueEntry_l1af984> last = {End()};
        return std::find_if(first, last, [&key](const UnkValueEntry_l1af984& e) { return e.key == key; }).p;
    }
};

void fn_71011af984(UnkValueWriter_l1af984* self, const u64* key, const void* value) {
    UnkValueEntry_l1af984* entry = self->Find(*key);
    u64 offset = fn_710007c1a4(&self->file);
    if (entry != self->End())
        offset = entry->offset;
    else
        self->entries.EmplaceBack(UnkValueEntry_l1af984(offset, *key, 0x76));
    CRntFile_Seek(&self->file, offset, 0);
    CRntFile_WriteElements(&self->file, value, 0xc, 1);
}
