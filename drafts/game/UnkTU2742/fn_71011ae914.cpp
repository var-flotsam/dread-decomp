// Unit 2742 (no reliable name): writes an s32 value into a keyed in-memory file: the value goes at the
// offset recorded for its key, or at the end of what is written so far, recording the key there with
// type tag 'i'. Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include <algorithm>

#include "base/global/CRntVector.h"
#include "../../game_s7/UnkIter.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntVector;

// Where a keyed value was written, and its type tag.
struct UnkKeyedSlot_l1ae914 {
    u64 offset;
    u64 key;  // 0x8
    u32 tag;  // 0x10

    UnkKeyedSlot_l1ae914(u64 offset, u64 key, u32 tag) : offset(offset), key(key), tag(tag) {}
    UnkKeyedSlot_l1ae914(UnkKeyedSlot_l1ae914&& o) : offset(o.offset), key(o.key), tag(o.tag) {}
};

struct UnkKeyedFile_l1ae914 {
    CRntVector<UnkKeyedSlot_l1ae914> slots;
    CRntFile file;  // 0x28
};
static_assert(__builtin_offsetof(UnkKeyedFile_l1ae914, file) == 0x28, "layout");

void fn_71011ae914(UnkKeyedFile_l1ae914* self, const u64* key, s32 value) {
    UnkIter<UnkKeyedSlot_l1ae914> begin{self->slots.data};
    UnkIter<UnkKeyedSlot_l1ae914> end{self->slots.data + self->slots.count};
    UnkKeyedSlot_l1ae914* it =
        std::find_if(begin, end, [&](const UnkKeyedSlot_l1ae914& slot) { return slot.key == *key; }).p;
    u64 offset = fn_710007c1a4(&self->file);
    if (it != &self->slots.data[self->slots.count])
        offset = it->offset;
    else
        self->slots.EmplaceBack(UnkKeyedSlot_l1ae914(offset, *key, 'i'));
    fn_710007c00c(&self->file, offset, 0);
    fn_710007c450(&self->file, &value, sizeof(value), 1);
}
