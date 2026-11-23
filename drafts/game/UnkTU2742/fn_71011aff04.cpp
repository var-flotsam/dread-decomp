// Unit 2742 (no class hint): records a 16-byte value under a key once, writing it to the table's file and
// appending (offset in the file, key, kind 0x77) to its entry list. Names and layouts are placeholders.
#include <algorithm>
#include <new>

#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/UnkHashStream.h"
#include "types.h"
#include "../../../src/game_s7/UnkIter.h"

using base::global::CRntFile;
using base::global::CRntVector;

struct UnkTU2742Entry {
    u64 offset;
    u64 key;
    u32 kind;

    UnkTU2742Entry(u64 offset_, u64 key_, u32 kind_) : offset(offset_), key(key_), kind(kind_) {}
    UnkTU2742Entry(UnkTU2742Entry&& other) : offset(other.offset), key(other.key), kind(other.kind) {}
};

struct UnkTU2742Table {
    CRntVector<UnkTU2742Entry> entries;  // 0x0
    CRntFile file;                       // 0x28
};

void fn_71011aff04(UnkTU2742Table* self, const u64* key, const void* value) {
    UnkTU2742Entry* end = self->entries.end();
    UnkTU2742Entry* found = std::find_if(UnkIter<UnkTU2742Entry>{self->entries.begin()}, UnkIter<UnkTU2742Entry>{end},
                                         [key](const UnkTU2742Entry& e) { return e.key == *key; }).p;
    if (found != end)
        return;
    u64 offset = fn_710007c1a4(&self->file);
    fn_710007c450(&self->file, value, 0x10, 1);
    self->entries.EmplaceBack(UnkTU2742Entry(offset, *key, 0x77));
}
