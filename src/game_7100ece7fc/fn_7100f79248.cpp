// Clear (eight copies over six value types) and two destructors of hash maps keyed by string
// ids: the clear destroys every node on the map's node list (the value at +8 with its own
// destructor, then the key), frees it through the allocator, then empties the buckets and
// resets the count and the list; the destructors clear the map and destroy its storage.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose release decrements and tests the count in two steps.
struct UnkMapKey {
    UnkStringData* data;

    ~UnkMapKey() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

template <u32 Next, void (*Destroy)(void*)>
struct UnkMapNode {
    UnkMapKey key;
    u8 value[Next - 8];  // destroyed by Destroy
    UnkMapNode* next;  // the map's node list
};

struct UnkIdMapStorage {
    base::UnkAllocator* allocator;
    u32 field_0x8;
    u32 count;
    void** buckets;
    u32 field_0x18;
    u32 bucket_count;
    u8 unk_0x20[0x18];
    void* tail;
    void* head;
};

template <u32 Next, void (*Destroy)(void*), class Storage = UnkIdMapStorage>
struct UnkIdMap : Storage {
    using Node = UnkMapNode<Next, Destroy>;

    __attribute__((always_inline)) void Clear() {
        if (!this->count)
            return;
        for (Node* n = static_cast<Node*>(this->head); n;) {
            Node* next = n->next;
            Destroy(n->value);
            n->key.~UnkMapKey();
            this->allocator->vf_0x48(n);
            n = next;
        }
        __builtin_memset(this->buckets, 0, (u64)this->bucket_count * sizeof(void*));
        this->count = 0;
        this->tail = nullptr;
        this->head = nullptr;
    }
};

// The storage's destructor frees the buckets (fn_7100d90040, fn_7100d8f8f8).
template <int Id>
struct UnkIdMapOwnedStorage : UnkIdMapStorage {
    ~UnkIdMapOwnedStorage();
};

// A map destroyed through its storage after clearing.
template <u32 Next, void (*Destroy)(void*), int Id>
struct UnkOwnedIdMap : UnkIdMap<Next, Destroy, UnkIdMapOwnedStorage<Id>> {
    __attribute__((always_inline)) ~UnkOwnedIdMap() { this->Clear(); }
};

#define DEFINE_CLEAR(addr, next, destroy)                                \
    void fn_##destroy(void* value);                                      \
    void fn_##addr(UnkIdMap<next, fn_##destroy>* self) { self->Clear(); }

DEFINE_CLEAR(7100f79248, 0x3b0, 7100d8f9e4)
DEFINE_CLEAR(7100f9c184, 0x3b0, 7100d8f9e4)
DEFINE_CLEAR(7100f86f20, 0x70, 7100d90304)
DEFINE_CLEAR(7100f949a8, 0x80, 7100f8da4c)
DEFINE_CLEAR(7100f96df4, 0x80, 7100f8da4c)
DEFINE_CLEAR(71010045d4, 0x58, 7100fed6cc)
DEFINE_CLEAR(71010057a4, 0x58, 7100fee034)
DEFINE_CLEAR(71010422c4, 0x5d8, 7101036e34)

void fn_7100f963c8(UnkOwnedIdMap<0x80, fn_7100f8da4c, 0>* self) {
    self->~UnkOwnedIdMap();
}

void fn_7100f9c84c(UnkOwnedIdMap<0x3b0, fn_7100d8f9e4, 1>* self) {
    self->~UnkOwnedIdMap();
}

