// Units 204 and 259 of tools/tu_map.py (no class hint): adds a node to a chained hash map keyed by a string id
// (the bucket is the string's 64-bit hash at +0x30, halves xored, modulo the bucket capacity): the
// node holds a copy of the id and an 8-byte (unit 204) or 4-byte (unit 259) value, goes at the head of its bucket chain and the tail
// of the list of all nodes (the layout of src/base/reflection/UnkUnit271_PairMapInsert_b61.cpp).
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// Out-of-line inserts of a dictionary by string id.
void fn_710016a1cc(base::global::CRntDictionary<CStrId, u64>* self, const CStrId* key, const u64* value) {
    self->InsertChecked(*key, *value);
}

void fn_71001d7cf0(base::global::CRntDictionary<CStrId, u32>* self, const CStrId* key, const u32* value) {
    self->InsertChecked(*key, *value);
}
