// CLightManager unit (tools/tu_map.py), two groups of functions.
//
// A chained hash map keyed by file-path ids (the bucket is the string's 64-bit hash at +0x30, halves
// xored, modulo the bucket capacity) whose values are the objects of
// src/game/CLightManager/fn_7100e0ccf8.cpp (a 0x58-byte base, a file-path id, an array cleared out of
// line by fn_7100da0870, a flag and a word); nodes keep the list links at +0x98 / +0xa0 and the bucket
// chain at +0xa8. The "set by key" move slot (fn_7100e0e9cc): the found value's array takes the
// other's (unless they are the same object), then its id, flag and word. The moving insert
// (fn_7100e0e788): a new node takes a copy of the key and the moved value, and goes at the head of its
// bucket chain and the tail of the list of all nodes.
//
// A fourth copy of the list pair of src/game/CLightManager/fn_7100e0babc.cpp, at +0xf0
// (fn_7100e0bf48, fn_7100e0b9a0). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/core/CAsset.h"
#include "base/global/CStrId.h"
#include "game/CLightsDef.h"
#include "game/UnkTU290.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// As src/game/CLightManager/fn_7100e0ccf8.cpp declares it.
struct UnkClearedArray;

struct UnkLightHashView_ovb0060 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkLightArray_ovb0060 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void MoveFrom(UnkLightArray_ovb0060& other) {
        if (this != &other) {
            fn_7100da0870(reinterpret_cast<UnkClearedArray*>(this), 0);
            allocator = other.allocator;
            flags = other.flags;
            step = other.step;
            field_0x14 = other.field_0x14;
            count = other.count;
            capacity = other.capacity;
            data = other.data;
            other.data = nullptr;
            other.count = 0;
            other.capacity = 0;
        }
    }
};

struct UnkLightPathId_ovb0060 {
    UnkStringData* data;

    __attribute__((always_inline)) void Take(UnkLightPathId_ovb0060& other) {
        UnkStringPool* pool = g_unk_7101d47200;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        other.data = &pool->empty;
    }
};

extern void* g_unk_7101aa9140[];

struct UnkLightValue_ovb0060 {
    void** vtable;
    u8 base_0x8[0x50];
    UnkLightPathId_ovb0060 path;  // 0x58
    UnkLightArray_ovb0060 array;  // 0x60
    bool field_0x88;
    u32 field_0x8c;

    __attribute__((always_inline)) void ConstructFrom(UnkLightValue_ovb0060& other) {
        fn_7100016ad8(this);
        vtable = &g_unk_7101aa9140[2];
        path.data = other.path.data;
        other.path.data = &g_unk_7101d47200->empty;
        array.data = other.array.data;
        array.count = other.array.count;
        array.capacity = other.array.capacity;
        array.step = other.array.step;
        array.field_0x14 = other.array.field_0x14;
        array.flags = other.array.flags;
        array.allocator = other.array.allocator;
        other.array.data = nullptr;
        other.array.count = 0;
        other.array.capacity = 0;
        field_0x88 = other.field_0x88;
        field_0x8c = other.field_0x8c;
    }

    __attribute__((always_inline)) void MoveFrom(UnkLightValue_ovb0060& other) {
        array.MoveFrom(other.array);
        path.Take(other.path);
        field_0x88 = other.field_0x88;
        field_0x8c = other.field_0x8c;
    }
};

struct UnkLightNode_ovb0060 {
    UnkStringData* key;
    UnkLightValue_ovb0060 value;  // 0x8
    UnkLightNode_ovb0060* next;   // 0x98
    UnkLightNode_ovb0060* prev;   // 0xa0
    UnkLightNode_ovb0060* chain;  // 0xa8
};
static_assert(__builtin_offsetof(UnkLightNode_ovb0060, chain) == 0xa8, "UnkLightNode_ovb0060 layout");

