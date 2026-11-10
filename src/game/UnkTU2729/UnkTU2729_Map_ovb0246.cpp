// Unit 2729 (no reliable name): a cache of pooled hash maps, its node release and erase. Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// Releases a string id on `*Pool` in two steps (decrement, then reload and test) and leaves the
// pool's empty id, as CStrId::Reset does on the string pool.
template <UnkStringPool** Pool>
static inline void ResetId_b0246(UnkStringData** slot) {
    if (UnkStringPool* pool = *Pool) {
        UnkStringData* data = *slot;
        if (data && data != &pool->empty) {
            --data->ref_count;
            if ((*slot)->ref_count == 0) {
                fn_71000803a4(pool, *slot);
                pool = *Pool;
            }
        }
        *slot = &pool->empty;
    }
}

struct UnkMapNode_b0246 {
    UnkStringData* name;  // 0x0
    UnkStringData* path;  // 0x8
    UnkMapNode_b0246* next;  // 0x10
};

struct UnkMap_b0246;

struct UnkMapIter_b0246 {
    UnkMapNode_b0246* node;  // 0x0
    UnkMapNode_b0246* prev;  // 0x8
    UnkMap_b0246* map;       // 0x10
    s32 bucket;              // 0x18
    u32 version;             // 0x1c
};

struct UnkMap_b0246 {
    u8 pad_0x0[8];
    u32 bucket_count;  // 0x8
    u8 pad_0xc[0x1c];
    u32 first_bucket;  // 0x28
    u8 pad_0x2c[4];
    UnkMapNode_b0246* first;  // 0x30
    base::UnkAllocator* allocator;  // 0x38
    u8 pad_0x40[4];
    u32 count;  // 0x44
    u32 version;  // 0x48
};

UnkMapNode_b0246** fn_7101194aa8(UnkMap_b0246* self, u32 bucket);

void fn_71011957b0(UnkMap_b0246* self, UnkMapNode_b0246* node) {
    if (!node)
        return;
    ResetId_b0246<&g_unk_7101d47200>(&node->path);
    ResetId_b0246<&g_unk_7101d47108>(&node->name);
    self->allocator->vf_0x48(node);
    self->version++;
}

extern char g_unk_7101cb5270[];

void fn_7101192ee8(void* member);
void fn_71011954cc(void* map);
void fn_71011951e8(void* map);

// A hash map of nodes over a block pool (0x90 bytes): the bucket array first, then the pool.
struct UnkPooledMap_b0246 {
    base::UnkVector<u8> buckets;  // 0x0
    u8 pad_0x28[0x10];
    u64 field_0x38;  // 0x38
    u8 pad_0x40[0x10];
    void* pool_vtable;  // 0x50
    base::UnkAllocator* pool_allocator;  // 0x58
    UnkBlockFreeList pool;  // 0x60
    u8 pad_0x88[4];
    bool dirty;  // 0x8c
    u8 pad_0x8d[3];
};
static_assert(sizeof(UnkPooledMap_b0246) == 0x90, "UnkPooledMap_b0246 layout");
static_assert(__builtin_offsetof(UnkPooledMap_b0246, pool) == 0x60, "UnkPooledMap_b0246 layout");

// Frees the pool's buffer through its allocator and the bucket array.
static inline void Release_b0246(UnkPooledMap_b0246* map, void (*clear)(void*)) {
    map->dirty = true;
    clear(map);
    map->field_0x38 = 0;
    void* buffer = map->pool.buffer;
    map->pool_vtable = g_unk_7101a1a4c8 + 0x10;
    if (buffer) {
        fn_71000837f4(&map->pool);
        map->pool_allocator->vf_0x48(buffer);
        map->pool_allocator = nullptr;
    }
    clear(map);
    if (map->buckets.data) {
        map->buckets.count = 0;
        map->buckets.allocator->vf_0x48(map->buckets.data);
        map->buckets.capacity = 0;
    }
}

struct UnkPooledCache_b0246 {
    void* vtable_0x0;
    void* vtable_0x8;
    UnkPooledMap_b0246 first;  // 0x10
    UnkPooledMap_b0246 second;  // 0xa0
};

void fn_7101192dc4(UnkPooledCache_b0246* self) {
    self->vtable_0x0 = g_unk_7101cb5270 + 0x10;
    self->vtable_0x8 = g_unk_7101cb5270 + 0x60;
    fn_7101192ee8(reinterpret_cast<u8*>(self) + 0x130);
    Release_b0246(&self->second, fn_71011954cc);
    Release_b0246(&self->first, fn_71011951e8);
}

void fn_7101195e7c(UnkMap_b0246* self, UnkMapIter_b0246* it) {
    u32 bucket;
    if (it->map && it->map->version == it->version) {
        bucket = it->bucket;
    } else {
        bucket = -1;
        it->prev = nullptr;
        it->map = nullptr;
        it->node = nullptr;
        it->bucket = bucket;
    }
    UnkMapNode_b0246** slot = fn_7101194aa8(self, bucket);
    UnkMapNode_b0246* node = it->node;
    if (!node)
        return;
    UnkMapNode_b0246* prev = it->prev;
    if (*slot == node) {
        *slot = node->next;
        if (bucket == self->first_bucket) {
            while (bucket < self->bucket_count && !*fn_7101194aa8(self, bucket)) {
                bucket = self->first_bucket + 1;
                self->first_bucket = bucket;
            }
            if (node == self->first) {
                UnkMapNode_b0246* next = nullptr;
                if (self->first_bucket < self->bucket_count)
                    next = *fn_7101194aa8(self, self->first_bucket);
                self->first = next;
            }
        }
    }
    if (prev)
        prev->next = node->next;
    self->count--;
    fn_71011957b0(self, node);
}
