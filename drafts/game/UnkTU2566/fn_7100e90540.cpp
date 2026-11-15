// Unit 2566 (the minimap manager): advances every item of the item table (a chained hash table walked from
// its cursor, stopping when the table changes under the walk), then the transitions of two arrays of
// records and three single ones, by `dt`. Names and layouts are placeholders.
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
    UnkMinimapItemNode** buckets;  // 0x0
    u32 bucket_count;              // 0x8
    u8 pad_0xc[0x1c];
    u32 cursor_bucket;             // 0x28
    UnkMinimapItemNode* cursor;    // 0x30
    u8 pad_0x38[0x10];
    u32 version;                   // 0x48
    u8 pad_0x4c[0x10c];
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
    UnkMinimapItemNode* node = table.cursor;
    u32 bucket = table.cursor_bucket;
    u32 version = table.version;
    while (node) {
        for (UnkMinimapItemSlot& slot : *node)
            fn_7100eb8978(reinterpret_cast<UnkMinimapItem*>(&slot), dt);
        if (table.version != version)
            break;
        node = node->next;
        while (!node && ++bucket < table.bucket_count)
            node = table.buckets[bucket];
    }
    for (u32 i = 0; i < table.b.count; ++i)
        fn_7100eb7470(table.b.data[i].transition, dt);
    for (u32 i = 0; i < table.a.count; ++i)
        fn_7100eb7470(table.a.data[i].transition, dt);
    fn_7100eb7470(reinterpret_cast<u8*>(self) + 0xd64470, dt);
    fn_7100eb7470(reinterpret_cast<u8*>(self) + 0xd64548, dt);
    fn_7100eb7470(reinterpret_cast<u8*>(self) + 0xd64670, dt);
}
