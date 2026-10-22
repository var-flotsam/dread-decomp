// Copies for a map with 0x18-byte values (destroyed by fn_71000a1c60), naming their own callees and data (the comments
// are adapted from the original's): of fn_7100f9c184 (src/game_7100ece7fc/fn_7100f79248.cpp): fn_71000d6844; and of
// fn_7100f8760c (src/game_7100ece7fc/fn_7100f79844.cpp): fn_71000d6f6c, with fn_71000d7024 the same from no node. Clear
// of a hash map keyed by string ids: destroys every node on the map's node list (the value at +8 with its own
// destructor, then the key), frees it through the allocator, then empties the buckets and resets the count and the
// list. Begin of the same maps: a temporary iterator starts at the map's first node and is advanced (fn_71000c2f58),
// the result starts at the node it reached and is advanced in turn, and the temporary's cached entry (kind 1: a key and
// a value) is destroyed.
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

template <u32 Next, void (*Destroy)(void*)>
struct UnkIdMap : UnkIdMapStorage {
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

void fn_71000a1c60(void* value);

void fn_71000d6844(UnkIdMap<0x20, fn_71000a1c60>* self) { self->Clear(); }

template <u32 ValueSize, void (*Destroy)(void*)>
struct UnkMapIter {
    void* node;
    union {
        struct {
            UnkMapKey key;
            u8 value[ValueSize];  // destroyed by Destroy
        } entry;  // kind 1
    };
    u32 kind;

    void Advance();  // fn_71000c2f58

    __attribute__((always_inline)) ~UnkMapIter() {
        switch (kind) {
        case 1:
            Destroy(entry.value);
            entry.key.~UnkMapKey();
            kind = 0;
            break;
        case 2:
            kind = 0;
            break;
        }
    }
};

struct UnkMapHead {
    u8 unk_0x0[0x40];
    void* head;
};

template <u32 ValueSize, void (*Destroy)(void*)>
__attribute__((always_inline)) inline void Start(void* node, UnkMapIter<ValueSize, Destroy>* out) {
    UnkMapIter<ValueSize, Destroy> it;
    it.node = node;
    it.kind = 0;
    it.Advance();
    out->node = it.node;
    out->Advance();
}

void fn_71000d6f6c(const UnkMapHead* map, UnkMapIter<0x18, fn_71000a1c60>* out) {
    Start(map->head, out);
}

void fn_71000d7024(const UnkMapHead* map, UnkMapIter<0x18, fn_71000a1c60>* out) {
    Start<0x18, fn_71000a1c60>(nullptr, out);
}
