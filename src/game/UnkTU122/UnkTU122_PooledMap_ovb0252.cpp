// Unit 122 (data/tu_map.csv, no reliable name): the constructor of a pooled chained map with a pointer list, and
// the factory that builds an object (0x218 bytes) holding a counted resource and files it in the map. Names and
// layouts are placeholders.
#include "base/UnkChainedMap.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "game/UnkTU127.h"
#include "game/UnkTU291.h"
#include "types.h"
#include <new>
struct UnkPooledChainedMap_b0252 {
    UnkChainedMap map;
    base::UnkBlockPool pool;  // 0x48
    base::UnkVector<void*> entries;  // 0x88
};
static_assert(__builtin_offsetof(UnkPooledChainedMap_b0252, entries) == 0x88, "layout");

void fn_7100142798(UnkPooledChainedMap_b0252* self, u32 buckets, u32 nodes);

void fn_71001420d8(UnkPooledChainedMap_b0252* self) {
    new (&self->map) UnkChainedMap();
    self->pool.Init();
    self->pool.field_0x38 = 0;
    self->pool.field_0x3c = false;
    fn_7100142798(self, 0x400, 0x4000);
    new (&self->entries) base::UnkVector<void*>(0xf);
    self->entries.Reserve(0x1000);
}

struct UnkTaggedSubObject127;
struct UnkTaggedSubArg;

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkPoolNode_b0252 {
    void* key;
    void* value;
    UnkPoolNode_b0252* next;   // 0x10: the list of all nodes
    UnkPoolNode_b0252* prev;   // 0x18
    UnkPoolNode_b0252* chain;  // 0x20: the next node in the bucket
};

struct UnkPoolMap_b0252 {
    using Node = UnkPoolNode_b0252;
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    Node** buckets;       // 0x10
    u32 bucket_count;     // 0x18
    u32 bucket_capacity;  // 0x1c
    u8 pad_0x20[0x18];
    Node* tail;  // 0x38
    Node* head;  // 0x40

    __attribute__((always_inline)) void Insert(void* key, void* value) {
        Node** slot = nullptr;
        if (bucket_count) {
            u32 i = (u32)(u64)key % bucket_capacity;
            if (bucket_count <= i)
                __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
            slot = &buckets[i];
        }
        Node* c = *slot;
        auto* n = static_cast<Node*>(allocator->vf_0x40(0x28, 8, flags));
        if (n) {
            n->key = key;
            n->value = value;
            n->next = nullptr;
            n->prev = nullptr;
            n->chain = nullptr;
        }
        n->chain = c;
        *slot = n;
        if (!head) {
            head = n;
        } else if (tail) {
            __builtin_memcpy(&n->prev, &tail, sizeof(Node*));  // the original reloads the tail
            tail->next = n;
        }
        tail = n;
        ++count;
    }
};

struct UnkDeletable_b0252 {
    void** vtable;
    void Refresh() { reinterpret_cast<void (*)(UnkDeletable_b0252*)>(vtable[0x90 / 8])(this); }
};

struct UnkResourceHolder_b0252 {
    u8 pad_0x0[0x10];
    UnkCountedResource* resource;  // 0x10
    u8 pad_0x18[8];
    UnkDeletable_b0252 sub;  // 0x20
    u8 pad_0x28[0x98 - 0x28];
    UnkCountedResource* second;  // 0x98
};

static inline void Assign(UnkCountedResource** slot, UnkCountedResource* r) {
    if (*slot != r) {
        if (r)
            ++r->ref_count;
        if (UnkCountedResource* old = *slot) {
            if (--old->ref_count == 0)
                fn_710001737c(old);
        }
        *slot = r;
    }
}

void* fn_7100141f1c(UnkPoolMap_b0252* map, UnkCountedResource* resource, UnkResourceHolder_b0252** out, UnkTaggedSubArg* name) {
    UnkResourceHolder_b0252* object = static_cast<UnkResourceHolder_b0252*>(fn_71000839a0(0x218, 0x32));
    fn_710014333c(reinterpret_cast<UnkTaggedSubObject127*>(object), name);
    void* node = nullptr;
    if (object) {
        fn_710020d32c(reinterpret_cast<char*>(object) + 0x20);
        Assign(&object->resource, resource);
        node = reinterpret_cast<u8*>(object) + 8;
        if (resource) {
            Assign(&object->second, resource);
            object->sub.Refresh();
        }
        map->Insert(node, object);
    }
    if (out)
        *out = object;
    return node;
}
