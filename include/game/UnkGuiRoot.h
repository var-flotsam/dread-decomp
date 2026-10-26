#pragma once

// The GUI root, the global g_unk_717271e8f8: the registry the menus look compositions up in
// (game/UnkGuiHandle.h) and the owner of the focus, a reference-counted handle to the focused
// display object at +0x230. tools/tu_map.py puts its functions in the unit of
// gui/scripts/guimanager.lua. The name and the layout are placeholders.

#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkGuiRoot;
void fn_71011b6bbc(UnkGuiRoot* root, UnkGuiDisplayObject* object, bool value);
