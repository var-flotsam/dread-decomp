// Unit 99 (base::snd::CSoundSystemATK's tunable sound system): drops a reference on a registered entry.
// The last reference releases its data from the engine heap, unregisters its id and clears the
// entries its keys name; returns false while references remain. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU58.h"
#include "game/UnkTU99.h"
#include "types.h"

extern UnkAtkHeap g_unk_7108b690a8;
extern u8 g_unk_7108b6b3c0[] __attribute__((visibility("hidden")));
// nn::atk (library): releases a block of `size` bytes.
void fn_7101209274(void* self, void* data, u32 size);

namespace {

// The engine heap's free side: the hook at +0x60 (argument +0x68) runs before a block is freed.
struct UnkAtkHeap_l1010e8 {
    u64 field_0x0;
    base::UnkSubAllocator allocator;  // 0x8
    u8 pad_0x40[0x20];
    void (*free_hook)(void* arg, UnkAtkHeap_l1010e8* heap);
    void* free_hook_arg;
};
static_assert(__builtin_offsetof(UnkAtkHeap_l1010e8, free_hook) == 0x60, "");

inline void Free(void* ptr) {
    UnkAtkHeap_l1010e8* heap = reinterpret_cast<UnkAtkHeap_l1010e8*>(&g_unk_7108b690a8);
    if (heap->free_hook)
        heap->free_hook(heap->free_hook_arg, heap);
    fn_7100081420(&heap->allocator, ptr);
}

struct UnkTarget_l1010e8 {
    u8 pad_0x0[0x18];
    void* field_0x18;
};

struct UnkMapNode_l1010e8 {
    u64 key;
    UnkTarget_l1010e8* value;
    u8 pad_0x10[0x10];
    UnkMapNode_l1010e8* next;
};

struct UnkMap_l1010e8 {
    u8 pad_0x0[0x10];
    UnkMapNode_l1010e8** buckets;
    u32 count;
    u32 bucketCount;

    // The node of a key that is in the map.
    UnkMapNode_l1010e8* Get(u64 key) {
        UnkMapNode_l1010e8* node = buckets[(u32)key % bucketCount];
        while (node->key != key)
            node = node->next;
        return node;
    }
};

struct UnkEntry_l1010e8 {
    u64 id;
    u8 pad_0x8[0x4];
    u32 size;
    u32 refs;
    u32 field_0x14;
    u8 pad_0x18[0x80];
    u32 key_count;
    u64* keys;
    void* data;
};
static_assert(__builtin_offsetof(UnkEntry_l1010e8, key_count) == 0x98, "");

struct UnkIdList_l1010e8 {
    base::global::CRntVector<u64> ids;
    u8 pad_0x28[0x10];
    UnkMap_l1010e8* map;

    bool Contains(u64 id) {
        for (u32 i = 0; i < ids.count; ++i)
            if (ids.data[i] == id)
                return true;
        return false;
    }

    // Removes the id, moving the last one into its place.
    void Remove(const u64& id) {
        u32 n = ids.count;
        for (u32 i = 0; i < n; ++i) {
            if (ids.data[i] == id) {
                u32 last = n - 1;
                if (last > i)
                    ids.data[i] = ids.data[last];
                ids.count = last;
                return;
            }
        }
    }
};

struct UnkSystem_l1010e8 {
    u8 pad_0x0[0x400840];
    UnkIdList_l1010e8 list;
};

}  // namespace

bool fn_71001010e8(void* self, void* object) {
    nn::os::GetCurrentThread();
    UnkEntry_l1010e8* entry = static_cast<UnkEntry_l1010e8*>(object);
    UnkIdList_l1010e8& list = static_cast<UnkSystem_l1010e8*>(self)->list;
    u64 id = entry->id;
    if (!list.Contains(id))
        return true;
    if (entry->refs == 0) {
        if (void* data = entry->data) {
            Free(data);
            entry->data = nullptr;
        }
        list.Remove(id);
        entry->refs = 0;
        return true;
    }
    if (--entry->refs != 0)
        return false;
    entry->field_0x14 = 0;
    fn_7101209274(g_unk_7108b6b3c0, entry->data, entry->size);
    Free(entry->data);
    entry->data = nullptr;
    list.Remove(entry->id);
    for (u32 i = 0; i < entry->key_count; ++i) {
        u64 key = entry->keys[i];
        list.map->Get(key)->value->field_0x18 = nullptr;
    }
    return true;
}
