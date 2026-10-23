// Looks up a 64-bit key in a hash map at +0xb8 (base::global::CRntVector buckets, indexed by the
// key's low 32 bits modulo the bucket capacity, as in fn_710019e100): returns the found entry's
// flag, or true when the key isn't there.
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkFlagNode {
    u64 key;
    bool flag;
    u8 field_0x9[0x17];
    UnkFlagNode* next;
};

// base::global::CRntVector (the name comes from its assertion).
template <class T>
struct UnkRntVector {
    T* data;
    u32 size;
    u32 capacity;

    // operator[]; the caller takes the element's address and null-checks it.
    T* At(u32 i) {
        if (!(i < size))
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return &data[i];
    }
};

struct UnkFlagMapOwner {
    u8 field_0x0[0xb8];
    UnkRntVector<UnkFlagNode*> buckets;  // +0xb8
};

bool fn_710014bdec(UnkFlagMapOwner* self, u64 key) {
    if (self->buckets.size == 0)
        return true;
    UnkFlagNode** bucket = self->buckets.At((u32)key % self->buckets.capacity);
    if (!bucket)
        return true;
    for (UnkFlagNode* n = *bucket; n; n = n->next) {
        if (n->key == key)
            return n->flag;
    }
    return true;
}
