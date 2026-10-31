// A hashed set's erase at an iterator, in the minimap manager's unit: the same as the string
// pool's erase (fn_7100080a00, src/base/global/fn_7100080a00.cpp) with this set's bucket accessor
// (fn_7100eb3814) and node destructor (fn_7100eb3d9c). Unlinks the node from its bucket, moves
// the first non-empty bucket and first node on if they were this one's, destroys the node's
// value and frees the node; a stale iterator is reset to the end first.
#include "base/UnkVector.h"
#include "base/global/UnkStringPool.h"

using base::global::UnkPoolIter;

// The address of a bucket (declared as src/game_aca57c/fn_7100e9bf60.cpp does).
struct UnkRecord;
struct UnkRecordBuckets;
UnkRecord** fn_7100eb3814(UnkRecordBuckets* set, u32 bucket);
static inline char** Bucket(char* set, u32 bucket) {
    return reinterpret_cast<char**>(fn_7100eb3814(reinterpret_cast<UnkRecordBuckets*>(set), bucket));
}
void fn_7100eb3d9c(char* value);              // the node's destructor (its value is at 0; the link at 0x15f8)

void fn_7100ea5c40(char* set, UnkPoolIter* it) {
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
    char** slot = Bucket(set, bucket);
    char* node = it->node;
    if (!node)
        return;
    char* prev = it->prev;
    u32& first = *reinterpret_cast<u32*>(set + 0x28);
    u32& count = *reinterpret_cast<u32*>(set + 8);
    char*& first_node = *reinterpret_cast<char**>(set + 0x30);
    if (*slot == node) {
        *slot = *reinterpret_cast<char**>(node + 0x15f8);
        if (bucket == first) {
            for (u32 b = bucket; b < count && !*Bucket(set, b); b = ++first) {
            }
            if (node == first_node)
                first_node = first < count ? *Bucket(set, first) : nullptr;
        }
    }
    if (prev)
        *reinterpret_cast<char**>(prev + 0x15f8) = *reinterpret_cast<char**>(node + 0x15f8);
    --*reinterpret_cast<u32*>(set + 0x44);
    fn_7100eb3d9c(node);
    (*reinterpret_cast<base::UnkAllocator**>(set + 0x38))->vf_0x48(node);
    ++*reinterpret_cast<u32*>(set + 0x48);
}
