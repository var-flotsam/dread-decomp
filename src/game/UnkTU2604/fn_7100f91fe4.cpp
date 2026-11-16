// Unit 2604: inserts an entry into a chained, linked hash map keyed by string id: the key id is copied (its count
// raised), the record's id is moved in and its three fields are copied. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkTU2604InsId {
    UnkStringData* data;

    UnkTU2604InsId(UnkTU2604InsId&& other) {
        data = other.data;
        other.data = &g_unk_7101d47108->empty;
    }
};

struct UnkTU2604InsRecord {
    UnkTU2604InsId id;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;

    UnkTU2604InsRecord(UnkTU2604InsRecord&& other) = default;
};

using UnkTU2604InsMap = base::global::CRntDictionary<CStrId, UnkTU2604InsRecord>;

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100f91fe4(UnkTU2604InsMap* map, const CStrId* key, UnkTU2604InsRecord* record) {
    map->Insert(*key, static_cast<UnkTU2604InsRecord&&>(*record));
}
