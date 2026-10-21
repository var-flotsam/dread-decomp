#pragma once

// A chained hash map as some reflected collections use it: an allocator at +0, the entry count
// at +0xc, the bucket array at +0x10 (used/allocated counts at +0x18/+0x1c, its allocator at
// +0x30) and a list of all nodes (+0x38/+0x40; the walk starts at +0x40). Layout and names are
// placeholders inferred from the code.

#include <string.h>

#include "base/UnkVector.h"
#include "types.h"

namespace base {

// Destroys every node's key (at KeyOff; the link to the next node is at NextOff), frees the
// nodes and then the bucket array (fn_7100075aac and its copies).
template <class Key, u32 KeyOff, u32 NextOff>
void refl_HashMapClear(char* self) {
    if (*reinterpret_cast<u32*>(self + 0xc)) {
        char* node = *reinterpret_cast<char**>(self + 0x40);
        while (node) {
            char* next = *reinterpret_cast<char**>(node + NextOff);
            reinterpret_cast<Key*>(node + KeyOff)->~Key();
            (*reinterpret_cast<UnkAllocator**>(self))->vf_0x48(node);
            node = next;
        }
        memset(*reinterpret_cast<void**>(self + 0x10), 0, *reinterpret_cast<u32*>(self + 0x1c) * 8ul);
        *reinterpret_cast<u32*>(self + 0xc) = 0;
        *reinterpret_cast<char**>(self + 0x38) = nullptr;
        *reinterpret_cast<char**>(self + 0x40) = nullptr;
    }
    if (void* buckets = *reinterpret_cast<void**>(self + 0x10)) {
        *reinterpret_cast<u32*>(self + 0x18) = 0;
        (*reinterpret_cast<UnkAllocator**>(self + 0x30))->vf_0x48(buckets);
        *reinterpret_cast<u32*>(self + 0x1c) = 0;
    }
}

// A chained hash set whose bucket array (an UnkVector at +0) is read through an out-of-line
// accessor; the node count is at +0x44, the nodes' allocator at +0x38 and a count of freed nodes
// at +0x48. Clearing destroys each node's key (at 0; the link at `Next`) and frees the node, then
// zeroes the buckets (fn_7100021128 and its copies).
template <class Tag>
struct UnkChainedSetTraits;  // bucket(self, i): the address of bucket i

template <class Tag, class Key, u32 Next>
void refl_ChainedSetClear(char* self) {
    if (*reinterpret_cast<u32*>(self + 0x44)) {
        u32 n = *reinterpret_cast<u32*>(self + 8);
        for (u32 i = 0; i < n; ++i) {
            char* node = *UnkChainedSetTraits<Tag>::bucket(self, i);
            while (node) {
                char* next = *reinterpret_cast<char**>(node + Next);
                reinterpret_cast<Key*>(node)->~Key();
                (*reinterpret_cast<UnkAllocator**>(self + 0x38))->vf_0x48(node);
                ++*reinterpret_cast<u32*>(self + 0x48);
                node = next;
            }
        }
        memset(*reinterpret_cast<void**>(self), 0, *reinterpret_cast<u32*>(self + 0xc) * 8ul);
        *reinterpret_cast<u32*>(self + 0x44) = 0;
        *reinterpret_cast<char**>(self + 0x30) = nullptr;
        *reinterpret_cast<s32*>(self + 0x28) = -1;
    }
}

// refl_HashMapClear for maps whose nodes hold a string id at 0 and a value at 8 with an
// out-of-line destructor (UnkHashMapTraits<Tag>::destroy), destroyed before the key
// (fn_71000d6ba8 and its copies).
template <class Tag>
struct UnkHashMapTraits;

template <class Tag, class Key, u32 NextOff>
void refl_ValueMapClear(char* self) {
    if (*reinterpret_cast<u32*>(self + 0xc)) {
        char* node = *reinterpret_cast<char**>(self + 0x40);
        while (node) {
            char* next = *reinterpret_cast<char**>(node + NextOff);
            UnkHashMapTraits<Tag>::destroy(node + 8);
            reinterpret_cast<Key*>(node)->~Key();
            (*reinterpret_cast<UnkAllocator**>(self))->vf_0x48(node);
            node = next;
        }
        memset(*reinterpret_cast<void**>(self + 0x10), 0, *reinterpret_cast<u32*>(self + 0x1c) * 8ul);
        *reinterpret_cast<u32*>(self + 0xc) = 0;
        *reinterpret_cast<char**>(self + 0x38) = nullptr;
        *reinterpret_cast<char**>(self + 0x40) = nullptr;
    }
    if (void* buckets = *reinterpret_cast<void**>(self + 0x10)) {
        *reinterpret_cast<u32*>(self + 0x18) = 0;
        (*reinterpret_cast<UnkAllocator**>(self + 0x30))->vf_0x48(buckets);
        *reinterpret_cast<u32*>(self + 0x1c) = 0;
    }
}

}  // namespace base
