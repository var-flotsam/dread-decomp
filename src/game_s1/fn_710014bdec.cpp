// Looks up a 64-bit key in a hash map at +0xb8 (base::global::CRntVector buckets, indexed by the
// key's low 32 bits modulo the bucket capacity, as in fn_710019e100): returns the found entry's
// flag, or true when the key isn't there. Also whether a key is in the hash set of
// fn_710019e100's owner (at +8, buckets at +0x18): fn_710019e4fc; and one at 0x108 hashing both
// key halves: fn_71001ef3f8.
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

struct UnkKeyNode {
    u64 key;
    u8 field_0x8[0x18];
    UnkKeyNode* next;
};

struct UnkKeySet {
    u8 field_0x0[0x10];
    UnkRntVector<UnkKeyNode*> buckets;  // +0x10
};

struct UnkKeySetOwner {
    u8 field_0x0[0x8];
    UnkKeySet set;  // +0x8
};

// Fold: the bucket hash is the key's two halves xor'd instead of its low half.
template <bool Fold = false>
__attribute__((always_inline)) static inline UnkKeyNode* Find(UnkKeySet* set, u64 key) {
    if (set->buckets.size == 0)
        return nullptr;
    u32 hash = Fold ? (u32)(key >> 32) ^ (u32)key : (u32)key;
    UnkKeyNode** bucket = set->buckets.At(hash % set->buckets.capacity);
    if (!bucket)
        return nullptr;
    for (UnkKeyNode* n = *bucket; n; n = n->next) {
        if (n->key == key)
            return n;
    }
    return nullptr;
}

bool fn_710019e4fc(UnkKeySetOwner* self, u64 key) { return Find(&self->set, key) != nullptr; }

// The same for a set at 0x108 with the folded hash and the key passed by address: fn_71001ef3f8.
struct UnkKeySetOwner108 {
    u8 field_0x0[0x108];
    UnkKeySet set;  // +0x108
};

bool fn_71001ef3f8(UnkKeySetOwner108* self, const u64* key) { return Find<true>(&self->set, *key) != nullptr; }
