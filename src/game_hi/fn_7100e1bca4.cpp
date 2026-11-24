// Constructs the billboard manager (src/game/UnkTU2560/UnkTU2560_BillboardMaps_ovb0218.cpp): three
// dictionaries (and a zeroed counter after the first), each starting with 11 empty buckets.
#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2560.h"
#include "types.h"

using base::global::CRntDictionary;
using base::global::CStrId;

// The values, which the nodes hold (their layouts are in the manager's file).
struct UnkBillboard_b218;
struct UnkDropList_b218;
struct UnkRecordList_b218;

// A dictionary that starts with 11 empty buckets.
template <class K, class V>
struct UnkDictionary11 : CRntDictionary<K, V> {
    __attribute__((always_inline)) UnkDictionary11() : CRntDictionary<K, V>(0xe) { this->ResizeBuckets(11); }
};

struct UnkMapSet {
    UnkDictionary11<u32, UnkBillboard_b218> billboards;
    u32 field_0x48;
    UnkDictionary11<CStrId, UnkDropList_b218> drops;
    UnkDictionary11<CStrId, UnkRecordList_b218> records;
    __attribute__((always_inline)) UnkMapSet() : field_0x48(0) {}
};

void ConstructBillboardManager(UnkMapSet* self) {
    new (self) UnkMapSet();
}
