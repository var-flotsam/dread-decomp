// Copies of fn_7100d94220 (src/game_aca57c/fn_7100d94220.cpp), which the binary
// repeats at other addresses: fn_71002f742c, fn_7100020898, fn_71000215f4, fn_7100022348, fn_7101025dd4.
// Copies the path ids of a hash map's values into a new list (returned): the list reserves the
// map's size, then each value's id is appended while the map is unchanged (its modification
// count still the one read at the start). fn_7100d94220 and its copies take the map at 8.
#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/UnkBucketSet.h"
#include "game/UnkPathNode.h"
#include "game/UnkPathValue.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkPathIdList_7100d94220 : base::global::CRntVector<CFilePathStrId> {
    explicit UnkPathIdList_7100d94220(u32 flags) : base::global::CRntVector<CFilePathStrId>(flags) {}

    UnkPathIdList_7100d94220(UnkPathIdList_7100d94220&&);  // elided (returned in place)
    void Reserve(u32 capacity);  // fn_7100018450

    __attribute__((always_inline)) void Append(const CFilePathStrId& id) {
        u32 n = count;
        u32 new_capacity = capacity;
        if (new_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        Reserve(new_capacity);
        // Copied bitwise, then the reference taken: count is reloaded after the store.
        CFilePathStrId* slot = &data[count];
        base::global::UnkStringData* value = id.data;
        __builtin_memcpy(slot, &value, sizeof(value));
        if (&g_unk_7101d47200->empty != value)
            ++value->ref_count;
        ++count;
    }
};

using UnkPathMap = base::global::UnkBucketSet<UnkPathNode>;

// Stops (node null) at the end or once the map has changed.
struct UnkPathMapIterator {
    UnkPathMap* map;
    UnkPathNode* node;
    u32 bucket;
    u32 modification;

    __attribute__((always_inline)) explicit UnkPathMapIterator(UnkPathMap* map)
        : map(map), node(map->first_node), bucket(map->first_bucket), modification(map->version) {}

    __attribute__((always_inline)) void Next() {
        if (map->version != modification) {
            node = nullptr;
            return;
        }
        node = node->next;
        if (!node) {
            while (!node) {
                if (++bucket >= map->buckets.count) {
                    node = nullptr;
                    break;
                }
                node = map->buckets.data[bucket];
            }
        }
    }
};

template <u32 Offset>
__attribute__((always_inline)) static inline UnkPathIdList_7100d94220 CopyIds(char* self) {
    UnkPathMap* map = reinterpret_cast<UnkPathMap*>(self + Offset);
    u32 size = map->node_count;
    UnkPathIdList_7100d94220 ids(0xf);
    ids.Reserve(size);
    for (UnkPathMapIterator it(map); it.node; it.Next())
        ids.Append(it.node->value->id);
    return ids;
}

UnkPathIdList_7100d94220 fn_71002f742c(char* self) { return CopyIds<8>(self); }

UnkPathIdList_7100d94220 fn_7100020898(char* self) { return CopyIds<8>(self); }

UnkPathIdList_7100d94220 fn_71000215f4(char* self) { return CopyIds<8>(self); }

UnkPathIdList_7100d94220 fn_7100022348(char* self) { return CopyIds<8>(self); }

UnkPathIdList_7100d94220 fn_7101025dd4(char* self) { return CopyIds<8>(self); }

