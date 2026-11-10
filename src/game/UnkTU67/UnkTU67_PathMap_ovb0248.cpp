// A unit of tools/tu_map.py (67, no reliable name): the pooled map of file-path ids (a chained map
// over a block pool, nodes keeping a string id and a list) torn down by its reset, and the node
// clear that reset calls. Names and layouts are placeholders.
#include <string.h>

#include "base/UnkBlockFreeList.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkIdList_b0248 {
    CFilePathStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // The id's destructor, with the decrement and the test in two steps.
    static void Release(CFilePathStrId& id) {
        UnkStringPool* pool = g_unk_7101d47200;
        if (pool) {
            if (id.data && id.data != &pool->empty) {
                --id.data->ref_count;
                if (id.data->ref_count == 0) {
                    fn_71000803a4(pool, id.data);
                    pool = g_unk_7101d47200;
                }
            }
            id.data = &pool->empty;
        }
    }
    void Clear() {
        for (u32 i = 0; i < count; ++i)
            Release(data[i]);
        count = 0;
    }
    void Free() {
        if (data) {
            Clear();
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkPathNode_b0248 {
    UnkStringData* key;  // 0x0
    void* storage;       // 0x8: a list's array
    u32 unk_0x10;        // its count
    u32 unk_0x14;        // its capacity
    u8 unk_0x18[0x10];
    base::UnkAllocator* allocator;  // 0x28
    UnkPathNode_b0248* next;        // 0x30
};
static_assert(__builtin_offsetof(UnkPathNode_b0248, next) == 0x30, "layout");

struct UnkPooledPathMap_b0248 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    base::UnkVector<UnkPathNode_b0248*> buckets;  // 0x10 (data, count, capacity, ..., allocator at 0x30)
    UnkPathNode_b0248* tail;  // 0x38
    UnkPathNode_b0248* head;  // 0x40
    void* vtable;                         // 0x48
    base::UnkAllocator* pool_allocator;  // 0x50
    UnkBlockFreeList free_list;          // 0x58
    u8 pad_0x80[0x4];
    bool field_0x84;
    u8 pad_0x85[0x3];
    UnkIdList_b0248 ids;  // 0x88
};
static_assert(__builtin_offsetof(UnkPooledPathMap_b0248, ids) == 0x88, "layout");
static_assert(__builtin_offsetof(UnkPooledPathMap_b0248, free_list) == 0x58, "layout");

void fn_710008c2cc(UnkPooledPathMap_b0248* self);

void fn_710008b924(UnkPooledPathMap_b0248* self) {
    self->ids.Clear();
    fn_710008c2cc(self);
    self->ids.Free();
    self->field_0x84 = true;
    fn_710008c2cc(self);
    self->allocator = nullptr;
    self->vtable = g_unk_7101a1a4c8 + 0x10;
    if (void* buffer = self->free_list.buffer) {
        fn_71000837f4(&self->free_list);
        self->pool_allocator->vf_0x48(buffer);
        self->pool_allocator = nullptr;
    }
    fn_710008c2cc(self);
    self->buckets.Free();
}

// The list of all nodes runs through +0x30.
void fn_710008c2cc(UnkPooledPathMap_b0248* self) {
    if (!self->count)
        return;
    for (UnkPathNode_b0248* n = self->head; n;) {
        UnkPathNode_b0248* next = n->next;
        if (n->storage) {
            n->unk_0x10 = 0;
            n->allocator->vf_0x48(n->storage);
            n->unk_0x14 = 0;
        }
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (n->key && n->key != &pool->empty) {
                --n->key->ref_count;
                if (n->key->ref_count == 0) {
                    fn_71000803a4(pool, n->key);
                    pool = g_unk_7101d47200;
                }
            }
            n->key = &pool->empty;
        }
        self->allocator->vf_0x48(n);
        n = next;
    }
    memset(self->buckets.data, 0, (u64)self->buckets.capacity * 8);
    self->count = 0;
    self->tail = nullptr;
    self->head = nullptr;
}
