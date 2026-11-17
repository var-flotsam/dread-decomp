// A reflected keyed array (8-byte values, CRntString keys compared out of line by fn_710007f120):
// its clear (CCollectionType +0x110), destructor (+0x40), find (+0x138), "set by key" slots (+0x140
// copy, +0x148 move, identical for these values), Begin (+0x160) and End (+0x168). Names and
// layouts are placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "../reflection/UnkTempIterValue_b44.h"
#include "base/global/CRntString.h"
#include "game/UnkTU57.h"
#include "types.h"

// As src/base/reflection/UnkReflArrayEqual.cpp declares it.
struct UnkCompared_0x30;

using UnkKey30_b44a1 = base::global::CRntString;

using UnkKeyed30Array_b44a1 = base::global::CRntSmallDictionary<UnkKey30_b44a1, u64>;

// The value under key, the keys compared by fn_710007f120.
static u64* Find(UnkKeyed30Array_b44a1* self, const UnkKey30_b44a1* key) {
    return self->FindIf([key](const UnkKey30_b44a1& k) {
        return fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&k),
                             reinterpret_cast<const UnkCompared_0x30*>(key));
    });
}

__attribute__((always_inline)) static inline void Set(UnkKeyed30Array_b44a1* self, const UnkKey30_b44a1* key,
                                                       const u64* value) {
    u64* p = Find(self, key);
    if (p != self->End())
        *p = *value;
}

void fn_7100138a4c(UnkKeyed30Array_b44a1* self, const UnkKey30_b44a1* key, const u64* value) { Set(self, key, value); }
void fn_7100138b04(UnkKeyed30Array_b44a1* self, const UnkKey30_b44a1* key, u64* value) { Set(self, key, value); }

u64* fn_71001389ac(UnkKeyed30Array_b44a1* self, const UnkKey30_b44a1* key) {
    u64* p = Find(self, key);
    return p == self->End() ? nullptr : p;
}

void fn_7100138718(UnkKeyed30Array_b44a1* self) { self->Clear(); }

// The destructor (+0x40): cleared, then the keys and the values freed.
void fn_7100138df8(UnkKeyed30Array_b44a1* self) {
    self->Clear();
    if (self->keys)
        self->allocator->vf_0x48(self->keys);
    if (self->values)
        self->allocator->vf_0x48(self->values);
}

using UnkKeyed30Iter_b44a1 = UnkTempIterValue<u64, UnkKey30_b44a1>;

void fn_71001391d4(UnkKeyed30Array_b44a1* self, UnkKeyed30Iter_b44a1* out) {
    *out = UnkKeyed30Iter_b44a1(self->keys, self->values);
}
void fn_7100139238(UnkKeyed30Array_b44a1* self, UnkKeyed30Iter_b44a1* out) {
    *out = UnkKeyed30Iter_b44a1(self->keys + self->count, self->values + self->count);
}
