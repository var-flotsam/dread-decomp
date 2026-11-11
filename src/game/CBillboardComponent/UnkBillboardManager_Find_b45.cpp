// The billboard manager's lookup by id (as src/game/CBillboardComponent/CBillboardComponent_b39.cpp
// declares it): a hash map of nodes in buckets (id modulo the bucket count), each node holding its
// id, the entry at +8 and the next node at +0x68. None when the map is empty or the id is absent.
// Names and layouts are placeholders.
#include "game/UnkTU2560.h"
#include "types.h"

// As src/game/CBillboardComponent/CBillboardComponent_b39.cpp declares it.
struct UnkBillboardEntry_bbc;

struct UnkBillboardNode_b45a3 {
    u32 id;
    u8 pad_0x4[0x4];
    u8 entry[0x60];  // 0x8
    UnkBillboardNode_b45a3* next;  // 0x68
};

struct UnkBillboardMap_b45a3 {
    u8 pad_0x0[0x10];
    UnkBillboardNode_b45a3** buckets;  // 0x10
    u32 count;                         // 0x18
    u32 bucket_count;                  // 0x1c
};

UnkBillboardEntry_bbc* fn_7100e1caf0(void* manager, u32 id) {
    UnkBillboardMap_b45a3* map = static_cast<UnkBillboardMap_b45a3*>(manager);
    UnkBillboardNode_b45a3* node = nullptr;
    if (map->count) {
        u32 index = id % map->bucket_count;
        UnkBillboardNode_b45a3** bucket = &map->buckets[index];
        if (bucket)
            for (node = *bucket; node && node->id != id; node = node->next) {
            }
    }
    return node ? reinterpret_cast<UnkBillboardEntry_bbc*>(node->entry) : nullptr;
}
