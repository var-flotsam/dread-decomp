// Unit 99 (base::snd::CSoundSystemATK's tunable sound system): takes a reference on a sound group,
// loading "rom:/packs/sounds/<name>.bfgrp" into the engine heap the first time, registering its id and
// making it the owner of the entries its keys name that have none. Names and layouts are placeholders.
#include <nn/fs.h>
#include <nn/os.h>

#include "base/UnkAtkHeap.h"
#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU57.h"
#include "game/UnkTU58.h"
#include "game/UnkTU99.h"
#include "types.h"

using base::global::CRntString;

extern u8 g_unk_7108b6b3c0[] __attribute__((visibility("hidden")));
// nn::atk (library): adds a loaded block of `size` bytes.
void fn_71012091dc(void* self, void* data, u64 size);

namespace {

inline void* Allocate(UnkAtkHeap* heap, u64 size, u64 align) {
    heap->last = fn_71000813e4(&heap->allocator, size, align, 0xb);
    if (heap->hook)
        heap->hook(heap->hook_arg, heap);
    return heap->last;
}

// A sound group's id.
struct UnkSoundId_l100e74 {
    u32 low;
    u32 high;

    bool IsSet() const { return ((u64)high << 32 | low) != 0; }
};

struct UnkTarget_l100e74 {
    u8 pad_0x0[0x18];
    UnkSoundId_l100e74 owner;
};

struct UnkMapNode_l100e74 {
    u64 key;
    UnkTarget_l100e74* value;
    u8 pad_0x10[0x10];
    UnkMapNode_l100e74* next;
};

struct UnkMap_l100e74 {
    u8 pad_0x0[0x10];
    UnkMapNode_l100e74** buckets;
    u32 count;
    u32 bucketCount;

    // The node of a key that is in the map.
    UnkMapNode_l100e74* Get(u64 key) {
        UnkMapNode_l100e74* node = buckets[(u32)key % bucketCount];
        while (node->key != key)
            node = node->next;
        return node;
    }
};

struct UnkEntry_l100e74 {
    UnkSoundId_l100e74 id;
    u8 pad_0x8[0x4];
    u32 size;
    u32 refs;
    u32 field_0x14;
    char name[0x80];
    u32 key_count;
    u64* keys;
    void* data;
};
static_assert(__builtin_offsetof(UnkEntry_l100e74, key_count) == 0x98, "");

struct UnkIdList_l100e74 {
    base::global::CRntVector<UnkSoundId_l100e74> ids;
    u8 pad_0x28[0x10];
    UnkMap_l100e74* map;
};

struct UnkSystem_l100e74 {
    u8 pad_0x0[0x400840];
    UnkIdList_l100e74 list;
};

}  // namespace

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
bool fn_7100100e74(void* self, void* object) {
    nn::os::GetCurrentThread();
    UnkEntry_l100e74* entry = static_cast<UnkEntry_l100e74*>(object);
    if (entry->data) {
        entry->refs++;
        return true;
    }
    UnkIdList_l100e74& list = static_cast<UnkSystem_l100e74*>(self)->list;
    nn::fs::FileHandle file;
    const char* name = entry->name;
    CRntString path("rom:/packs/sounds/");
    CRntString_AppendCString(&path, name);
    CRntString_AppendCString(&path, ".bfgrp");

    nn::fs::OpenFile(&file, path.data ? path.data : g_unk_710162e2ed, nn::fs::OpenMode_Read);
    s64 size = 0;
    nn::fs::GetFileSize(&size, file);
    entry->size = size;
    entry->data = Allocate(&g_unk_7108b690a8, size, 0x40);
    u64 read = 0;
    nn::fs::ReadFile(&read, file, 0, entry->data, size);
    nn::fs::CloseFile(file);
    fn_71012091dc(g_unk_7108b6b3c0, entry->data, size);

    entry->refs = 1;
    entry->field_0x14 = 0;
    list.ids.PushBack(entry->id);
    for (u32 i = 0; i < entry->key_count; ++i) {
        UnkTarget_l100e74* target = list.map->Get(entry->keys[i])->value;
        if (!target->owner.IsSet())
            target->owner = entry->id;
    }
    return true;
}
