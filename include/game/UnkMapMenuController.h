#pragma once

// The map menu (most likely GUI::CMapMenuController: tools/tu_map.py unit 1689,
// 0x7100b37360-0x7100b49730, holds that reflected class and the functions that take it), as its
// callers see it: the minimap manager's reference at +0xd63f40 points to it (src/game/
// CMinimapManager.cpp, src/game/GUI/CIngameMenu/fn_7100b2de48.cpp), and src/game/GUI/CMapMenuController.cpp
// lays out its markers. Only the members the callers touch are spelled out; the names and the
// layout are placeholders.

#include "types.h"

// The object behind the reference at +0x238: a flag at +0xb4 that hides the menu's markers.
struct UnkMapMenuSub {
    u8 pad_0x0[0xb4];
    bool field_0xb4;
};
struct UnkMapMenuHolder {
    u8 pad_0x0[0x10];
    UnkMapMenuSub* field_0x10;
};
struct UnkMapMenuController {
    u8 pad_0x0[0x238];
    UnkMapMenuHolder* field_0x238;
};

// The flag at +0xb4 of the object behind the reference at +0x238, false when there is none. While
// it is set the minimap is drawn transparent (fn_7100e8f6bc) and the in-game menu does not switch
// pages (fn_7100b2de48's unit). Not defined by hand.
bool fn_7100b41e5c(UnkMapMenuController* self);
