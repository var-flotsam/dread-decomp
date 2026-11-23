// Unit 2666 (lua_bind): records a flag under a key once: the value is written to the records'
// file and its offset, the key and the type tag 'b' are appended to the list. Names and layouts are placeholders.
#include <algorithm>
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "../../../src/game_s7/UnkIter.h"
#include "types.h"

struct UnkRecord_l0971fc {
    u64 offset;  // 0x0: where the value starts in the file
    u64 key;     // 0x8
    u32 type;    // 0x10

    UnkRecord_l0971fc(u64 offset, u64 key, u32 type) : offset(offset), key(key), type(type) {}
    ~UnkRecord_l0971fc() {}
};
static_assert(sizeof(UnkRecord_l0971fc) == 0x18, "layout");

struct UnkRecords_l0971fc {
    base::global::CRntVector<UnkRecord_l0971fc> records;  // 0x0
    base::global::CRntFile file;                                 // 0x28
};

void fn_71010971fc(UnkRecords_l0971fc* self, const u64* key, bool value) {
    UnkIter<UnkRecord_l0971fc> end{self->records.data + self->records.count};
    UnkIter<UnkRecord_l0971fc> begin{self->records.data};
    u64 id = *key;
    UnkIter<UnkRecord_l0971fc> found =
        std::find_if(begin, end, [id](const UnkRecord_l0971fc& r) { return r.key == id; });
    if (found == end) {
        u64 offset = fn_710007c1a4(&self->file);
        CRntFile_WriteElements(&self->file, &value, sizeof(value), 1);
        self->records.EmplaceBack(UnkRecord_l0971fc(offset, *key, 'b'));
    }
}
