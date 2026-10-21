// The string pool's insert: allocates a node for a string not in the pool yet, links it into
// its bucket and fills its UnkStringData (reference count 1). With `copy` the pool keeps its
// own heap copy of the string; otherwise it points at the caller's. Layouts in
// base/global/UnkStringPool.h.
#include <string.h>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPool.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// NON_MATCHING(minor): register allocation. We keep the heap's GOT address in x28 across the
// buffer compare (the original reloads it in each branch), which renames x22/x23 and moves the
// `set` address computation ahead of the bucket count test.
UnkStringData* fn_71000801e4(UnkStringPool* pool_, u64 hash, const char* str, u32 length, bool copy) {
    char* pool = reinterpret_cast<char*>(pool_);
    if (!**reinterpret_cast<void***>(pool))
        return nullptr;
    char* set = pool + 0x58;
    ++*reinterpret_cast<u32*>(set + 0x48);
    char* node = static_cast<char*>((*reinterpret_cast<base::UnkAllocator**>(set + 0x38))
                                        ->vf_0x40(0x58, 8, *reinterpret_cast<u32*>(set + 0x40)));
    if (node) {
        *reinterpret_cast<u64*>(node) = hash;
        *reinterpret_cast<u32*>(node + 0x10) = 0;
        fn_710007ecb4(node + 0x18, "", 0, -1);
        *reinterpret_cast<u8*>(node + 0x48) = 1;
        *reinterpret_cast<u32*>(node + 0x4c) = 0;
        *reinterpret_cast<char**>(node + 0x8) = nullptr;
        *reinterpret_cast<char**>(node + 0x50) = nullptr;
    }
    u32 bucket = *reinterpret_cast<u32*>(set + 8) ? (u32)hash % *reinterpret_cast<u32*>(set + 0xc) : -1;
    u32& first = *reinterpret_cast<u32*>(set + 0x28);
    first = bucket < first ? bucket : first;
    if (first == bucket)
        *reinterpret_cast<char**>(set + 0x30) = node;
    char** slot = fn_7100080824(set, bucket);
    *reinterpret_cast<char**>(node + 0x50) = *slot;
    *slot = node;
    ++*reinterpret_cast<u32*>(set + 0x44);

    if (!node)
        return reinterpret_cast<UnkStringData*>(pool + 0x10);
    char* data = node + 8;
    char* buffer = data + 0x10;
    char* old = *reinterpret_cast<char**>(data);
    if (fn_710007f240(buffer, "")) {
        fn_7100084144(g_unk_7101d58d40, old);
        *reinterpret_cast<char**>(data) = nullptr;
    }
    const char* s = "";
    if (str && copy) {
        char* block = fn_7100084030(g_unk_7101d58d40, length);
        *reinterpret_cast<char**>(data) = block;
        char* mem = *reinterpret_cast<char**>(block);
        strcpy(mem, str);
        s = mem;
    }
    *reinterpret_cast<u32*>(data + 8) = 1;
    *reinterpret_cast<u32*>(data + 0x44) = 0;
    *reinterpret_cast<u8*>(data + 0x40) = copy;
    fn_710007edac(buffer, s, length, hash);
    return reinterpret_cast<UnkStringData*>(data);
}
