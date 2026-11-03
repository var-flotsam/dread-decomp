// CCharClassShotComponent's unit (tools/tu_map.py unit 1507): finds the first entry of the pointer
// array at +0x58 (count at +0x60) whose key (+0x10) equals *key and stores it in *out; false when
// there is none or no out. Names are placeholders.
#include "types.h"

struct UnkShotEntry_b45a2 {
    u8 pad_0x0[0x10];
    u64 key;
};

struct UnkShotComponent_b45a2 {
    u8 pad_0x0[0x58];
    UnkShotEntry_b45a2** entries;
    u32 count;
};

bool fn_71009f8d1c(UnkShotComponent_b45a2* self, const u64* key, UnkShotEntry_b45a2** out) {
    if (!out)
        return false;
    UnkShotEntry_b45a2** it = self->entries;
    UnkShotEntry_b45a2** end = it + self->count;
    u64 k = *key;
    for (; it != end; ++it) {
        UnkShotEntry_b45a2* e = *it;
        if (e && e->key == k) {
            *out = e;
            return true;
        }
    }
    return false;
}
