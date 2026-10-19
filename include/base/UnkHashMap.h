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

}  // namespace base
