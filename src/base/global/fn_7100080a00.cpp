// The string pool set's erase at an iterator (fn_71000803a4 builds it): unlinks the node from
// its bucket, moves the set's first non-empty bucket and first node on if they were this one's,
// destroys the node's string buffer and frees the node. An iterator from before the set's last
// change (its modification count differs) is reset to the end first.
#include "base/UnkVector.h"
#include "base/global/UnkStringPool.h"

using base::global::UnkPoolIter;

void fn_7100080a00(char* set, UnkPoolIter* it) {
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
    char** slot = fn_7100080824(set, bucket);
    char* node = it->node;
    if (!node)
        return;
    char* prev = it->prev;
    u32& first = *reinterpret_cast<u32*>(set + 0x28);
    u32& count = *reinterpret_cast<u32*>(set + 8);
    char*& first_node = *reinterpret_cast<char**>(set + 0x30);
    if (*slot == node) {
        *slot = *reinterpret_cast<char**>(node + 0x50);
        if (bucket == first) {
            for (u32 b = bucket; b < count && !*fn_7100080824(set, b); b = ++first) {
            }
            if (node == first_node)
                first_node = first < count ? *fn_7100080824(set, first) : nullptr;
        }
    }
    if (prev)
        *reinterpret_cast<char**>(prev + 0x50) = *reinterpret_cast<char**>(node + 0x50);
    --*reinterpret_cast<u32*>(set + 0x44);
    fn_710007ecd4(node + 0x18);
    (*reinterpret_cast<base::UnkAllocator**>(set + 0x38))->vf_0x48(node);
    ++*reinterpret_cast<u32*>(set + 0x48);
}
