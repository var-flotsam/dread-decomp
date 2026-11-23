// Unit 2742 (no reliable name): writes a u32 value into a keyed in-memory file: the value goes at the
// offset recorded for its key, or at the end of what is written so far, recording the key there with
// type tag 'u'. Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntVector;

// Where a keyed value was written, and its type tag.
struct UnkKeyedSlot_l1aee9c {
    u64 offset;
    u64 key;  // 0x8
    u32 tag;  // 0x10

    UnkKeyedSlot_l1aee9c(u64 offset, u64 key, u32 tag) : offset(offset), key(key), tag(tag) {}
    UnkKeyedSlot_l1aee9c(UnkKeyedSlot_l1aee9c&& o) : offset(o.offset), key(o.key), tag(o.tag) {}
};

struct UnkKeyedFile_l1aee9c {
    CRntVector<UnkKeyedSlot_l1aee9c> slots;
    CRntFile file;  // 0x28
};
static_assert(__builtin_offsetof(UnkKeyedFile_l1aee9c, file) == 0x28, "layout");

void fn_71011aee9c(UnkKeyedFile_l1aee9c* self, const u64* key, u32 value) {
    UnkKeyedSlot_l1aee9c* it = self->slots.data;
    UnkKeyedSlot_l1aee9c* end = self->slots.data + self->slots.count;
    for (; it != end; ++it)
        if (it->key == *key)
            break;
    u64 offset = fn_710007c1a4(&self->file);
    if (it != &self->slots.data[self->slots.count])
        offset = it->offset;
    else
        self->slots.EmplaceBack(UnkKeyedSlot_l1aee9c(offset, *key, 'u'));
    CRntFile_Seek(&self->file, offset, 0);
    CRntFile_WriteElements(&self->file, &value, sizeof(value), 1);
}
