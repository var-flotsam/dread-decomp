// Unit 2742 (no reliable name): writes a block of bytes to the in-memory file once per key, recording
// where it starts in a table of entries (kind 0x63). Names and layouts are placeholders.
#include <algorithm>

#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/UnkHashStream.h"
#include "../../game_s7/UnkIter.h"
#include "types.h"

// Where a keyed block starts in the file, and what kind it is.
struct UnkBlobEntry_l1ae740 {
    u64 offset;
    u64 key;
    u32 kind;

    UnkBlobEntry_l1ae740(u64 offset, u64 key, u32 kind) : offset(offset), key(key), kind(kind) {}
    UnkBlobEntry_l1ae740(UnkBlobEntry_l1ae740&& other)
        : offset(other.offset), key(other.key), kind(other.kind) {}
};

// A size-prefixed block of bytes.
struct UnkBlob_l1ae740 {
    u32 size;
    const void* data;
};

struct UnkBlobWriter_l1ae740 {
    base::global::CRntVector<UnkBlobEntry_l1ae740> entries;
    base::global::CRntFile file;  // +0x28
};

// The entry with the key, or the end of the table.
inline UnkBlobEntry_l1ae740* FindEntry(UnkBlobEntry_l1ae740* begin, UnkBlobEntry_l1ae740* end, const u64& key) {
    UnkIter<UnkBlobEntry_l1ae740> first = {begin};
    UnkIter<UnkBlobEntry_l1ae740> last = {end};
    return std::find_if(first, last, [&key](const UnkBlobEntry_l1ae740& e) { return e.key == key; }).p;
}

void fn_71011ae740(UnkBlobWriter_l1ae740* self, const u64* key, const UnkBlob_l1ae740* blob) {
    UnkBlobEntry_l1ae740* end = &self->entries.data[self->entries.count];
    if (FindEntry(self->entries.data, end, *key) != end)
        return;

    u64 offset = fn_710007c1a4(&self->file);
    CRntFile_WriteElements(&self->file, blob, 1, 4);
    CRntFile_WriteElements(&self->file, blob->data, blob->size, 1);
    self->entries.EmplaceBack(UnkBlobEntry_l1ae740(offset, *key, 0x63));
}
