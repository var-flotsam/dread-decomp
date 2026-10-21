// Removes a 64-bit key from the hash set at +8 (buckets indexed by the key's low 32 bits modulo
// the bucket capacity) and appends it to the array at +0x90. Returns whether the key was there.
#include "base/UnkVector.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkKeyNode {
    u64 key;
    u8 field_0x8[0x18];
    UnkKeyNode* next;
};

// base::global::CRntVector (the name comes from its assertion and reflected type names).
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

struct UnkKeySet {
    u8 field_0x0[0x10];
    UnkRntVector<UnkKeyNode*> buckets;  // +0x10
};

struct UnkKeyOwner {
    u8 field_0x0[0x8];
    UnkKeySet set;  // +0x8
    u8 field_0x28[0x68];
    base::UnkVector<u64> removed;  // +0x90
};

void fn_710019e268(UnkKeySet* set, const u64* key);  // erases the key

bool fn_710019e100(UnkKeyOwner* self, u64 key) {
    if (self->set.buckets.size == 0)
        return false;
    UnkKeyNode** bucket = self->set.buckets.At((u32)key % self->set.buckets.capacity);
    if (!bucket)
        return false;
    for (UnkKeyNode* n = *bucket; n; n = n->next) {
        if (n->key == key) {
            u64 k = key;
            fn_710019e268(&self->set, &k);
            self->removed.PushBack(key);
            return true;
        }
    }
    return false;
}
