// Unit 2742 (no reliable name): writes a signed 64-bit value for a key into the writer's stream once,
// recording where it went (the record's kind is 0x49, 'I'); a key already recorded is skipped. Names and
// layouts are placeholders.
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
};

void fn_71011aecdc(UnkValueWriter_2742* self, const u64* key, s64 value) {
    UnkIter<UnkValueRecord_2742> last = self->end();
    UnkIter<UnkValueRecord_2742> it =
        std::find_if(self->begin(), last, [key](const UnkValueRecord_2742& r) { return r.key == *key; });
    if (it == last) {
        u64 offset = fn_710007c1a4(&self->stream);
        CRntFile_WriteElements(&self->stream, &value, sizeof(value), 1);
        self->records.EmplaceBack(UnkValueRecord_2742(offset, *key, 0x49));
    }
}
