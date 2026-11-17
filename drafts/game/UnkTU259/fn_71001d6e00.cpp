// Unit 259 (the graphics system): adds a named vector under a new key; fails when the key is
// already in the table. Names and layouts are placeholders.
#include <new>

#include "base/UnkParallelFind.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkGfxNamedVectors;
void fn_71001d6c58(UnkGfxNamedVectors* self, u32 capacity);

// A string id that starts as the pool's empty string.
struct UnkEmptyStrId_l1d6e00 {
    base::global::UnkStringData* data;

    UnkEmptyStrId_l1d6e00() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    // Takes a reference to `id` (CStrId's copy assignment).
    void Assign(const CStrId& id) { *reinterpret_cast<CStrId*>(this) = id; }
};

struct UnkNamedVector_l1d6e00 {
    UnkEmptyStrId_l1d6e00 name;
    UnkVector4D value;  // 0x8
};

// Named vectors in an array parallel to their 64-bit keys.
struct UnkNamedVectors_l1d6e00 {
    UnkNamedVector_l1d6e00* values;
    u64* keys;  // 0x8
    u32 count;  // 0x10
    u32 capacity;
    u32 step;

    UnkNamedVector_l1d6e00* End() { return &values[count]; }
};

struct UnkGfxSystem_l1d6e00 {
    void** vtable;
    UnkNamedVectors_l1d6e00 named_vectors;  // 0x8
};

bool fn_71001d6e00(UnkGfxSystem_l1d6e00* self, const u64* key, const CStrId* name, const CVector4D* value) {
    UnkNamedVectors_l1d6e00& t = self->named_vectors;
    if (UnkParallelFindSlotByValue(&t, *key) != t.End())
        return false;
    if (t.count == t.capacity)
        fn_71001d6c58(reinterpret_cast<UnkGfxNamedVectors*>(&t), t.count + t.step);
    t.keys[t.count] = *key;
    new (&t.values[t.count]) UnkNamedVector_l1d6e00();
    u32 count = ++t.count;
    UnkNamedVector_l1d6e00* end = &t.values[count];
    UnkNamedVector_l1d6e00& entry = count ? end[-1] : *end;
    entry.name.Assign(*name);
    Assign(entry.value, *value);
    return true;
}
