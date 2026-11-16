// Unit 2742 (no reliable name): writes a string into a keyed in-memory file as a 256-byte field (cut
// short or padded with zeros): it goes at the offset recorded for its key, or at the end of what is
// written so far, recording the key there with type tag 's'. Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntVector;

// Where a keyed value was written, and its type tag.
struct UnkKeyedSlot_l1ae328 {
    u64 offset;
    u64 key;  // 0x8
    u32 tag;  // 0x10

    UnkKeyedSlot_l1ae328(u64 offset, u64 key, u32 tag) : offset(offset), key(key), tag(tag) {}
    UnkKeyedSlot_l1ae328(UnkKeyedSlot_l1ae328&& o) : offset(o.offset), key(o.key), tag(o.tag) {}
};

struct UnkKeyedFile_l1ae328 {
    CRntVector<UnkKeyedSlot_l1ae328> slots;
    CRntFile file;  // 0x28
};
static_assert(__builtin_offsetof(UnkKeyedFile_l1ae328, file) == 0x28, "layout");

// The size of a string field.
static constexpr u64 kFieldSize = 0x100;

void fn_71011ae328(UnkKeyedFile_l1ae328* self, const u64* key, const char* text) {
    UnkKeyedSlot_l1ae328* it = self->slots.data;
    for (u32 n = self->slots.count; n != 0; --n, ++it)
        if (it->key == *key)
            break;
    u64 offset = fn_710007c1a4(&self->file);
    if (it != &self->slots.data[self->slots.count])
        offset = it->offset;
    else
        self->slots.EmplaceBack(UnkKeyedSlot_l1ae328(offset, *key, 's'));
    fn_710007c00c(&self->file, offset, 0);
    u64 length = __builtin_strlen(text) + 1;
    u64 written = length < kFieldSize ? length : kFieldSize;
    fn_710007c3f0(&self->file, text, written);
    if (length < kFieldSize)
        fn_710007c4b8(&self->file, 0, kFieldSize - written);
}
