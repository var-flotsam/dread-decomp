// Copies the path ids of a hash map's values into a new list (returned): the list reserves the
// map's size, then each value's id is appended while the map is unchanged (its modification
// count still the one read at the start). fn_7100d94220 and its copies take the map at 8.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkPathIdList_7100d94220 : base::UnkVector<CFilePathStrId> {
    explicit UnkPathIdList_7100d94220(u32 flags) : base::UnkVector<CFilePathStrId>(flags) {}

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

struct UnkPathValue {
    u8 unk_0x0[0x10];
    CFilePathStrId id;
};

struct UnkPathNode {
    void* key;
    UnkPathValue* value;
    UnkPathNode* next;
};

struct UnkPathMap {
    UnkPathNode** buckets;
    u32 bucket_count;
    u8 unk_0xc[0x1c];
    u32 first_bucket;
    UnkPathNode* first;
    u8 unk_0x38[0xc];
    u32 size;
    u32 modification;
};

// Stops (node null) at the end or once the map has changed.
struct UnkPathMapIterator {
    UnkPathMap* map;
    UnkPathNode* node;
    u32 bucket;
    u32 modification;

    __attribute__((always_inline)) explicit UnkPathMapIterator(UnkPathMap* map)
        : map(map), node(map->first), bucket(map->first_bucket), modification(map->modification) {}

    __attribute__((always_inline)) void Next() {
        if (map->modification != modification) {
            node = nullptr;
            return;
        }
        node = node->next;
        if (!node) {
            while (!node) {
                if (++bucket >= map->bucket_count) {
                    node = nullptr;
                    break;
                }
                node = map->buckets[bucket];
            }
        }
    }
};

template <u32 Offset>
__attribute__((always_inline)) static inline UnkPathIdList_7100d94220 CopyIds(char* self) {
    UnkPathMap* map = reinterpret_cast<UnkPathMap*>(self + Offset);
    u32 size = map->size;
    UnkPathIdList_7100d94220 ids(0xf);
    ids.Reserve(size);
    for (UnkPathMapIterator it(map); it.node; it.Next())
        ids.Append(it.node->value->id);
    return ids;
}

UnkPathIdList_7100d94220 fn_7100d94220(char* self) { return CopyIds<8>(self); }
UnkPathIdList_7100d94220 fn_7100d94f74(char* self) { return CopyIds<8>(self); }
UnkPathIdList_7100d94220 fn_7100d95cc8(char* self) { return CopyIds<8>(self); }
UnkPathIdList_7100d94220 fn_7100db2c28(char* self) { return CopyIds<8>(self); }
// A copy in another unit, with the map 8 bytes further on.
UnkPathIdList_7100d94220 fn_7101193774(char* self) { return CopyIds<0x10>(self); }
