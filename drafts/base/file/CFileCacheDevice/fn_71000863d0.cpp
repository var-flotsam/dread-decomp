// Unit CFileCacheDevice: walks a chained hash map from its iterator position to the end, stopping if the
// map's version changes; the work the walk fed is compiled out. Names and layouts are placeholders.
#include "types.h"

struct UnkHashNode_l0863d0 {
    u8 pad_0x0[0x10];
    UnkHashNode_l0863d0* next;
};

struct UnkHashMap_l0863d0 {
    UnkHashNode_l0863d0** buckets;
    u32 bucketCount;
    u8 pad_0xc[0x28 - 0xc];
    u32 bucket;
    UnkHashNode_l0863d0* node;
    u8 pad_0x38[0x48 - 0x38];
    u32 version;
};

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
            if (++bucket >= map->bucketCount) {
                node = nullptr;
                return;
            }
            node = map->buckets[bucket];
        } while (!node);
    }
};

void fn_71000863d0(UnkHashMap_l0863d0* map) {
    UnkHashCursor_l0863d0 cur = {map, map->bucket, map->node, map ? map->version : 0};
    for (; cur.node; cur.Next()) {
        if (!cur.Valid())
            break;
    }
}
