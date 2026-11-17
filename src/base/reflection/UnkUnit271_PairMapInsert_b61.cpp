// Unit 271 of tools/tu_map.py (base::reflection): collection slot 0x118 of two reflected chained hash
// maps keyed by a pair of u32 (the bucket is their xor modulo the bucket capacity), fn_71001f2148 and
// fn_71001f2244: a node with the key and an 8-byte value goes at the head of its bucket chain and the
// tail of the list of all nodes (base::global::CRntDictionary::InsertChecked).
// Names and layouts are placeholders.
#include "base/global/CRntDictionary.h"
#include "types.h"

struct UnkPairKey_b61 {
    u32 a;
    u32 b;
};

// The bucket of a pair key is its halves xored.
template <>
struct base::global::CRntDictionaryHash<UnkPairKey_b61> {
    static u32 Of(const UnkPairKey_b61& key) { return key.a ^ key.b; }
};

using UnkPairMap_b61 = base::global::CRntDictionary<UnkPairKey_b61, u64>;

void fn_71001f2148(UnkPairMap_b61* self, const UnkPairKey_b61* key, const u64* value) { self->InsertChecked(*key, *value); }
void fn_71001f2244(UnkPairMap_b61* self, const UnkPairKey_b61* key, const u64* value) { self->InsertChecked(*key, *value); }
