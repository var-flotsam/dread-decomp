// Unit 2666 (lua_bind): records an integer under a key once: the value is written to the records'
// file and its offset, the key and the type tag 'i' are appended to the list. Names and layouts are placeholders.
#include <algorithm>
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "../../game_s7/UnkIter.h"
#include "types.h"

struct UnkRecord_l09703c {
    u64 offset;  // 0x0: where the value starts in the file
    u64 key;     // 0x8
    u32 type;    // 0x10

    UnkRecord_l09703c(u64 offset, u64 key, u32 type) : offset(offset), key(key), type(type) {}
    UnkRecord_l09703c(UnkRecord_l09703c&& other)
        : offset(other.offset), key(other.key), type(other.type) {}
};
static_assert(sizeof(UnkRecord_l09703c) == 0x18, "layout");

struct UnkRecords_l09703c {
    base::global::CRntVector<UnkRecord_l09703c> records;  // 0x0
    base::global::CRntFile file;                                 // 0x28
};

void fn_710109703c(UnkRecords_l09703c* self, const u64* key, s32 value) {
    UnkIter<UnkRecord_l09703c> end{self->records.data + self->records.count};
    UnkIter<UnkRecord_l09703c> begin{self->records.data};
    u64 id = *key;
    UnkIter<UnkRecord_l09703c> found =
        std::find_if(begin, end, [id](const UnkRecord_l09703c& r) { return r.key == id; });
    if (found == end) {
        u64 offset = fn_710007c1a4(&self->file);
        fn_710007c450(&self->file, &value, sizeof(value), 1);
        self->records.EmplaceBack(UnkRecord_l09703c(offset, *key, 'i'));
    }
}
