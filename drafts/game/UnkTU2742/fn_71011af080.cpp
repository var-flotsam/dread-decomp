// Unit 2742 (no reliable name): writes a 32-bit value for a key into the writer's stream once, recording
// where it went (the record's kind is 0x75); a key already recorded is skipped. Names and layouts are
// placeholders.
#include <algorithm>

#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "../../../src/game_s7/UnkIter.h"
#include "types.h"

// Where a value was written: its offset in the stream, its key and a kind code.
struct UnkValueRecord_2742 {
    u64 offset;
    u64 key;
    u32 kind;

    UnkValueRecord_2742(u64 offset, u64 key, u32 kind) : offset(offset), key(key), kind(kind) {}
    ~UnkValueRecord_2742() {}
};

struct UnkValueWriter_2742 {
    base::global::CRntVector<UnkValueRecord_2742> records;
    base::global::CRntFile stream;  // 0x28

    UnkIter<UnkValueRecord_2742> begin() { return {records.data}; }
    UnkIter<UnkValueRecord_2742> end() { return {records.data + records.count}; }

    UnkIter<UnkValueRecord_2742> Find(const u64& key) {
        UnkIter<UnkValueRecord_2742> last = end();
        for (UnkIter<UnkValueRecord_2742> it = begin(); it != last; ++it)
            if ((*it).key == key)
                return it;
        return last;
    }
};

void fn_71011af080(UnkValueWriter_2742* self, const u64* key, u32 value) {
    if (self->Find(*key) != self->end())
        return;

    u64 offset = fn_710007c1a4(&self->stream);
    fn_710007c450(&self->stream, &value, sizeof(value), 1);
    self->records.EmplaceBack(UnkValueRecord_2742(offset, *key, 0x75));
}
