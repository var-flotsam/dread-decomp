// base::snd::CSoundSystemATK (unit 99): releases one reference on a sound entry; when the last
// reference goes (or when forced), the entry's resource loses a reference too and the entry leaves
// the active list (swap with the last). Names and offsets are placeholders.
#include <nn/os.h>

#include "types.h"

struct UnkSoundResource {
    u8 pad_0x0[0x14];
    u32 refs;  // 0x14
};

struct UnkSoundEntry {
    u64 handle;  // 0x0
    u8 pad_0x8[0xc];
    u32 refs;  // 0x14
    u64 key;   // 0x18
};

struct UnkSoundMapNode {
    u64 key;                     // 0x0
    UnkSoundResource* resource;  // 0x8
    u8 pad_0x10[0x10];
    UnkSoundMapNode* next;  // 0x20
};

struct UnkSoundMap {
    u8 pad_0x0[0x10];
    UnkSoundMapNode** buckets;  // 0x10
    u32 count;                  // 0x18
    u32 bucket_count;           // 0x1c
};

struct UnkSoundActiveList {
    u64* data;  // 0x0
    u32 count;  // 0x8
    u8 pad_0xc[0x44];
    UnkSoundMap* map;  // 0x50
};

void fn_71001033d4(u8* self, UnkSoundEntry* entry, bool force) {
    nn::os::GetCurrentThread();
    if (!entry)
        return;
    u32 refs = entry->refs;
    if (refs == 0) {
        entry->refs = 0;
        return;
    }
    entry->refs = refs - 1;
    if (refs != 1 && !force)
        return;
    UnkSoundActiveList* list = reinterpret_cast<UnkSoundActiveList*>(self + 0x400818);
    UnkSoundMap* map = list->map;
    if (map->count == 0)
        return;
    u64 key = entry->key;
    u32 index = (u32)key % map->bucket_count;
    UnkSoundMapNode** bucket = &map->buckets[index];
    if (!bucket)
        return;
    UnkSoundMapNode* first = *bucket;
    if (!first)
        return;
    UnkSoundMapNode* node = first;
    while (node->key != key) {
        node = node->next;
        if (!node)
            return;
    }
    node = first;
    while (node->key != key)
        node = node->next;
    UnkSoundResource* res = node->resource;
    if (res->refs)
        res->refs--;
    entry->refs = 0;
    u32 n = list->count;
    if (n == 0)
        return;
    u64* data = list->data;
    for (u64 i = 0; i != n; ++i) {
        if (data[i] == entry->handle) {
            u32 last = n - 1;
            if (last > (u32)i)
                data[i] = data[last];
            list->count = last;
            return;
        }
    }
}
