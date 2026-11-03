// gui/scripts/guimanager.lua unit: the object registered under `id` in the hash map behind +0x1b0
// (buckets at +0x10, count and capacity after them; a node's key object holds the id at +0x30), or
// null. The same lookup as the game manager's (src/game/CGameManager.cpp). Names and layouts are
// placeholders.
#include "types.h"

struct UnkGuiMapKey {
    u8 pad_0x0[0x30];
    u64 id;
};
struct UnkGuiMapNode {
    UnkGuiMapKey* key;
    void* value;
    u8 pad_0x10[0x10];
    UnkGuiMapNode* next;  // 0x20
};
struct UnkGuiMap {
    u8 pad_0x0[0x10];
    UnkGuiMapNode** buckets;
    u32 count;
    u32 capacity;
};
struct UnkGuiMapOwner {
    u8 pad_0x0[0x1b0];
    UnkGuiMap* map;
};

void* fn_71011ba9dc(UnkGuiMapOwner* self, u64 id) {
    UnkGuiMap* map = self->map;
    if (map->count) {
        u32 index = ((u32)(id >> 32) ^ (u32)id) % map->capacity;
        UnkGuiMapNode** bucket = map->buckets + index;
        if (bucket) {
            for (UnkGuiMapNode* n = *bucket; n; n = n->next) {
                if (n->key->id == id)
                    return n->value;
            }
        }
    }
    return nullptr;
}
