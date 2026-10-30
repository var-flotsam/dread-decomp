// Lookups in parallel arrays of 8-byte values and keys (as in fn_71002519a8,
// src/game/CActorComponent/fn_71002519a8.cpp): the value stored for a key, or null (fn_71002eef6c), and the
// removals of a key's value through an out-of-line function when it is present (fn_7100251934
// with fn_71002332e0, fn_71002eeef8 with fn_71002ec674).

#include "base/UnkCloneCalls.h"
#include "types.h"

struct UnkKeyedValues_71002eef6c;
using UnkValuesByKey = base::UnkKeyedArray<UnkKeyedValues_71002eef6c, u64, u64>;

u64* fn_71002eef6c(UnkValuesByKey* a, const u64* key) {
    u64* p = a->Find(*key);
    return p == a->End() ? nullptr : p;
}

// Identical copies: fn_7100d86504, fn_710104ab4c, fn_71010c634c.
u64* fn_7100d86504(UnkValuesByKey* a, const u64* key) {
    u64* p = a->Find(*key);
    return p == a->End() ? nullptr : p;
}
u64* fn_710104ab4c(UnkValuesByKey* a, const u64* key) {
    u64* p = a->Find(*key);
    return p == a->End() ? nullptr : p;
}
u64* fn_71010c634c(UnkValuesByKey* a, const u64* key) {
    u64* p = a->Find(*key);
    return p == a->End() ? nullptr : p;
}

void fn_71002332e0(UnkValuesByKey* a, u64* value);
void fn_71002ec674(UnkValuesByKey* a, u64* value);

template <void (*Remove)(UnkValuesByKey*, u64*)>
__attribute__((always_inline)) static inline void RemoveKey(UnkValuesByKey* a, const u64* key) {
    u64* p = a->Find(*key);
    if (p != a->End())
        Remove(a, p);
}

void fn_7100251934(UnkValuesByKey* a, const u64* key) { RemoveKey<fn_71002332e0>(a, key); }

void fn_71002eeef8(UnkValuesByKey* a, const u64* key) { RemoveKey<fn_71002ec674>(a, key); }
