// Unit 347 of tools/tu_map.py (no class hint): creates an object for a string id with fn_71002548c0 and
// files it under the id in the owner's chained hash map at +0x10 (bucket = the string's 64-bit hash
// at +0x30, halves xored, modulo the bucket capacity; the node goes at the head of its bucket chain
// and the tail of the list of all nodes, as in src/base/UnkUnit204/UnkStrIdMapInsert_A1_b71.cpp).
// Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkStringHashView_A1b72 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkStrIdNode_A1b72 {
    CStrId key;
    void* value;
    UnkStrIdNode_A1b72* next;   // 0x10: the list of all nodes
    UnkStrIdNode_A1b72* prev;   // 0x18
    UnkStrIdNode_A1b72* chain;  // 0x20: the next node in the bucket

    UnkStrIdNode_A1b72*& Next() { return next; }
    UnkStrIdNode_A1b72*& Prev() { return prev; }
};

struct UnkStrIdMap_A1b72 {
    using Node = UnkStrIdNode_A1b72;
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    Node** buckets;        // 0x10
    u32 bucket_count;      // 0x18
    u32 bucket_capacity;   // 0x1c
    u8 pad_0x20[0x18];
    Node* tail;  // 0x38
    Node* head;  // 0x40

    __attribute__((always_inline)) void Insert(const CStrId* key, void* const* value) {
        Node** slot = nullptr;
        if (bucket_count) {
            u64 h = reinterpret_cast<const UnkStringHashView_A1b72*>(key->data)->hash;
            u32 i = ((u32)(h >> 32) ^ (u32)h) % bucket_capacity;
            asm("" : "+r"(i));  // the original computes the index before loading the buckets
            slot = &buckets[i];
        }
        Node* c = *slot;
        auto* n = static_cast<Node*>(allocator->vf_0x40(0x28, 8, flags));
        if (n) {
            new (&n->key) CStrId(*key);
            n->value = *value;
            n->next = nullptr;
            n->prev = nullptr;
            n->chain = nullptr;
        }
        n->chain = c;
        *slot = n;
        if (!head) {
            head = n;
        } else if (tail) {
            n->Prev() = tail;
            tail->Next() = n;
        }
        tail = n;
        ++count;
    }
};

struct UnkUnit347Owner_A1b72 {
    u8 pad_0x0[0x10];
    UnkStrIdMap_A1b72 map;  // 0x10
};

void* fn_71002548c0(const CStrId* key, void* arg, UnkUnit347Owner_A1b72* owner);

// The signature src/game/UnkUnit347/UnkUnit347_ReflCall_b41.cpp declares.
void* fn_71002634f8(char* self, void* a, void* b) {
    auto* owner = reinterpret_cast<UnkUnit347Owner_A1b72*>(self);
    const CStrId* key = static_cast<const CStrId*>(a);
    void* object = fn_71002548c0(key, b, owner);
    if (object)
        owner->map.Insert(key, &object);
    return object;
}
