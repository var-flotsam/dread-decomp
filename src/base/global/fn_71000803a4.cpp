// The string pool's release: called when a string id's reference count drops to zero. It
// frees the string's heap copy, then finds the string's node in the pool's chained hash set
// (keyed by the string's CRC-64, fn_7100001570) and erases it (layouts in
// base/global/UnkStringPool.h).
#include <new>

#include "base/global/CStrId.h"
#include "base/global/UnkStringPool.h"

using base::global::UnkPoolIter;
using base::global::UnkStringData;
using base::global::UnkStringPool;

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
