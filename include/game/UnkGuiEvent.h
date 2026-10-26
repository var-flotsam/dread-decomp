#pragma once

// A GUI event, as the senders build it on the stack (0x28 bytes): fn_710120235c fills one for a
// display object (through the object's handle slot, vf_0x28), an event kind and an optional
// property bag, and fn_7101202488 destroys it (it frees an owned bag). The GUI root sends it
// (fn_71011b6e0c, game/UnkGuiRoot.h). The name and the layout are placeholders.

#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkGuiEvent;
void fn_710120235c(UnkGuiEvent* self, UnkGuiDisplayObject* object, const s32* kind, void* bag);
void fn_7101202488(UnkGuiEvent* self);
