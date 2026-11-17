// Unit 2604: inserts an entry into a chained, linked hash map keyed by string id: the key id is copied (its count
// raised), the record's id is copied too and its three fields with it. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkTU2604InsBRecord {
    CStrId id;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;

    UnkTU2604InsBRecord(const UnkTU2604InsBRecord& other) = default;
};

using UnkTU2604InsBMap = base::global::CRntDictionary<CStrId, UnkTU2604InsBRecord>;

void fn_7100f91ec0(UnkTU2604InsBMap* map, const CStrId* key, const UnkTU2604InsBRecord* record) {
    map->Insert(*key, *record);
}
