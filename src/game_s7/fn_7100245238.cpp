// Copies of fn_7100d8a010 (src/game_aca57c/fn_7100d419c0.cpp), which the binary
// repeats at other addresses: fn_7100245238, fn_71002487ac, fn_7100a75724.
// Hash map node inserts: a node holding a string id and a value (a reference-counted pointer,
// assigned, or a vector, moved in) is allocated, pushed onto the front of the key's bucket chain
// and appended to the list of all nodes. The bucket comes from the key's two hash words modulo
// the bucket count.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    // A union member, so the count's updates may alias the node's fields (the original reloads
    // the old value after the increment).
    union {
        s32 ref_count;
    };
};

struct UnkRefPtr {
    UnkRefObject* ptr = nullptr;

    UnkRefPtr& operator=(const UnkRefPtr& other) {
        UnkRefObject* p = other.ptr;
        if (p != ptr) {
            if (p)
                ++p->ref_count;
            if (UnkRefObject* old = ptr) {
                if (--old->ref_count == 0)
                    old->vf_0x10();
            }
            ptr = p;
        }
        return *this;
    }
};

struct UnkKeyedNode {
    CStrId key;
    UnkRefPtr value;
    UnkKeyedNode* next;
    UnkKeyedNode* prev;
    UnkKeyedNode* chain;

    UnkKeyedNode(const CStrId& key, const UnkRefPtr& value) : key(key) {
        this->value = value;
        prev = nullptr;
        chain = nullptr;
        next = nullptr;
    }
};

// A vector (base::UnkVector's layout) that is moved into the node.
struct UnkMovedVector {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) UnkMovedVector(UnkMovedVector&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

struct UnkKeyedVectorNode {
    CStrId key;
    UnkMovedVector value;
    UnkKeyedVectorNode* next;
    UnkKeyedVectorNode* prev;
    UnkKeyedVectorNode* chain;

    UnkKeyedVectorNode(const CStrId& key, UnkMovedVector& value)
        : key(key), value(static_cast<UnkMovedVector&&>(value)) {
        prev = nullptr;
        chain = nullptr;
        next = nullptr;
    }
};

template <class Node>
struct UnkKeyedMap {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    Node** buckets;
    u32 used;
    u32 bucket_count;
    u8 unk_0x20[0x18];
    Node* tail;
    Node* head;
};

struct UnkHashWords {
    u8 unk_0x0[0x30];
    u32 hash_a;
    u32 hash_b;
};

template <class Node, class Value>
__attribute__((always_inline)) static inline Node* Insert(UnkKeyedMap<Node>* self, const CStrId& key, Value& value) {
    Node** bucket;
    if (self->used) {
        const UnkHashWords* h = *reinterpret_cast<UnkHashWords* const*>(&key);
        u32 i = (h->hash_a ^ h->hash_b) % self->bucket_count;
        bucket = self->buckets + i;
    } else {
        bucket = nullptr;
    }
    Node* old = *bucket;
    Node* node = static_cast<Node*>(self->allocator->vf_0x40(sizeof(Node), 8, self->flags));
    if (node)
        new (node) Node(key, value);
    node->chain = old;
    *bucket = node;
    if (!self->head) {
        self->head = node;
    } else {
        Node** tail = &self->tail;
        if (*tail) {
            node->prev = *tail;
            (*tail)->next = node;
        }
    }
    self->tail = node;
    ++self->count;
    return node;
}

UnkKeyedVectorNode* fn_7100245238(UnkKeyedMap<UnkKeyedVectorNode>* self, const CStrId& key, UnkMovedVector& value) {
    return Insert(self, key, value);
}

UnkKeyedVectorNode* fn_71002487ac(UnkKeyedMap<UnkKeyedVectorNode>* self, const CStrId& key, UnkMovedVector& value) {
    return Insert(self, key, value);
}

UnkKeyedVectorNode* fn_7100a75724(UnkKeyedMap<UnkKeyedVectorNode>* self, const CStrId& key, UnkMovedVector& value) {
    return Insert(self, key, value);
}

