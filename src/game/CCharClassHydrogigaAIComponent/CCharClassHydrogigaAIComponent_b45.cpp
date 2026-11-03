// CCharClassHydrogigaAIComponent's unit (tools/tu_map.py; the name from reflection): the first
// entry of the pointer array at +0x528 (count at +0x530) whose key (+8) equals *key, or null.
// Names are placeholders.
#include "types.h"

struct UnkHydrogigaEntry_b45a2 {
    void** vtable;
    u64 key;
};

struct UnkHydrogigaAI_b45a2 {
    u8 pad_0x0[0x528];
    UnkHydrogigaEntry_b45a2** entries;
    s32 count;
};

UnkHydrogigaEntry_b45a2* fn_71005b9ad8(UnkHydrogigaAI_b45a2* self, const u64* key) {
    UnkHydrogigaEntry_b45a2** entries = self->entries;
    u64 k = *key;
    for (s64 i = 0; i < self->count; ++i) {
        UnkHydrogigaEntry_b45a2* e = entries[i];
        if (e && e->key == k)
            return e;
    }
    return nullptr;
}
