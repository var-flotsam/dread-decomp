// Erase at an iterator of another hash set laid out as the string pool set, as fn_7100080a00
// (src/base/global/fn_7100080a00.cpp) does: here the node's link is at 0x10 and the node has
// nothing to destroy before it is freed (buckets through fn_7100086810): fn_71000866dc.
#include "base/UnkVector.h"
#include "base/global/UnkStringPool.h"

using base::global::UnkPoolIter;

char** fn_7100086810(char* set, u32 bucket);  // the address of a bucket

void fn_71000866dc(char* set, UnkPoolIter* it) {
    u32 bucket;
    if (it->ref.set && *reinterpret_cast<u32*>(it->ref.set + 0x48) == it->ref.version) {
        bucket = it->ref.bucket;
    } else {
        bucket = -1;
        it->prev = nullptr;
        it->ref.set = nullptr;
        it->node = nullptr;
        it->ref.bucket = -1;
    }
    char** slot = fn_7100086810(set, bucket);
    char* node = it->node;
    if (!node)
        return;
    char* prev = it->prev;
    u32& first = *reinterpret_cast<u32*>(set + 0x28);
    u32& count = *reinterpret_cast<u32*>(set + 8);
    char*& first_node = *reinterpret_cast<char**>(set + 0x30);
    if (*slot == node) {
        *slot = *reinterpret_cast<char**>(node + 0x10);
        if (bucket == first) {
            for (u32 b = bucket; b < count && !*fn_7100086810(set, b); b = ++first) {
            }
            if (node == first_node)
                first_node = first < count ? *fn_7100086810(set, first) : nullptr;
        }
    }
    if (prev)
        *reinterpret_cast<char**>(prev + 0x10) = *reinterpret_cast<char**>(node + 0x10);
    --*reinterpret_cast<u32*>(set + 0x44);
    (*reinterpret_cast<base::UnkAllocator**>(set + 0x38))->vf_0x48(node);
    ++*reinterpret_cast<u32*>(set + 0x48);
}
