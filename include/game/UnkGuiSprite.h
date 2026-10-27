#pragma once

// GUI::CSprite (tools/tu_map.py unit 2756, named after the class by reflection) as the hand-written
// screens use it: a display object (game/UnkGuiDisplayObject.h) that shows an image. Only the
// setters the callers share are declared; the names are placeholders.

#include "base/global/CStrId.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

// Sets the sprite's image from an id of the form "sheet/image": the two parts become string ids,
// compared with the ones the sprite has (+0x1c8, +0x1d0). Not decompiled yet.
void fn_71011f8bdc(UnkGuiDisplayObject* self, base::global::CStrId id);
