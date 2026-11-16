// Unit 99 (base::snd::CSoundSystemATK's tunable sound system): takes a reference on an entry whose key is
// in the system's map, recording the entry's id the first time it is seen. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/global/CRntVector.h"
#include "types.h"

namespace {

struct UnkRefCounted_l103210 {
    u8 pad_0x0[0x14];
    u32 refs;
};

struct UnkMapNode_l103210 {
    u64 key;
    UnkRefCounted_l103210* value;
    u8 pad_0x10[0x10];
    UnkMapNode_l103210* next;
};

struct UnkMap_l103210 {
    u8 pad_0x0[0x10];
    UnkMapNode_l103210** buckets;
    u32 count;
    u32 bucketCount;

    UnkMapNode_l103210* Find(const u64& key) {
        if (count == 0)
            return nullptr;
        u32 index = (u32)key % bucketCount;
        UnkMapNode_l103210** bucket = &buckets[index];
        if (!bucket)
            return nullptr;
        for (UnkMapNode_l103210* node = *bucket; node; node = node->next)
            if (node->key == key)
                return node;
        return nullptr;
    }
};

struct UnkEntry_l103210 {
    u64 id;
    u8 pad_0x8[0xc];
    u32 refs;
    u64 key;
};

struct UnkIdList_l103210 {
    base::global::CRntVector<u64> ids;
    u8 pad_0x28[0x28];
    UnkMap_l103210* map;

    bool Contains(const u64& id) {
        u64 n = ids.count;
        u64 i = 0;
        for (; i < n; ++i)
            if (ids.data[i] == id)
                break;
        return i != n;
    }
};

struct UnkSystem_l103210 {
    u8 pad_0x0[0x400818];
    UnkIdList_l103210 list;
};

}  // namespace

void fn_7100103210(void* self, void* entry) {
    nn::os::GetCurrentThread();
    if (!entry)
        return;
    UnkIdList_l103210& list = static_cast<UnkSystem_l103210*>(self)->list;
    UnkEntry_l103210* e = static_cast<UnkEntry_l103210*>(entry);
    if (!list.map->Find(e->key))
        return;
    if (!list.Contains(e->id)) {
        list.map->Find(e->key)->value->refs++;
        list.ids.PushBack(e->id);
    }
    e->refs++;
}
