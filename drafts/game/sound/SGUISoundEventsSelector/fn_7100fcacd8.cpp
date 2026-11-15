// sound::SGUISoundEventsSelector's unit: removes the entry of a sound list whose first handle
// refers to the given object, moving the last entry into its place. Names and layouts are placeholders.
#include "types.h"

struct UnkSoundSlot_lfcacd8 {
    void* object;
    u64 id;
};

struct UnkSoundHandle_lfcacd8 {
    UnkSoundSlot_lfcacd8* slot;
    u64 id;

    void* Get() const {
        if (!slot)
            return nullptr;
        if (slot->id == id && id != ~0ull)
            return slot->object;
        return nullptr;
    }
};

struct UnkSoundEntry_lfcacd8 {
    UnkSoundHandle_lfcacd8 first;
    UnkSoundHandle_lfcacd8 second;
};

struct UnkSoundList_lfcacd8 {
    u8 pad_0x0[8];
    UnkSoundEntry_lfcacd8* entries;
    u64* keys;
    u32 count;
};

void fn_7100fcacd8(UnkSoundList_lfcacd8* self, void* object) {
    if (!object)
        return;
    u32 count = self->count;
    if (!count)
        return;
    UnkSoundEntry_lfcacd8* entries = self->entries;
    UnkSoundEntry_lfcacd8* found = entries;
    for (u32 i = 0; i < count; ++i) {
        found = &entries[i];
        if (found->first.Get() == object)
            goto matched;
    }
    return;
matched:
    u64 key = self->keys[found - entries];
    for (u64 j = 0; j < count; ++j) {
        if (self->keys[j] == key) {
            if (j == count - 1) {
                self->count = j;
                return;
            }
            entries[j] = entries[count - 1];
            self->keys[j] = self->keys[self->count - 1];
            self->count = self->count - 1;
            return;
        }
    }
}
