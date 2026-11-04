// Unit 2560 (tools/tu_map.py, no name hint): two-level lookups. The object's chained hash map
// (buckets at Map, count at Map + 8, modulus at Map + 0xc) is keyed by objects whose 64-bit hash is
// at +0x30; the found node (bucket chain at Chain) holds a packed list at +8 (u32 count, then
// Stride-byte entries from +0xc) whose entries start with a pointer to an object hashed the same
// way. Returns the entry whose hash equals `inner`, or null. Names and layouts are placeholders.
#include "types.h"

namespace {

struct UnkHashed_A6b73 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkHashNode_A6b73 {
    UnkHashed_A6b73* key;
    u32 count;  // 0x8
};

template <u32 Map, u32 Chain, u32 Stride>
static inline __attribute__((always_inline)) void* FindEntry(void* self_, u64* outer, void* inner_) {
    char* self = static_cast<char*>(self_);
    if (*reinterpret_cast<u32*>(self + Map + 8) == 0)
        return nullptr;
    u64 h = *outer;
    u32 i = ((u32)(h >> 32) ^ (u32)h) % *reinterpret_cast<u32*>(self + Map + 0xc);
    char** bucket = *reinterpret_cast<char***>(self + Map) + i;
    if (!bucket)
        return nullptr;
    char* n = *bucket;
    if (!n)
        return nullptr;
    while (reinterpret_cast<UnkHashNode_A6b73*>(n)->key->hash != h) {
        n = *reinterpret_cast<char**>(n + Chain);
        if (!n)
            return nullptr;
    }
    u64 bytes = (u64)reinterpret_cast<UnkHashNode_A6b73*>(n)->count * Stride;
    asm("" : "+r"(bytes));  // the original keeps the byte size as the loop counter
    if (!bytes)
        return nullptr;
    char* it = n + 0xc;
    u64 want = *static_cast<u64*>(inner_);
    do {
        if ((*reinterpret_cast<UnkHashed_A6b73**>(it))->hash == want)
            return it;
        it += Stride;
        bytes -= Stride;
    } while (bytes);
    return nullptr;
}

}  // namespace

void* fn_7100e1cb50(void* self, u64* outer, void* inner) {
    return FindEntry<0x60, 0x2a0, 0x28>(self, outer, inner);
}
