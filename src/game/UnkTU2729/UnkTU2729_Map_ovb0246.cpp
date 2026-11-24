// Unit 2729 (no reliable name): a cache of pooled hash maps, its node release and erase. Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkBucketSet.h"
#include "game/UnkTU2729.h"
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
                StringPool_Release(pool, *slot);
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

using UnkMap_b0246 = base::global::UnkBucketSet<UnkMapNode_b0246>;

struct UnkMapIter_b0246 {
    UnkMapNode_b0246* node;  // 0x0
    UnkMapNode_b0246* prev;  // 0x8
    UnkMap_b0246* map;       // 0x10
    s32 bucket;              // 0x18
    u32 version;             // 0x1c
};

UnkMapNode_b0246** fn_7101194aa8(UnkMap_b0246* self, u32 bucket);

void fn_71011957b0(UnkMap_b0246* self, UnkMapNode_b0246* node) {
    if (!node)
        return;
    ResetId_b0246<&g_unk_7101d47200>(&node->path);
    ResetId_b0246<&g_unk_7101d47108>(&node->name);
    self->node_allocator->vf_0x48(node);
    self->version++;
}

void fn_7101192ee8(void* member);
void fn_71011954cc(void* map);
void fn_71011951e8(void* map);

// A hash map of nodes over a block pool (base/global/UnkBucketSet.h); nodes.field_0x3c is set once
// it is being released.
struct UnkPooledNode_b0246;
using UnkPooledMap_b0246 = base::global::UnkPooledBucketSet<UnkPooledNode_b0246>;

// Frees the pool's buffer through its allocator and the bucket array.
static inline void Release_b0246(UnkPooledMap_b0246* map, void (*clear)(void*)) {
    map->nodes.field_0x3c = true;
    clear(map);
    map->node_allocator = nullptr;
    void* buffer = map->nodes.blocks.buffer;
    map->nodes.vtable = g_unk_7101a1a4c8 + 0x10;
    if (buffer) {
        fn_71000837f4(&map->nodes.blocks);
        map->nodes.parent->vf_0x48(buffer);
        map->nodes.parent = nullptr;
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
    u8 field_0x130[1];          // the member fn_7101192ee8 takes
};
static_assert(__builtin_offsetof(UnkPooledCache_b0246, field_0x130) == 0x130, "UnkPooledCache_b0246 layout");

void fn_7101192dc4(UnkPooledCache_b0246* self) {
    self->vtable_0x0 = g_unk_7101cb5270 + 0x10;
    self->vtable_0x8 = g_unk_7101cb5270 + 0x60;
    fn_7101192ee8(self->field_0x130);
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
            while (bucket < self->buckets.count && !*fn_7101194aa8(self, bucket)) {
                bucket = self->first_bucket + 1;
                self->first_bucket = bucket;
            }
            if (node == self->first_node) {
                UnkMapNode_b0246* next = nullptr;
                if (self->first_bucket < self->buckets.count)
                    next = *fn_7101194aa8(self, self->first_bucket);
                self->first_node = next;
            }
        }
    }
    if (prev)
        prev->next = node->next;
    self->node_count--;
    fn_71011957b0(self, node);
}
