// Unit 271 of tools/tu_map.py (base::reflection): collection slot 0x118 of two reflected chained hash
// maps keyed by a pair of u32 (the bucket is their xor modulo the bucket capacity), fn_71001f2148 and
// fn_71001f2244: a node with the key and an 8-byte value goes at the head of its bucket chain and the
// tail of the list of all nodes (the layout of src/base/UnkUnit122/UnkChainedMapErase_b54.cpp). The
// node is filled under a null check; the links are stored after it either way.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkPairKey_b61 {
    u32 a;
    u32 b;
};

struct UnkPairNode_b61 {
    UnkPairKey_b61 key;
    u64 value;
    UnkPairNode_b61* next;   // 0x10: the list of all nodes
    UnkPairNode_b61* prev;   // 0x18
    UnkPairNode_b61* chain;  // 0x20: the next node in the bucket

    // The list links, through accessors (their stores may alias the map's ends).
    UnkPairNode_b61*& Next() { return next; }
    UnkPairNode_b61*& Prev() { return prev; }

};

template <int Tag>
struct UnkPairMap_b61 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkPairNode_b61** buckets;  // 0x10 (base::global::CRntVector)
    u32 bucket_count;           // 0x18
    u32 bucket_capacity;        // 0x1c
    u8 pad_0x20[0x18];
    UnkPairNode_b61* tail;  // 0x38
    UnkPairNode_b61* head;  // 0x40

    UnkPairNode_b61** At(u32 i) {
        if (!(i < bucket_count))
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return &buckets[i];
    }

    __attribute__((always_inline)) void Insert(const UnkPairKey_b61* key, const u64* value) {
        UnkPairNode_b61** slot = nullptr;
        if (bucket_count)
            slot = At((key->a ^ key->b) % bucket_capacity);
        UnkPairNode_b61* chain = *slot;
        auto* n = static_cast<UnkPairNode_b61*>(allocator->vf_0x40(0x28, 8, flags));
        if (n) {
            *reinterpret_cast<u64*>(&n->key) = *reinterpret_cast<const u64*>(key);
            n->value = *value;
            n->next = nullptr;
            n->prev = nullptr;
            n->chain = nullptr;
        }
        n->chain = chain;
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

void fn_71001f2148(UnkPairMap_b61<0>* self, const UnkPairKey_b61* key, const u64* value) { self->Insert(key, value); }
void fn_71001f2244(UnkPairMap_b61<1>* self, const UnkPairKey_b61* key, const u64* value) { self->Insert(key, value); }
