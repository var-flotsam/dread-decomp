// Unit 2742 (no reliable name): writes a u64 value into a keyed in-memory file: the value goes at the
// offset recorded for its key, or at the end of what is written so far, recording the key there with
// type tag 'I'. Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntVector;

// Where a keyed value was written, and its type tag.
struct UnkKeyedSlot_l1aeaf8 {
    u64 offset;
    u64 key;  // 0x8
    u32 tag;  // 0x10

    UnkKeyedSlot_l1aeaf8(u64 offset, u64 key, u32 tag) : offset(offset), key(key), tag(tag) {}
    UnkKeyedSlot_l1aeaf8(UnkKeyedSlot_l1aeaf8&& o) : offset(o.offset), key(o.key), tag(o.tag) {}
};

struct UnkKeyedFile_l1aeaf8 {
    CRntVector<UnkKeyedSlot_l1aeaf8> slots;
    CRntFile file;  // 0x28

    // The slot recorded for `key`, or the end of the slots.
    UnkKeyedSlot_l1aeaf8* Find(const u64* key) {
        UnkKeyedSlot_l1aeaf8* end = slots.data + slots.count;
        for (UnkKeyedSlot_l1aeaf8* it = slots.data; it != end; ++it)
            if (it->key == *key)
                return it;
        return end;
    }
};
static_assert(__builtin_offsetof(UnkKeyedFile_l1aeaf8, file) == 0x28, "layout");

void fn_71011aeaf8(UnkKeyedFile_l1aeaf8* self, const u64* key, u64 value) {
    UnkKeyedSlot_l1aeaf8* it = self->Find(key);
    u64 offset = fn_710007c1a4(&self->file);
    if (it != self->slots.data + self->slots.count)
        offset = it->offset;
    else
        self->slots.EmplaceBack(UnkKeyedSlot_l1aeaf8(offset, *key, 'I'));
    fn_710007c00c(&self->file, offset, 0);
    fn_710007c450(&self->file, &value, sizeof(value), 1);
}
