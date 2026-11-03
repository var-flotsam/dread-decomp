// Unit 270 (tools/tu_map.py), beside src/game/UnkSpherePool.cpp: sets the word +0xdc of a pool entry
// when `entry` lies inside the pool's 25000 entries (from +0x30d48, 0xe0 bytes each) and the pool's
// word at +0x5e9b74 is set. Names and layouts are placeholders.
#include "types.h"

struct UnkSpherePoolTagEntry {
    u8 pad_0x0[0xdc];
    s32 tag;  // 0xdc
};
static_assert(sizeof(UnkSpherePoolTagEntry) == 0xe0, "pool entry stride");

struct UnkSpherePoolTags {
    u8 pad_0x0[0x30d48];
    UnkSpherePoolTagEntry entries[25000];  // 0x30d48
    u8 pad_0x588048[0x5e9b74 - 0x588048];
    u32 active;  // 0x5e9b74
};

void fn_71001ebb34(UnkSpherePoolTags* self, UnkSpherePoolTagEntry* entry, s32 tag) {
    if (entry == nullptr) {
        return;
    }
    asm("" : "+r"(entry));
    if (self->entries > entry) {
        return;
    }
    if (self->entries + 25000 <= entry) {
        return;
    }
    if (self->active != 0) {
        entry->tag = tag;
    }
}
