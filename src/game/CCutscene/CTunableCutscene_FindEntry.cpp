// CCutscene::CTunableCutscene unit: the value of the first entry (0x30 bytes, at +0x150, count at
// +0x158) whose key object's first word equals `key`'s, or null. Names and layouts are placeholders.
#include "types.h"

struct UnkCutsceneEntryKey {
    u64 id;
};
struct UnkCutsceneEntry {
    u8 pad_0x0[0x10];
    UnkCutsceneEntryKey* key;  // 0x10
    void* value;               // 0x18
    u8 pad_0x20[0x10];
};
struct UnkCutsceneEntries {
    u8 pad_0x0[0x150];
    UnkCutsceneEntry* data;  // 0x150
    u32 count;               // 0x158
};

void* fn_710106ca5c(UnkCutsceneEntries* self, const UnkCutsceneEntryKey* key) {
    for (UnkCutsceneEntry *it = self->data, *end = self->data + self->count; it != end; ++it)
        if (it->key->id == key->id)
            return it->value;
    return nullptr;
}
