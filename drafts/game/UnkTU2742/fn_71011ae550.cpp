// Unit 2742 (no reliable name): writes a byte buffer (its size, then its bytes) into a keyed in-memory
// file: it goes at the offset recorded for its key, or at the end of what is written so far, recording
// the key there with type tag 'c'. Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntVector;

// Where a keyed value was written, and its type tag.
struct UnkKeyedSlot_l1ae550 {
    u64 offset;
    u64 key;  // 0x8
    u32 tag;  // 0x10

    UnkKeyedSlot_l1ae550(u64 offset, u64 key, u32 tag) : offset(offset), key(key), tag(tag) {}
    UnkKeyedSlot_l1ae550(UnkKeyedSlot_l1ae550&& o) : offset(o.offset), key(o.key), tag(o.tag) {}
};

struct UnkKeyedFile_l1ae550 {
    CRntVector<UnkKeyedSlot_l1ae550> slots;
    CRntFile file;  // 0x28
};
static_assert(__builtin_offsetof(UnkKeyedFile_l1ae550, file) == 0x28, "layout");

// A sized byte buffer.
struct UnkBytes_l1ae550 {
    u32 size;
    const u8* data;  // 0x8
};

void fn_71011ae550(UnkKeyedFile_l1ae550* self, const u64* key, const UnkBytes_l1ae550* bytes) {
    UnkKeyedSlot_l1ae550* it = self->slots.data;
    for (u32 n = self->slots.count; n != 0; --n, ++it)
        if (it->key == *key)
            break;
    u64 offset = fn_710007c1a4(&self->file);
    if (it != &self->slots.data[self->slots.count])
        offset = it->offset;
    else
        self->slots.EmplaceBack(UnkKeyedSlot_l1ae550(offset, *key, 'c'));
    fn_710007c00c(&self->file, offset, 0);
    fn_710007c3f0(&self->file, &bytes->size, sizeof(bytes->size));
    fn_710007c450(&self->file, bytes->data, 1, bytes->size);
}
