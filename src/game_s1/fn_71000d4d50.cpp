// Equality of cached iterator values (base::operator== on UnkIterValue, as in fn_71002b9400) for
// other index and element types: 0x18-byte elements compared by fn_71000a1ed4 (fn_71000d4d50,
// fn_71000d5074; with 64-bit keys fn_71000d6030, fn_71000d6548, fn_71000d70d8, fn_71000d75f0),
// 0x28-byte elements compared by fn_710024693c with 64-bit keys (fn_7100247830, fn_7100247f10),
// 64-bit keys with 8-byte elements (fn_71000d87c8, fn_71000d8dfc, fn_71000da1b0, fn_71000da828),
// and 0x30-byte elements compared by fn_710007f120 (fn_71000df9dc, fn_71000dfd00).
#include "base/UnkCollectionIter.h"
#include "types.h"

using base::UnkIterValue;

struct UnkCompared_0x18 {
    u64 words[3];
};
bool fn_71000a1ed4(const UnkCompared_0x18* a, const UnkCompared_0x18* b);
inline bool operator==(const UnkCompared_0x18& a, const UnkCompared_0x18& b) { return fn_71000a1ed4(&a, &b); }

struct UnkCompared_0x30 {
    u64 words[6];
};
bool fn_710007f120(const UnkCompared_0x30* a, const UnkCompared_0x30* b);
inline bool operator==(const UnkCompared_0x30& a, const UnkCompared_0x30& b) { return fn_710007f120(&a, &b); }

using UnkIterValue_0x18 = UnkIterValue<UnkCompared_0x18>;
// A 64-bit key compared through its own operator (a plain u64 index loads it earlier).
struct UnkKey64 {
    u64 value;
    bool operator!=(const UnkKey64& o) const { return value != o.value; }
};
using UnkIterValueU64 = UnkIterValue<u64, UnkKey64>;
using UnkIterValue_0x30 = UnkIterValue<UnkCompared_0x30>;

bool fn_71000d4d50(const UnkIterValue_0x18& a, const UnkIterValue_0x18& b) { return a == b; }

bool fn_71000d5074(const UnkIterValue_0x18& a, const UnkIterValue_0x18& b) { return a == b; }

bool fn_71000d87c8(const UnkIterValueU64& a, const UnkIterValueU64& b) { return a == b; }

bool fn_71000d8dfc(const UnkIterValueU64& a, const UnkIterValueU64& b) { return a == b; }

bool fn_71000da1b0(const UnkIterValueU64& a, const UnkIterValueU64& b) { return a == b; }

bool fn_71000da828(const UnkIterValueU64& a, const UnkIterValueU64& b) { return a == b; }

bool fn_71000df9dc(const UnkIterValue_0x30& a, const UnkIterValue_0x30& b) { return a == b; }

bool fn_71000dfd00(const UnkIterValue_0x30& a, const UnkIterValue_0x30& b) { return a == b; }

struct UnkCompared_0x28 {
    u64 words[5];
};
bool fn_710024693c(const UnkCompared_0x28* a, const UnkCompared_0x28* b);
inline bool operator==(const UnkCompared_0x28& a, const UnkCompared_0x28& b) { return fn_710024693c(&a, &b); }

using UnkIterValueKey64_0x18 = UnkIterValue<UnkCompared_0x18, UnkKey64>;
using UnkIterValueKey64_0x28 = UnkIterValue<UnkCompared_0x28, UnkKey64>;

bool fn_71000d6030(const UnkIterValueKey64_0x18& a, const UnkIterValueKey64_0x18& b) { return a == b; }

bool fn_71000d6548(const UnkIterValueKey64_0x18& a, const UnkIterValueKey64_0x18& b) { return a == b; }

bool fn_71000d70d8(const UnkIterValueKey64_0x18& a, const UnkIterValueKey64_0x18& b) { return a == b; }

bool fn_71000d75f0(const UnkIterValueKey64_0x18& a, const UnkIterValueKey64_0x18& b) { return a == b; }

bool fn_7100247830(const UnkIterValueKey64_0x28& a, const UnkIterValueKey64_0x28& b) { return a == b; }

bool fn_7100247f10(const UnkIterValueKey64_0x28& a, const UnkIterValueKey64_0x28& b) { return a == b; }