struct UnkLightMap_ovb0060 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkLightNode_ovb0060** buckets;  // 0x10
    u32 bucket_count;                // 0x18
    u32 bucket_capacity;             // 0x1c
    u8 pad_0x20[0x18];
    UnkLightNode_ovb0060* tail;  // 0x38
    UnkLightNode_ovb0060* head;  // 0x40
};

void fn_7100e0e9cc(UnkLightMap_ovb0060* self, const UnkLightPathId_ovb0060* key, UnkLightValue_ovb0060* value) {
    if (!self->bucket_count)
        return;
    UnkStringData* k = key->data;
    u64 h = reinterpret_cast<const UnkLightHashView_ovb0060*>(k)->hash;
    u32 i = ((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity;
    UnkLightNode_ovb0060** bucket = &self->buckets[i];
    if (!bucket)
        return;
    UnkLightNode_ovb0060* n = *bucket;
    if (!n)
        return;
    while (n->key != k) {
        n = n->chain;
        if (!n)
            return;
    }
    n->value.MoveFrom(*value);
}

void fn_7100e0e788(UnkLightMap_ovb0060* self, const UnkLightPathId_ovb0060* key, UnkLightValue_ovb0060* value) {
    UnkLightNode_ovb0060** slot = nullptr;
    if (self->bucket_count) {
        u64 h = reinterpret_cast<const UnkLightHashView_ovb0060*>(key->data)->hash;
        u32 i = ((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity;
        slot = &self->buckets[i];
    }
    UnkLightNode_ovb0060* chain = *slot;
    auto* n = static_cast<UnkLightNode_ovb0060*>(self->allocator->vf_0x40(0xb0, 8, self->flags));
    UnkStringData* k = key->data;
    n->key = k;
    if (&g_unk_7101d47200->empty != k)
        ++k->ref_count;
    n->value.ConstructFrom(*value);
    n->next = nullptr;
    n->prev = nullptr;
    n->chain = chain;
    *slot = n;
    if (!self->head) {
        self->head = n;
    } else if (self->tail) {
        __builtin_memcpy(&n->prev, &self->tail, sizeof(n->prev));
        self->tail->next = n;
    }
    self->tail = n;
    ++self->count;
}

struct UnkListedObj;

// Erasing moves the later entries down with memcpy.
struct UnkPtrList_ovb0060 : base::UnkVector<UnkListedObj*> {
    __attribute__((always_inline)) void EraseAt(u64 index) {
        if (index < count) {
            __builtin_memcpy(&data[index], &data[index + 1], (u64)(u32)(count - 1 - (u32)index) * 8);
            --count;
        }
    }
};

struct UnkListPair_ovb0060 {
    u8 unk_0x0[0x8];
    base::UnkVector<UnkListedObj*> active;
    UnkPtrList_ovb0060 pending;
};

UnkListedObj** fn_7100e0a88c(UnkPtrList_ovb0060* list, u32 index);

void fn_7100e0bf48(char* self, UnkListedObj* obj) {
    if (!obj)
        return;
    UnkListPair_ovb0060* lists = reinterpret_cast<UnkListPair_ovb0060*>(self + 0xf0);
    fn_710020c8ac(reinterpret_cast<char*>(obj) + 0x20, false);
    for (u64 i = 0; i < lists->pending.count; ++i) {
        if (*fn_7100e0a88c(&lists->pending, i) == obj) {
            lists->pending.EraseAt(i);
            lists->active.PushBack(obj);
        }
    }
}

UnkListedObj* fn_7100e0b9a0(char* self) {
    UnkListPair_ovb0060* lists = reinterpret_cast<UnkListPair_ovb0060*>(self + 0xf0);
    if (lists->active.count == 0)
        return nullptr;
    UnkListedObj* obj = *(lists->active.data + lists->active.count - 1);
    fn_710020c8ac(reinterpret_cast<char*>(obj) + 0x20, true);
    --lists->active.count;
    lists->pending.PushBack(obj);
    return obj;
}
