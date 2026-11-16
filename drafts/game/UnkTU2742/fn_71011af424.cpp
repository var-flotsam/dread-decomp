// Unit 2742 (no reliable name): writes a 64-bit value for a key into the writer's stream once, recording
// where it went (the record's kind is 0x55); a key already recorded is skipped. Names and layouts are
// placeholders.
#include <algorithm>

#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
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

    UnkValueRecord_2742* begin() { return records.data; }
    UnkValueRecord_2742* end() { return records.data + records.count; }

    UnkValueRecord_2742* Find(const u64& key) {
        for (UnkValueRecord_2742& r : *this)
            if (r.key == key)
                return &r;
        return end();
    }
};

void fn_71011af424(UnkValueWriter_2742* self, const u64* key, u64 value) {
    if (self->Find(*key) != self->end())
        return;

    u64 offset = fn_710007c1a4(&self->stream);
    fn_710007c450(&self->stream, &value, sizeof(value), 1);
    self->records.EmplaceBack(UnkValueRecord_2742(offset, *key, 0x55));
}
