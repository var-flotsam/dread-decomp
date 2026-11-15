// Unit 2661 (CCutscene::CTunableCutscene): the index of the entry (24-byte entries at +0x128) holding a key, or the count when there is none.
// Names and layouts are placeholders.
#include "types.h"

struct UnkKeyHolder2661 {
    u64 key;
};

struct UnkEntry2661 {
    UnkKeyHolder2661* holder;  // 0x0
    u8 pad_0x8[0x10];
};

struct UnkKeyRef2661 {
    UnkKeyHolder2661* holder;  // 0x0
};

struct UnkOwner2661 {
    u8 pad_0x0[0x128];
    UnkEntry2661* entries;  // 0x128
    u32 count;              // 0x130
};

static inline UnkEntry2661* FindEntry(UnkOwner2661* self, u64 key) {
    UnkEntry2661* it = self->entries;
    UnkEntry2661* end = &self->entries[self->count];
    if (self->count != 0 && !(key == it->holder->key)) {
        for (++it; it != end; ++it) {
            if (key == it->holder->key)
                break;
        }
    }
    return it;
}

u32 fn_7101068c80(UnkOwner2661* self, const UnkKeyRef2661* ref) {
    return FindEntry(self, ref->holder->key) - self->entries;
}
