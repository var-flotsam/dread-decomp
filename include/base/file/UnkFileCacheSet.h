#pragma once

// A chained hash set of the file devices' units (tools/tu_map.py: unit 62, base::file::CFileCacheDevice,
// whose constructor is fn_7100085cd8 and its clear fn_71000853b8, see base/UnkHashMap.h's
// refl_ChainedSetClear; and unit 68, base::file::CThreadSafeDevice): setting up its storage, with
// nodes of each unit's size. Names and layouts are placeholders inferred from the code.

#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkFileCacheSetNode;

// The bucket array at +0, the first non-empty bucket (+0x28) and first node (+0x30), the nodes'
// allocator (+0x38: the pool at +0x50 once set up) with its flags (+0x40), the node count
// (+0x44) and the count of freed nodes (+0x48).
struct UnkFileCacheSet {
    base::UnkVector<UnkFileCacheSetNode*> buckets;
    s32 field_0x28;
    UnkFileCacheSetNode* field_0x30;
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    u32 field_0x48;
    base::UnkBlockPool pool;
};
static_assert(__builtin_offsetof(UnkFileCacheSet, pool) == 0x50, "UnkFileCacheSet layout");

// Releases the node pool's old block, carves a block of `nodes` nodes of `NodeSize` bytes (from the
// set's allocator, or the default one) into the pool, makes the pool the set's allocator, then
// resizes the bucket array to `buckets`, clearing the new buckets.
template <u64 NodeSize>
__attribute__((always_inline)) inline void UnkFileCacheSetSetUp(UnkFileCacheSet* self, u32 buckets, u32 nodes) {
    u32 flags = self->flags;
    base::UnkAllocator* a = self->allocator;
    self->pool.Release();
    if (nodes) {
        if (!a)
            a = fn_7100083990();
        self->pool.parent = a;
        u64 size = (u64)nodes * NodeSize;
        void* block = a->vf_0x40(size, 8, flags);
        fn_710008378c(&self->pool.blocks, block, size, NodeSize);
    }
    // The pool's address, formed as an integer: the original recomputes it here rather than
    // keeping it live across the calls (as base::UnkPoolList::Init).
    self->allocator = reinterpret_cast<base::UnkAllocator*>(reinterpret_cast<u64>(self) + 0x50);
    base::UnkVector<UnkFileCacheSetNode*>& v = self->buckets;
    u32 count = v.count;
    if (count < buckets) {
        if (v.capacity < buckets) {
            UnkFileCacheSetNode** p = static_cast<UnkFileCacheSetNode**>(
                v.allocator->vf_0x40((u64)buckets * sizeof(UnkFileCacheSetNode*), 8, v.flags));
            if (UnkFileCacheSetNode** old = v.data) {
                // Copied bytewise: the original reloads the count after each element.
                UnkFileCacheSetNode** dst = p;
                for (u32 i = 0; i < v.count; ++i, ++dst, ++old)
                    __builtin_memcpy(dst, old, sizeof(*dst));
                v.allocator->vf_0x48(v.data);
            }
            count = v.count;
            v.data = p;
            v.capacity = buckets;
        }
        for (u32 i = count; i < buckets; ++i)
            v.data[i] = nullptr;
        v.count = buckets;
    } else if (count > buckets) {
        v.count = buckets;
    }
}
