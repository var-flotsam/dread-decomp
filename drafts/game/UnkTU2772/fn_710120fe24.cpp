// nn::atk unit 2772: finds the value for an id in a sound-archive table. The id's top byte picks one of the
// archive's sections; each section is a counted list of offsets to entries, and kind 2 refers to a kind 1 entry.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAtkSection_l20fe24 {
    u32 count;  // 0x0
    u32 pad_0x4;
    struct Slot {
        s32 offset;
        u32 pad_0x4;
    } slots[1];  // 0x8
};

struct UnkAtkTable_l20fe24 {
    u8 pad_0x0[0x4];
    s32 kind1;  // 0x4
    u8 pad_0x8[0x4];
    s32 kind2;  // 0xc
    u8 pad_0x10[0x4];
    s32 kind3;  // 0x14
    u8 pad_0x18[0x4];
    s32 kind5;  // 0x1c
    u8 pad_0x20[0x4];
    s32 kind6;  // 0x24
};

static inline u32* Lookup(UnkAtkTable_l20fe24* self, s32 section_offset, u32 index) {
    UnkAtkSection_l20fe24* section = reinterpret_cast<UnkAtkSection_l20fe24*>(reinterpret_cast<u8*>(self) + section_offset);
    if (index >= section->count)
        return nullptr;
    return reinterpret_cast<u32*>(reinterpret_cast<u8*>(section) + section->slots[index].offset);
}

s32 fn_710120fe24(UnkAtkTable_l20fe24* self, u32 id) {
    u32 index = id & 0xffffff;
    u32* entry;
    switch (id >> 24) {
    case 1:
        entry = Lookup(self, self->kind1, index);
        break;
    case 2: {
        u32* ref = Lookup(self, self->kind2, index);
        if (!ref || (*ref >> 24) != 1)
            return -1;
        entry = Lookup(self, self->kind1, *ref & 0xffffff);
        break;
    }
    case 3:
        entry = Lookup(self, self->kind3, index);
        break;
    case 5:
        entry = Lookup(self, self->kind5, index);
        break;
    case 6:
        entry = Lookup(self, self->kind6, index);
        break;
    default:
        return -1;
    }
    if (!entry)
        return -1;
    return *entry;
}
