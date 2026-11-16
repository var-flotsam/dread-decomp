#pragma once

// CLabel: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2752,
// 0x71011e8df8-0x71011f0810). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/CRntString.h"
#include "types.h"

struct UnkLabelLayout;
struct UnkStruct_71011ebdb4;
void fn_71011ebdb4(UnkStruct_71011ebdb4* self);
void fn_71011ec314(UnkLabelLayout* self);
void fn_71011ec768(UnkLabelLayout* self);
void fn_71011ed1e8(UnkLabelLayout* self, bool only_if_shown);
extern char g_unk_7101cb6380[];  // The label's vtable.
extern char g_unk_7101cb65e8[];  // Its line list's vtable.
void fn_71011ef0c8(UnkGuiDisplayObject* label, bool value);
void fn_71011ed150(UnkGuiDisplayObject* label, const base::global::CStrId& font);
void fn_71011eec60(UnkGuiDisplayObject* label, u32 mode);

// A label's glyphs (0x30 bytes each; src/game/GUI/CLabel.cpp lays one out): reserves `capacity` glyphs;
// assigns another vector's glyphs, returning `self`.
struct UnkLabelGlyph;
using UnkGlyphVector = base::global::CRntVector<UnkLabelGlyph>;
void fn_71011f05d4(UnkGlyphVector* self, u32 capacity);
UnkGlyphVector* fn_71011ee3b4(UnkGlyphVector* self, const UnkGlyphVector* other);
void fn_71011ecfbc(UnkGuiDisplayObject* label, const char32_t* text);  // Sets the label's text.
void fn_71011ec424(UnkGuiDisplayObject* label, const base::global::CRntString& text);
