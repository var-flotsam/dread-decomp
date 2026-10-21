// The string pool's release: called when a string id's reference count drops to zero. It
// frees the string's heap copy, then finds the string's node in the pool's chained hash set
// (keyed by the string's CRC-64, fn_7100001570) and erases it. Layouts are placeholders:
// a node is the key (+0), the UnkStringData (+0x8) and the next node in its bucket (+0x50);
// the data keeps the heap copy at +0, a string buffer at +0x10 and the key again at +0x30.
#include <new>

#include "base/global/CStrId.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

namespace base::global {

// The set, a bucket index (-1 when out of range) and the set's modification count.
struct UnkPoolBucketRef {
    char* set;
    u32 bucket;
    u32 version;

    UnkPoolBucketRef(char* set, u32 bucket)
        : set(set), bucket(bucket), version(*reinterpret_cast<u32*>(set + 0x48)) {}
};

// Where a lookup in the pool's set ended: the node found (or null) and the node before it.
struct UnkPoolIter {
    char* node;
    char* prev;
    UnkPoolBucketRef ref;

    UnkPoolIter(char* node, char* prev, char* set, u32 bucket) : node(node), prev(prev), ref(set, bucket) {}
};

}  // namespace base::global

using base::global::UnkPoolIter;

char** fn_7100080824(char* set, u32 bucket);  // the address of a bucket
bool fn_710007f240(char* buffer, const char* str);
void fn_7100084144(void* heap, char* ptr);       // free
void fn_7100080a00(char* set, UnkPoolIter* it);  // erase
void fn_710007edac(char* buffer, const char* str, u32 length, s64 capacity);

extern void* g_unk_7101d58d40;  // the heap the string copies come from (reached through the GOT)

namespace base::global {

// Inlined: the node with this key and the node before it in its bucket. Building the iterator
// in place keeps the original's loop registers.
inline void UnkPoolFind(UnkPoolIter* it, char* set, u64 key) {
    u32 n = *reinterpret_cast<u32*>(set + 8);
    if (n) {
        u32 bucket = (u32)key % *reinterpret_cast<u32*>(set + 0xc);
        if (bucket < n) {
            char* prev = nullptr;
            char* node = *fn_7100080824(set, bucket);
            while (node && *reinterpret_cast<u64*>(node) != key) {
                prev = node;
                node = *reinterpret_cast<char**>(node + 0x50);
            }
            new (it) UnkPoolIter(node, prev, set, bucket);
            return;
        }
    }
    new (it) UnkPoolIter(nullptr, nullptr, set, -1);
}

}  // namespace base::global

// NON_MATCHING(minor): block layout of the inlined lookup. The original computes the
// iterator's `ref` address (sp+0x10) in both branches and stores through it after the join,
// and stores a not-found result with one `stp`; we fold the address (3 instructions fewer).
void fn_71000803a4(UnkStringPool* pool_, UnkStringData* data_) {
    char* pool = reinterpret_cast<char*>(pool_);
    char* data = reinterpret_cast<char*>(data_);
    // Loaded before the empty-string test, as in the original.
    void* owner = **reinterpret_cast<void***>(pool);
    if (pool + 0x10 == data || !owner)
        return;
    if (char* str = *reinterpret_cast<char**>(data)) {
        if (fn_710007f240(data + 0x10, "")) {
            fn_7100084144(g_unk_7101d58d40, str);
            *reinterpret_cast<char**>(data) = nullptr;
        }
    }
    char* set = pool + 0x58;
    char* buffer = data + 0x10;
    alignas(UnkPoolIter) char raw[sizeof(UnkPoolIter)];
    UnkPoolIter* it = reinterpret_cast<UnkPoolIter*>(raw);
    base::global::UnkPoolFind(it, set, *reinterpret_cast<u64*>(data + 0x30));
    fn_7100080a00(set, it);
    fn_710007edac(buffer, "", 0, -1);
}
