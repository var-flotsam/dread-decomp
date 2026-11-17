// Unit 2560 (the billboard manager): inserts a node into the chained, linked hash map of drop-request lists
// keyed by string id: the key id is copied (its count raised) and the list of up to 16 requests copied with
// it. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkDrop_le1d620 {
    CStrId id;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    bool field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    f32 field_0x20;

    UnkDrop_le1d620(const UnkDrop_le1d620& other) = default;
};
static_assert(sizeof(UnkDrop_le1d620) == 0x28, "drop layout");

#pragma pack(push, 4)
// A fixed-capacity list: the requests are built in place as they are added.
struct UnkDropList_le1d620 {
    u32 count;
    u8 storage[16 * sizeof(UnkDrop_le1d620)];

    UnkDrop_le1d620* items() { return reinterpret_cast<UnkDrop_le1d620*>(storage); }
    const UnkDrop_le1d620* items() const { return reinterpret_cast<const UnkDrop_le1d620*>(storage); }

    UnkDropList_le1d620(const UnkDropList_le1d620& other) {
        for (u64 i = 0; i < other.count; ++i)
            new (&items()[i]) UnkDrop_le1d620(other.items()[i]);
        count = other.count;
    }
};
#pragma pack(pop)

using UnkDropMap_le1d620 = base::global::CRntDictionary<CStrId, UnkDropList_le1d620>;
static_assert(sizeof(UnkDropMap_le1d620::TNode) == 0x2a8, "node layout");

UnkDropMap_le1d620::TNode* fn_7100e1d620(UnkDropMap_le1d620* map, const CStrId* key,
                                         const UnkDropList_le1d620* list) {
    return map->Insert(*key, *list);
}
