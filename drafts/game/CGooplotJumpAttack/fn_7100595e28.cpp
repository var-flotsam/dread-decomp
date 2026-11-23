// Unit 848 (CGooplotJumpAttack): records a float under a key once: the value is written to the records'
// file and its offset, the key and the type tag 'f' are appended to the list. Names and layouts are placeholders.
#include <algorithm>
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "../../../src/game_s7/UnkIter.h"
#include "types.h"

struct UnkGooplotRecord_l595e28 {
    u64 offset;  // 0x0: where the value starts in the file
    u64 key;     // 0x8
    u32 type;    // 0x10

    UnkGooplotRecord_l595e28(u64 offset, u64 key, u32 type) : offset(offset), key(key), type(type) {}
    UnkGooplotRecord_l595e28(UnkGooplotRecord_l595e28&& other)
        : offset(other.offset), key(other.key), type(other.type) {}
};
static_assert(sizeof(UnkGooplotRecord_l595e28) == 0x18, "layout");

struct UnkGooplotRecords_ovb0123 {
    base::global::CRntVector<UnkGooplotRecord_l595e28> records;  // 0x0
    base::global::CRntFile file;                                 // 0x28
};

void fn_7100595e28(UnkGooplotRecords_ovb0123* self, const u64* key, f32 value) {
    UnkIter<UnkGooplotRecord_l595e28> end{self->records.data + self->records.count};
    UnkIter<UnkGooplotRecord_l595e28> begin{self->records.data};
    u64 id = *key;
    UnkIter<UnkGooplotRecord_l595e28> found =
        std::find_if(begin, end, [id](const UnkGooplotRecord_l595e28& r) { return r.key == id; });
    if (found == end) {
        u64 offset = fn_710007c1a4(&self->file);
        CRntFile_WriteElements(&self->file, &value, sizeof(value), 1);
        self->records.EmplaceBack(UnkGooplotRecord_l595e28(offset, *key, 'f'));
    }
}
