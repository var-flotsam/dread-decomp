// A copy of fn_710019e100 (src/game_lo/fn_710019e100.cpp) for another owner: if a 64-bit key is
// in the hash set at +0 (buckets indexed by the key's low 32 bits modulo the bucket capacity),
// it is appended to the array at +0x88, then erased from the set. Returns whether it was there.
#include "base/global/CRntVector.h"
#include "engine/UnkKeyNode.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

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

struct UnkKeySet {
    u8 field_0x0[0x10];
    UnkRntVector<UnkKeyNode*> buckets;  // +0x10
};

struct UnkKeyOwner_14239c {
    UnkKeySet set;  // +0x0
    u8 field_0x20[0x68];
    base::global::CRntVector<u64> moved;  // +0x88
};

void* EraseChainedMapNodeByU64Key(UnkKeySet* set, const u64* key);  // erases the key

bool fn_710014239c(UnkKeyOwner_14239c* self, u64 key) {
    if (self->set.buckets.size == 0)
        return false;
    UnkKeyNode** bucket = self->set.buckets.At((u32)key % self->set.buckets.capacity);
    if (!bucket)
        return false;
    for (UnkKeyNode* n = *bucket; n; n = n->next) {
        if (n->key == key) {
            self->moved.PushBack(key);
            EraseChainedMapNodeByU64Key(&self->set, &key);
            return true;
        }
    }
    return false;
}
