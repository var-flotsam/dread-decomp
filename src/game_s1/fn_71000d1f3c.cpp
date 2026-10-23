// Vector and iterator helpers of the reflection collection types for other element and index
// types: End for 2-byte, 1-byte and 12-byte elements (as in fn_7100b0d608: fn_71000d1f3c, fn_71000e1e84;
// fn_71000cc60c, fn_71001205d0), Insert of
// a 2-byte element (as in fn_7100b844f8: fn_71000d19c4, fn_71000d1b74), and by-value equality of
// cached iterator values with 64-bit indices (as in fn_7101070418: fn_71001f1c04, fn_71001f1ef4).
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "types.h"

using base::UnkCollectionIter;
using base::UnkIterValue;
using base::UnkVector;

struct UnkElem_0x2 {
    u8 data[2];
};

struct UnkElem_0xc {
    u32 words[3];
};

struct UnkWord64 {
    u64 value;

    bool operator==(const UnkWord64& other) const { return value == other.value; }
};

void fn_71000d1f3c(const UnkVector<u16>* v, UnkCollectionIter<u16>* out) { base::refl_End(v, out); }

void fn_71000e1e84(const UnkVector<u8>* v, UnkCollectionIter<u8>* out) { base::refl_End(v, out); }

void fn_71000cc60c(const UnkVector<UnkElem_0xc>* v, UnkCollectionIter<UnkElem_0xc>* out) { base::refl_End(v, out); }

void fn_71001205d0(const UnkVector<UnkElem_0xc>* v, UnkCollectionIter<UnkElem_0xc>* out) { base::refl_End(v, out); }

__attribute__((flatten)) void fn_71000d19c4(UnkVector<UnkElem_0x2>* v, UnkElem_0x2&& value, u32 index) {
    v->Insert(static_cast<UnkElem_0x2&&>(value), index);
}

__attribute__((flatten)) void fn_71000d1b74(UnkVector<UnkElem_0x2>* v, UnkElem_0x2&& value, u32 index) {
    v->Insert(static_cast<UnkElem_0x2&&>(value), index);
}

bool fn_71001f1c04(const UnkIterValue<UnkWord64, u64>& a, const UnkIterValue<UnkWord64, u64>& b) {
    return base::refl_IterValueEqualByValue(a, b);
}

bool fn_71001f1ef4(const UnkIterValue<UnkWord64, u64>& a, const UnkIterValue<UnkWord64, u64>& b) {
    return base::refl_IterValueEqualByValue(a, b);
}
