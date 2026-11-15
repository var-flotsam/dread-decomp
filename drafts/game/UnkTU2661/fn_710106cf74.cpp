// Unit 2661 (CCutscene::CTunableCutscene): looks an entry up by key in the array at +0x128 (24-byte entries); a miss returns the last entry.
// Names and layouts are placeholders.
#include "types.h"

struct UnkKeyHolder2661 {
    u64 key;
};

struct UnkEntry2661 {
    UnkKeyHolder2661* holder;  // 0x0
    u8 pad_0x8[0x10];
};

struct UnkOwner2661 {
    u8 pad_0x0[0x128];
    UnkEntry2661* entries;  // 0x128
    u32 count;              // 0x130
};

UnkEntry2661* fn_710106cf74(UnkOwner2661* self, const u64* key) {
    UnkEntry2661* it = self->entries;
    UnkEntry2661* end = &self->entries[self->count];
    if (self->count != 0 && *key != it->holder->key) {
        for (++it; it != end; ++it) {
            if (*key == it->holder->key)
                break;
        }
    }
    UnkEntry2661* last = self->count ? end - 1 : end;
    return end == it ? last : it;
}
