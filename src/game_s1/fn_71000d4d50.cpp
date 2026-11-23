// Equality of cached iterator values (base::operator== on UnkIterValue, as in fn_71002b9400) for
// other index and element types: 0x18-byte elements compared by fn_71000a1ed4 (fn_71000d4d50,
// fn_71000d5074; with 64-bit keys fn_71000d6030, fn_71000d6548, fn_71000d70d8, fn_71000d75f0),
// 0x28-byte elements compared by fn_710024693c with 64-bit keys (fn_7100247830, fn_7100247f10),
// 64-bit keys with 8-byte elements (fn_71000d87c8, fn_71000d8dfc, fn_71000da1b0, fn_71000da828),
// and 0x30-byte elements compared by fn_710007f120 (fn_71000df9dc, fn_71000dfd00).
#include "base/UnkCollectionIter.h"
#include "base/UnkCompared_0x18.h"
#include "base/UnkCompared_0x30.h"
#include "base/reflection.h"
#include "game/UnkCompared_0x28b.h"
#include "game/UnkKey64.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::UnkIterValue;

inline bool operator==(const UnkCompared_0x18& a, const UnkCompared_0x18& b) { return fn_71000a1ed4(&a, &b); }

inline bool operator==(const UnkCompared_0x30& a, const UnkCompared_0x30& b) { return fn_710007f120(&a, &b); }

using UnkIterValue_0x18 = UnkIterValue<UnkCompared_0x18>;
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

bool fn_7100d883f0(const UnkCompared_0x28b* a, const UnkCompared_0x28b* b);
inline bool operator==(const UnkCompared_0x28b& a, const UnkCompared_0x28b& b) { return fn_7100d883f0(&a, &b); }
using UnkIterValueKey64_0x28b = UnkIterValue<UnkCompared_0x28b, UnkKey64>;

bool fn_7100d89094(const UnkIterValueKey64_0x28b& a, const UnkIterValueKey64_0x28b& b) { return a == b; }

bool fn_7100d89774(const UnkIterValueKey64_0x28b& a, const UnkIterValueKey64_0x28b& b) { return a == b; }

// 0x248-byte elements whose first 0x30 bytes are compared by fn_710007f120 (fn_7100e7afd0,
// fn_7100e7b35c), and 0x48-byte elements compared by fn_71011136b8 (fn_7101113398, fn_7101113980).
struct UnkCompared_0x248 {
    UnkCompared_0x30 head;
    u64 words[(0x248 - 0x30) / 8];
};
inline bool operator==(const UnkCompared_0x248& a, const UnkCompared_0x248& b) {
    return fn_710007f120(&a.head, &b.head);
}
struct UnkCompared_0x48 {
    u64 words[9];
};
inline bool operator==(const UnkCompared_0x48& a, const UnkCompared_0x48& b) { return fn_71011136b8(&a, &b); }

using UnkIterValue_0x248 = UnkIterValue<UnkCompared_0x248>;
using UnkIterValue_0x48 = UnkIterValue<UnkCompared_0x48>;

bool fn_7100e7afd0(const UnkIterValue_0x248& a, const UnkIterValue_0x248& b) { return a == b; }

bool fn_7100e7b35c(const UnkIterValue_0x248& a, const UnkIterValue_0x248& b) { return a == b; }

bool fn_7101113398(const UnkIterValue_0x48& a, const UnkIterValue_0x48& b) { return a == b; }

bool fn_7101113980(const UnkIterValue_0x48& a, const UnkIterValue_0x48& b) { return a == b; }
