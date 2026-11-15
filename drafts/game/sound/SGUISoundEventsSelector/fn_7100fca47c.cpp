// sound::SGUISoundEventsSelector's unit: clears a sound list, dropping its counted resource and
// stopping the sounds its entries still hold. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "game/UnkTU101.h"
#include "types.h"

void fn_7100071e9c(char* p0);

struct UnkSoundSlot_lfca47c {
    void* object;
    u64 id;
};

struct UnkSoundHandle_lfca47c {
    UnkSoundSlot_lfca47c* slot;
    u64 id;

    bool Alive() const { return slot && id != ~0ull && slot->id == id && slot->object; }
};

struct UnkSoundEntry_lfca47c {
    UnkSoundHandle_lfca47c first;
    UnkSoundHandle_lfca47c second;
};

struct UnkSoundList_lfca47c {
    u8 pad_0x0[0x10];
    UnkSoundEntry_lfca47c* entries;
    u8 pad_0x18[8];
    u32 count;
    u8 pad_0x24[0x14];
    UnkCountedResource* resource;
};

void fn_7100fca47c(UnkSoundList_lfca47c* self) {
    if (UnkCountedResource* resource = self->resource) {
        if (--resource->ref_count == 0)
            fn_710001737c(resource);
        self->resource = nullptr;
    }
    for (UnkSoundEntry_lfca47c *it = self->entries, *end = it + self->count; it != end; ++it) {
        UnkSoundHandle_lfca47c second = it->second;
        if (it->first.Alive())
            fn_7100105034(it->first.slot->object);
        if (second.Alive())
            fn_7100071e9c(static_cast<char*>(second.slot->object));
    }
    self->count = 0;
}
