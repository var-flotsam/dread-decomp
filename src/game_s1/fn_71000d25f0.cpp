// Copies of generated helpers for other element types. Equality of two arrays (base::UnkVector's
// layout), element by element, as base::refl_VectorEqual (the generated fn_7100d42580) does for
// words: words (fn_71000d25f0), floats (fn_71000d4404) and bytes (fn_71000e17a8). Set of a value
// by key in a keyed array (u64 keys, u32 values), as base::refl_KeyedSet (the generated
// fn_7100251a80) does for words: fn_71000dee28, fn_71000dee9c.
#include "base/UnkCloneCalls.h"
#include "base/UnkVector.h"
#include "types.h"

template <class T>
__attribute__((always_inline)) static inline bool Equal(const base::UnkVector<T>* a, const base::UnkVector<T>* b) {
    if (a->count != b->count)
        return false;
    const T* pa = a->data;
    const T* pb = b->data;
    for (u64 n = a->count; n != 0; --n, ++pa, ++pb)
        if (*pb != *pa)
            return false;
    return true;
}

bool fn_71000d25f0(const base::UnkVector<u32>* a, const base::UnkVector<u32>* b) { return Equal(a, b); }

bool fn_71000d4404(const base::UnkVector<f32>* a, const base::UnkVector<f32>* b) { return Equal(a, b); }

bool fn_71000e17a8(const base::UnkVector<u8>* a, const base::UnkVector<u8>* b) { return Equal(a, b); }

struct UnkKeyedWords;
using UnkWordsByKey = base::UnkKeyedArray<UnkKeyedWords, u64, u32>;

__attribute__((always_inline)) static inline void Set(UnkWordsByKey* a, const u64* key, const u32* value) {
    u32* p = a->Find(*key);
    if (p != a->End())
        *p = *value;
}

void fn_71000dee28(UnkWordsByKey* a, const u64* key, const u32* value) { Set(a, key, value); }

void fn_71000dee9c(UnkWordsByKey* a, const u64* key, const u32* value) { Set(a, key, value); }
