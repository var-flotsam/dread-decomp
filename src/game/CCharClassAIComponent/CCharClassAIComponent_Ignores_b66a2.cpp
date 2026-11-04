// CCharClassAIComponent unit (tools/tu_map.py, the name from reflection): true when the flag at +0x3b0 is
// set, or the first id is in the list at +0x3b8 (count +0x3c0), or the second in the list at +0x3e0
// (count +0x3e8). Names and layouts are placeholders.
#include "types.h"

struct UnkIdList_b66a2 {
    u64* data;
    u32 count;
};

static inline __attribute__((always_inline)) bool UnkIdListContains_b66a2(const UnkIdList_b66a2& list, const u64& id) {
    for (u32 i = 0; i < list.count; i++) {
        if (list.data[i] == id) {
            return true;
        }
    }
    return false;
}

struct UnkCharClassAIIgnores_b66a2 {
    u8 pad_0x0[0x3b0];
    bool all;
    u8 pad_0x3b1[0x7];
    UnkIdList_b66a2 first;
    u8 pad_0x3c8[0x18];
    UnkIdList_b66a2 second;
};

bool fn_7100414b64(UnkCharClassAIIgnores_b66a2* self, const u64* first, const u64* second) {
    if (self->all) {
        return true;
    }
    if (UnkIdListContains_b66a2(self->first, *first)) {
        return true;
    }
    if (UnkIdListContains_b66a2(self->second, *second)) {
        return true;
    }
    return false;
}
