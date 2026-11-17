// Unit 2566 (the minimap manager): advances every item of the item table (a chained hash table walked from
// its cursor, stopping when the table changes under the walk), then the transitions of two arrays of
// records and three single ones, by `dt`. Names and layouts are placeholders.
#include "base/global/UnkBucketSet.h"
#include "game/UnkTU2567.h"
#include "types.h"

struct UnkMinimapItem;
void fn_7100eb8978(UnkMinimapItem* item, f32 dt);

struct UnkMinimapItemSlot {
    u8 pad_0x0[0x578];
};

struct UnkMinimapItemNode {
    u8 pad_0x0[8];
    u32 count;
    UnkMinimapItemSlot slots[4];
    u8 pad_0x15ec[0xc];
    UnkMinimapItemNode* next;

    UnkMinimapItemSlot* begin() { return slots; }
    UnkMinimapItemSlot* end() { return slots + count; }
};
static_assert(__builtin_offsetof(UnkMinimapItemNode, next) == 0x15f8, "UnkMinimapItemNode layout");

struct UnkMinimapTransitionRecord {
    u8 pad_0x0[0x40];
    u8 transition[0xd8];
};
static_assert(sizeof(UnkMinimapTransitionRecord) == 0x118, "UnkMinimapTransitionRecord layout");

struct UnkMinimapTransitionArray {
    UnkMinimapTransitionRecord* data;
    u8 pad_0x8[8];
    u32 count;
};

struct UnkMinimapItemTable {
    // The pooled set fn_7100eb2cc0 sets up (base/file/UnkFileCacheSet.h, nodes of 0x1600 bytes).
    base::global::UnkPooledBucketSet<UnkMinimapItemNode> items;  // 0x0
    u8 pad_0x90[0xc8];
    UnkMinimapTransitionArray a;   // 0x158
    u8 pad_0x170[0x70];
    UnkMinimapTransitionArray b;   // 0x1e0
};
static_assert(__builtin_offsetof(UnkMinimapItemTable, b) == 0x1e0, "UnkMinimapItemTable layout");

struct UnkMinimapManager {
    u8 pad_0x0[0xd64210];
    UnkMinimapItemTable table;
};

void fn_7100e90540(UnkMinimapManager* self, f32 dt) {
    UnkMinimapItemTable& table = self->table;
    UnkMinimapItemNode* node = table.items.first_node;
    u32 bucket = table.items.first_bucket;
    u32 version = table.items.version;
    while (node) {
        for (UnkMinimapItemSlot& slot : *node)
            fn_7100eb8978(reinterpret_cast<UnkMinimapItem*>(&slot), dt);
        if (table.items.version != version)
            break;
        node = node->next;
        while (!node && ++bucket < table.items.buckets.count)
            node = table.items.buckets.data[bucket];
    }
    for (u32 i = 0; i < table.b.count; ++i)
        fn_7100eb7470(table.b.data[i].transition, dt);
    for (u32 i = 0; i < table.a.count; ++i)
        fn_7100eb7470(table.a.data[i].transition, dt);
    fn_7100eb7470(reinterpret_cast<u8*>(self) + 0xd64470, dt);
    fn_7100eb7470(reinterpret_cast<u8*>(self) + 0xd64548, dt);
    fn_7100eb7470(reinterpret_cast<u8*>(self) + 0xd64670, dt);
}
