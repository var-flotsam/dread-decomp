// nn::atk unit 2772: finds the value an id maps to in a sound-archive table: the id's top byte picks one of six
// sections, whose entries each carry a valid flag and a value; answers -1 when there is none. Names and layouts
// are placeholders.
#include "types.h"

struct UnkAtkSection_l20ffc8 {
    u32 count;  // 0x0
    u32 pad_0x4;
    struct Slot {
        s32 offset;
        u32 pad_0x4;
    } slots[1];  // 0x8
};

struct UnkAtkTable_l20ffc8 {
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
    u8 pad_0x28[0x4];
    s32 kind4;  // 0x2c
};

struct UnkAtkEntry1_l20ffc8 {
    u8 pad_0x0[0x14];
    u8 valid;  // 0x14
    u8 pad_0x15[3];
    s32 value;  // 0x18
};

struct UnkAtkEntry2_l20ffc8 {
    u8 pad_0x0[0x18];
    u8 valid;  // 0x18
    u8 pad_0x19[3];
    s32 value;  // 0x1c
};

struct UnkAtkEntry3_l20ffc8 {
    u8 pad_0x0[0xc];
    u8 valid;  // 0xc
    u8 pad_0xd[3];
    s32 value;  // 0x10
};

struct UnkAtkEntry4_l20ffc8 {
    u8 pad_0x0[0x4];
    u8 valid;  // 0x4
    u8 pad_0x5[3];
    s32 value;  // 0x8
};

struct UnkAtkEntry5_l20ffc8 {
    u8 pad_0x0[0x8];
    u8 valid;  // 0x8
    u8 pad_0x9[3];
    s32 value;  // 0xc
};

template <typename Entry>
static inline Entry* Lookup(UnkAtkTable_l20ffc8* self, s32 section_offset, u32 id) {
    UnkAtkSection_l20ffc8* section =
        reinterpret_cast<UnkAtkSection_l20ffc8*>(reinterpret_cast<u8*>(self) + section_offset);
    u32 index = id & 0xffffff;
    if (!(index < section->count))
        return nullptr;
    return reinterpret_cast<Entry*>(reinterpret_cast<u8*>(section) + section->slots[index].offset);
}

s32 fn_710120ffc8(UnkAtkTable_l20ffc8* self, u32 id) {
    u32 kind = id >> 24;
    if (kind == 1) {
        UnkAtkEntry1_l20ffc8* e = Lookup<UnkAtkEntry1_l20ffc8>(self, self->kind1, id);
        if (e && (e->valid & 1))
            return e->value;
    }
    if (kind == 2) {
        UnkAtkEntry2_l20ffc8* e = Lookup<UnkAtkEntry2_l20ffc8>(self, self->kind2, id);
        if (e && (e->valid & 1))
            return e->value;
    }
    if (kind == 3) {
        UnkAtkEntry3_l20ffc8* e = Lookup<UnkAtkEntry3_l20ffc8>(self, self->kind3, id);
        if (e && (e->valid & 1))
            return e->value;
    }
    if (kind == 4) {
        UnkAtkEntry4_l20ffc8* e = Lookup<UnkAtkEntry4_l20ffc8>(self, self->kind4, id);
        if (e && (e->valid & 1))
            return e->value;
    }
    if (kind == 5) {
        UnkAtkEntry5_l20ffc8* e = Lookup<UnkAtkEntry5_l20ffc8>(self, self->kind5, id);
        if (e && (e->valid & 1))
            return e->value;
    }
    if (kind == 6) {
        UnkAtkEntry4_l20ffc8* e = Lookup<UnkAtkEntry4_l20ffc8>(self, self->kind6, id);
        if (e && (e->valid & 1))
            return e->value;
    }
    return -1;
}
