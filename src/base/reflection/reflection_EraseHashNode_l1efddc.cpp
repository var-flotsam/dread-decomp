// Unit base::reflection: removes the node with a given 64-bit key from a chained hash table, unlinks it from
// the table's ordered list, frees it and returns the neighbouring node. Names and layouts are placeholders.
#include "types.h"

extern "C" __attribute__((noreturn)) void __assert_fail(const char*, const char*, unsigned, const char*);

struct UnkHashNode_l1efddc {
    u64 key;
    u64 field_0x8;
    UnkHashNode_l1efddc* prev;
    UnkHashNode_l1efddc* next;
    UnkHashNode_l1efddc* chain;
};

struct UnkNodeAllocator {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(UnkHashNode_l1efddc* node);
};

struct UnkHashTable_l1efddc {
    UnkNodeAllocator* allocator;
    u32 pad_0x8;
    s32 count;
    UnkHashNode_l1efddc** buckets;
    u32 bucketCount;
    u32 modulus;
    u8 pad_0x20[0x18];
    UnkHashNode_l1efddc* head;
    UnkHashNode_l1efddc* tail;
};

static inline __attribute__((always_inline)) UnkHashNode_l1efddc* EraseNode(UnkHashTable_l1efddc* self, const u64* key) {
    u64 k = *key;
    u32 i = (u32)(k >> 32) ^ (u32)k;
    i %= self->modulus;
    if (self->bucketCount <= i)
        __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
    UnkHashNode_l1efddc** slot = &self->buckets[i];
    UnkHashNode_l1efddc* node = *slot;
    if (!node)
        return nullptr;
    UnkHashNode_l1efddc* before = nullptr;
    if (node->key != k) {
        do {
            before = node;
            node = node->chain;
            if (!node)
                return nullptr;
        } while (node->key != k);
    }
    *(before ? &before->chain : slot) = node->chain;
    if (self->tail == node)
        self->tail = node->prev;
    if (self->head == node) {
        UnkHashNode_l1efddc* n = node->next;
        self->head = n;
    }
    if (node->next) {
        UnkHashNode_l1efddc* p = node->prev;
        node->next->prev = p;
    }
    if (node->prev) {
        UnkHashNode_l1efddc* n = node->next;
        node->prev->next = n;
    }
    UnkHashNode_l1efddc* result = node->prev;
    self->allocator->vf_0x48(node);
    self->count--;
    return result;
}

struct UnkRegMap_ovb0108;

void* fn_71001efddc(UnkRegMap_ovb0108* map, const u64* key) {
    return EraseNode(reinterpret_cast<UnkHashTable_l1efddc*>(map), key);
}
