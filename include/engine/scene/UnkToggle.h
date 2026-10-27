#pragma once

// A scene item with three state flags (0xf6, 0xf8, and 0xf9, which gates the other two) that
// reports its effective state to the owner at its +8; the owner forwards each report to the
// delegate at its own +8, whose virtual slots 0x40..0x58 receive it (fn_71001eef58 and
// fn_71001eef74 below, in engine::scene::CScene's unit). Setting the flag at 0xf9 also recopies
// a style block (0x88, 0x98..0xcd) from the item's description at +0x108 and passes it to the
// item's virtual slot 0x30. Names and the layout are placeholders inferred from the callers
// (src/game_s1/fn_71001f3224.cpp and its siblings); only the members they share are spelled out.

#include "types.h"

struct UnkToggle;

// The description an item copies its style block from; its flag at 0xe0 mirrors the item's at
// 0xf9 (fn_710020c8ac).
struct UnkToggleSrc {
    u8 pad_0x0[0x28];
    u32 f_0x28;
    u8 pad_0x2c[0xc];
    u32 f_0x38, f_0x3c, f_0x40, f_0x44, f_0x48, f_0x4c, f_0x50;
    u8 pad_0x54[4];
    u64 f_0x58;
    u32 f_0x60, f_0x64, f_0x68;
    u8 f_0x6c, f_0x6d;
    u8 pad_0x6e[0x72];
    u8 flag_0xe0;
};
static_assert(sizeof(UnkToggleSrc) == 0xe8, "UnkToggleSrc placeholder layout");

// The object the owner forwards an item's reports to: one virtual for "on", the next for "off".
struct UnkToggleDelegate;
struct UnkToggleDelegateVt {
    void* pad_0x0[8];
    void (*vf_0x40)(UnkToggleDelegate* self, UnkToggle* item);  // flag 0xf8 effectively on
    void (*vf_0x48)(UnkToggleDelegate* self, UnkToggle* item);  // flag 0xf8 effectively off
    void (*vf_0x50)(UnkToggleDelegate* self, UnkToggle* item);  // flag 0xf6 effectively on
    void (*vf_0x58)(UnkToggleDelegate* self, UnkToggle* item);  // flag 0xf6 effectively off
};
struct UnkToggleDelegate {
    UnkToggleDelegateVt* vtable_0x0;
};

// An item's owner, as the two notifiers see it: they only read its delegate at +8.
struct UnkToggleListener {
    void* field_0x0;
    UnkToggleDelegate* delegate_0x8;
};

struct UnkToggleVt {
    void* slot[6];
    void (*slot_0x30)(UnkToggle* self, void* style);  // receives the recopied style block (&f_0x98)
};

struct UnkToggle {
    UnkToggleVt* vtable_0x0;
    UnkToggleListener* listener_0x8;
    u8 pad_0x10[0x78];
    u32 f_0x88;
    u8 pad_0x8c[0xc];
    u32 f_0x98, f_0x9c, f_0xa0, f_0xa4, f_0xa8, f_0xac, f_0xb0;  // the style block, from the
    u8 pad_0xb4[4];                                               // description's 0x38..0x6d
    u64 f_0xb8;
    u32 f_0xc0, f_0xc4, f_0xc8;
    u8 f_0xcc, f_0xcd;
    u8 pad_0xce[0x28];
    u8 flag_0xf6;
    u8 pad_0xf7;
    u8 flag_0xf8;
    u8 flag_0xf9;
    u8 pad_0xfa;
    u8 flag_0xfb;  // pending update: set once the view the item shows is moved or resized
    u8 pad_0xfc[0xc];
    UnkToggleSrc* src_0x108;
};
static_assert(sizeof(UnkToggle) == 0x110, "UnkToggle placeholder layout");

// Report `item`'s effective state to `owner`'s delegate: fn_71001eef58 the state of the flag at
// 0xf8 (vf_0x40 when on, vf_0x48 when off), fn_71001eef74 that of the flag at 0xf6 (vf_0x50,
// vf_0x58). The callers pass the flag gated by the one at 0xf9. Defined in
// src/engine/scene/CScene.cpp.
void fn_71001eef58(UnkToggleListener* owner, UnkToggle* item, bool on);
void fn_71001eef74(UnkToggleListener* owner, UnkToggle* item, bool on);

// Set `item`'s flag at 0xf9 to `on`; when it changes, report the flags at 0xf8 and 0xf6, gated by
// it, to the owner (fn_71001eef58, fn_71001eef74). Defined in src/game_s1/fn_710020ac34.cpp.
void fn_710020ac34(UnkToggle* item, bool on);
