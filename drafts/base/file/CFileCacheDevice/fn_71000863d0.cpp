// Unit CFileCacheDevice: walks a chained hash map from its iterator position to the end, stopping if the
// map's version changes; the work the walk fed is compiled out. Names and layouts are placeholders.
#include "base/global/UnkBucketSet.h"
#include "types.h"

struct UnkHashNode_l0863d0 {
    u8 pad_0x0[0x10];
    UnkHashNode_l0863d0* next;
};

using UnkHashMap_l0863d0 = base::global::UnkBucketSet<UnkHashNode_l0863d0>;

struct UnkHashCursor_l0863d0 {
    UnkHashMap_l0863d0* map;
    u32 bucket;
    UnkHashNode_l0863d0* node;
    u32 version;
    bool Valid() const { return map && map->version == version; }
    void Next() {
        node = node->next;
        if (node)
            return;
        do {
            if (++bucket >= map->buckets.count) {
                node = nullptr;
                return;
            }
            node = map->buckets.data[bucket];
        } while (!node);
    }
};

void fn_71000863d0(UnkHashMap_l0863d0* map) {
    UnkHashCursor_l0863d0 cur = {map, map->first_bucket, map->first_node, map ? map->version : 0};
    for (; cur.node; cur.Next()) {
        if (!cur.Valid())
            break;
    }
}
