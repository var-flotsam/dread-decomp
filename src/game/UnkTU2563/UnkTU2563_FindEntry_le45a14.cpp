// Unit 2563 (CMinimapData hint): looks an entry up in a two-level flat map: the outer map (at +0x6000)
// by the first key, the inner map of that entry by the second; null when either is missing.
// Names and layouts are placeholders.
#include "types.h"

struct UnkInnerEntry_e45a14 {
    u8 pad_0x0[0x60];
};

struct UnkInnerMap_e45a14 {
    UnkInnerEntry_e45a14* values;  // 0x0
    u64* keys;                     // 0x8
    u32 count;                     // 0x10
    u8 pad_0x14[0x14];

    UnkInnerEntry_e45a14* Find(const u64& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};
static_assert(sizeof(UnkInnerMap_e45a14) == 0x28, "placeholder layout");

struct UnkOuterMap_e45a14 {
    UnkInnerMap_e45a14* values;  // 0x0
    u64* keys;                   // 0x8
    u32 count;                   // 0x10

    UnkInnerMap_e45a14* Find(const u64& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

struct UnkMapOwner_e45a14 {
    u8 pad_0x0[0x6000];
    UnkOuterMap_e45a14 map;  // 0x6000
};

UnkInnerEntry_e45a14* fn_7100e45a14(UnkMapOwner_e45a14* self, const u64* outer_key, u64 inner_key) {
    UnkInnerMap_e45a14* outer = self->map.Find(*outer_key);
    if (outer == self->map.values + self->map.count)
        return nullptr;
    UnkInnerEntry_e45a14* entry = outer->Find(inner_key);
    if (entry == outer->values + outer->count)
        return nullptr;
    return entry;
}
