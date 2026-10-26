#pragma once

// The GUI root, the global g_unk_717271e8f8: the registry the menus look compositions up in
// (game/UnkGuiHandle.h), the menu container at +0x68 the screens add their compositions to, the
// owner of the focus (a reference to the focused object at +0x230), and a list of keys at +0xca8
// the menus register with. tools/tu_map.py puts its functions in the unit of
// gui/scripts/guimanager.lua. The name and the layout are placeholders.

#include "game/UnkGuiDisplayObject.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// The reference at +0x230: the focused object it points at, at +0x10.
struct UnkGuiFocus {
    u8 unk_0x0[0x10];
    UnkWeakTarget* field_0x10;
};

struct UnkGuiRoot {
    u8 unk_0x0[0x68];
    UnkGuiDisplayObject* menu;
    u8 unk_0x70[0x1c0];
    UnkGuiFocus* field_0x230;
    u8 unk_0x238[0xa70];
    u8 field_0xca8[1];
};
static_assert(__builtin_offsetof(UnkGuiRoot, menu) == 0x68, "UnkGuiRoot placeholder layout");
static_assert(__builtin_offsetof(UnkGuiRoot, field_0x230) == 0x230, "UnkGuiRoot placeholder layout");
static_assert(__builtin_offsetof(UnkGuiRoot, field_0xca8) == 0xca8, "UnkGuiRoot placeholder layout");

extern UnkGuiRoot* g_unk_717271e8f8;

// Makes `object` the root's focused object (the reference at +0x230, `value` is the flag the
// menus pass false for). Defined in the unit's generated code.
void fn_71011b6bbc(UnkGuiRoot* root, UnkGuiDisplayObject* object, bool value);
