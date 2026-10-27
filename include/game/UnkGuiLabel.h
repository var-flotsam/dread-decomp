#pragma once

// GUI::CLabel (tools/tu_map.py unit 2752, named after the class by reflection) as the hand-written
// screens and menus use it: a display object (game/UnkGuiDisplayObject.h) that shows a text. Only
// the setters the callers share are declared; the names and the layout are placeholders.

#include "game/UnkGuiDisplayObject.h"
#include "types.h"

// A text of 32-bit characters as the label's text setter reads it (0x20 bytes: the characters at
// +0x8, null when empty, and their count at +0x10). The callers build it on derived types that add
// its constructor from UTF-32 text (fn_7100066a50) and its destructor (fn_7100066e44).
struct UnkGuiText {
    u8 unk_0x0[0x20];
};
static_assert(sizeof(UnkGuiText) == 0x20, "UnkGuiText placeholder layout");

// Sets the label's text (+0x238; `force` sets it in a case where the label otherwise keeps its own)
// and builds the text it shows from it (+0x218), the localization keys in it replaced by their
// texts; vf_0x88 runs when the shown text changes. Not decompiled yet.
void fn_71011ec468(UnkGuiDisplayObject* self, const UnkGuiText& text, bool force);

// Set the two values the label lays its text out by (+0x2b4: the map markers pass the side their
// widget is anchored on; +0x328), each re-laying the text out (fn_71011ec768) when it changes.
// Defined in the unit's generated code.
void fn_71011ed018(UnkGuiDisplayObject* self, u32 value);
void fn_71011ed084(UnkGuiDisplayObject* self, u32 value);
