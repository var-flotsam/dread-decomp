#pragma once

// The graphics system's render target (unit 259), as far as the hand-written sources read it: the
// per-pass entry list from +0x128, the object list from +0xc0140, the current pass and mode, and
// the flags its passes set. fn_71001cde84 runs a pass over it. Names and the layout are
// placeholders.

#include "types.h"

struct UnkPassEntryA1 {
    u8 pad_0x0[0x10];
    u32 pass_mask;  // 0x10
    u8 pad_0x14[0x40 - 0x14];
    void (*callback)(UnkPassEntryA1* self);  // 0x40
    u8 pad_0x48[0x8];
};

struct UnkPassSubA1 {
    u8 pad_0x0[0x30];
    u8 field_0x30;
    u8 pad_0x31[0xa9 - 0x31];
    u8 field_0xa9;
    u8 pad_0xaa[0xb0 - 0xaa];
    u8 field_0xb0;
};

struct UnkPassObjectA1 {
    u8 pad_0x0[0x20];
    UnkPassSubA1* sub;  // 0x20
    u8 pad_0x28[0x18];
    void (*callback)(UnkPassObjectA1* self);  // 0x40
};

struct UnkRenderTargetA1 {
    u8 pad_0x0[0x128];
    UnkPassEntryA1 entries[0x2000];  // 0x128
    u32 entry_count;                       // 0xa0128
    u8 pad_0xa012c[0xb0130 - 0xa012c];
    u32 field_0xb0130;
    u8 pad_0xb0134[0xc0138 - 0xb0134];
    u32 field_0xc0138;
    u8 pad_0xc013c[0x4];
    UnkPassObjectA1* objects[0x2000];  // 0xc0140
    u32 object_count;                        // 0xd0140
    u8 pad_0xd0144[0x1948168 - 0xd0144];
    u32 field_0x1948168;
    u8 pad_0x194816c[0x3178170 - 0x194816c];
    u32 field_0x3178170;
    u8 pad_0x3178174[0x317a978 - 0x3178174];
    u32 field_0x317a978;
    u32 field_0x317a97c;
    u32 pass_kind;  // 0x317a980
    u8 pad_0x317a984[0x317a998 - 0x317a984];
    u32 pass;  // 0x317a998
    u32 mode;  // 0x317a99c
    u8 pad_0x317a9a0[0x317aa68 - 0x317a9a0];
    u32 field_0x317aa68;  // a counter fn_71001cb51c restarts at 20
    u8 pad_0x317aa6c[0x317acb8 - 0x317aa6c];
    u64 field_0x317acb8;
    u64 field_0x317acc0;
    u8 pad_0x317acc8[0x3183668 - 0x317acc8];
    bool field_0x3183668;  // flipped by fn_71001cb51c
    u8 pad_0x3183669[0x3183672 - 0x3183669];
    u8 field_0x3183672;
};

static_assert(__builtin_offsetof(UnkRenderTargetA1, entry_count) == 0xa0128, "UnkRenderTargetA1 layout");
static_assert(__builtin_offsetof(UnkRenderTargetA1, object_count) == 0xd0140, "UnkRenderTargetA1 layout");
static_assert(__builtin_offsetof(UnkRenderTargetA1, field_0x3183668) == 0x3183668, "UnkRenderTargetA1 layout");
