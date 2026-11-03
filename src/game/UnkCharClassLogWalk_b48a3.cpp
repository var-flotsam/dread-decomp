// Unit 2729 (tools/tu_map.py, no name hint), called by lua_bind::LogLoadedCharClasses: walks a
// chained hash map from an iterator position to its end. The logging the walk fed is compiled out, so
// nothing is stored. Names and layout are placeholders.
#include "types.h"

struct UnkHashNode_b48a3 {
    u8 pad_0x0[0x10];
    UnkHashNode_b48a3* next;
};

struct UnkHashIter_b48a3 {
    u8 pad_0x0[0x10];
    UnkHashNode_b48a3** buckets;
    u32 bucketCount;
    u8 pad_0x1c[0x38 - 0x1c];
    u32 bucket;
    UnkHashNode_b48a3* node;
};

struct UnkHashCursor_b48a3 {
    UnkHashIter_b48a3* map;
    u32 bucket;
    UnkHashNode_b48a3* node;
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

void fn_7101194438(UnkHashIter_b48a3* it) {
    UnkHashCursor_b48a3 cur = {it, it->bucket, it->node};
    for (; cur.node; cur.Next()) {
    }
}
