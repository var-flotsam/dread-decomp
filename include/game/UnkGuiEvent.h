#pragma once

// A GUI event, as the senders build it on the stack (0x28 bytes): fn_710120235c fills one for a
// display object (through the object's handle slot, vf_0x28), an event kind and an optional
// property bag, and fn_7101202488 destroys it (it frees an owned bag). The GUI root sends it
// (fn_71011b6e0c, game/UnkGuiRoot.h). The name and the layout are placeholders.

#include "game/UnkGuiDisplayObject.h"
#include "types.h"

namespace unk_prop_bag {
struct UnkPropBag;  // game_s7/UnkPropBagS7.h
}

// The event itself: the first word is the handle fn_710120235c takes from the object's vf_0x28
// slot (null if the object has none); the rest is not spelled out.
struct UnkGuiEvent {
    u8 unk_0x0[0x28];
};
static_assert(sizeof(UnkGuiEvent) == 0x28, "UnkGuiEvent placeholder layout");

// Builds the event in `self` for `object`, an event kind (0 and 1: shown or hidden, 2 and 3:
// selected or deselected, 4: a state index) and an optional property bag (src/game/GUI/UnkGuiEvent.cpp).
void fn_710120235c(UnkGuiEvent* self, UnkGuiDisplayObject* object, const s32* kind, unk_prop_bag::UnkPropBag* bag);
// Destroys the event (frees an owned bag; src/game/GUI/UnkGuiEvent.cpp).
void fn_7101202488(UnkGuiEvent* self);
