// Unit 2666 (lua_bind): walks the actor definition factory's chained hash map from its iterator
// position to the end; the logging the walk fed is compiled out. Names and layouts are placeholders.
#include "game/UnkTU344.h"
#include "types.h"

struct UnkHashNode_l07ef50 {
    u8 pad_0x0[0x10];
    UnkHashNode_l07ef50* next;
};

struct UnkActorDefFactory_l07ef50 {
    u8 pad_0x0[0x10];
    UnkHashNode_l07ef50** buckets;
    u32 bucketCount;
    u8 pad_0x1c[0x38 - 0x1c];
    u32 bucket;
    UnkHashNode_l07ef50* node;
};

struct UnkHashCursor_l07ef50 {
    UnkActorDefFactory_l07ef50* map;
    u32 bucket;
    UnkHashNode_l07ef50* node;
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

u32 fn_710107ef50() {
    UnkActorDefFactory_l07ef50* factory = reinterpret_cast<UnkActorDefFactory_l07ef50*>(g_unk_71726a0220);
    UnkHashCursor_l07ef50 cur = {factory, factory->bucket, factory->node};
    for (; cur.node; cur.Next()) {
    }
    return 0;
}
