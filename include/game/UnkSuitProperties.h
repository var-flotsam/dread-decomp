#pragma once

// The suit items' blackboard properties, built by the item unit's static initialiser
// (src/game_s7/fn_7100ae39e0.cpp) from the names "ITEM_HYPER_SUIT", "ITEM_GRAVITY_SUIT" and
// "ITEM_VARIA_SUIT"; their layout is UnkPropertyEx<> (game/UnkTUProperty.h).
//
// The item code sees the same objects as item definitions (UnkIdSource at 0, the qualified id of
// game/UnkGameBlackboard.h at 0x10, which is the property's member): a file that does so declares
// its own definition view and reaches it through SuitAs.

#include "game/UnkTUProperty.h"

extern UnkPropertyEx<> g_unk_717275a418;  // ITEM_HYPER_SUIT
extern UnkPropertyEx<> g_unk_717275a458;  // ITEM_GRAVITY_SUIT
extern UnkPropertyEx<> g_unk_717275a498;  // ITEM_VARIA_SUIT

// The item-definition view (`Def`, derived from UnkIdSource) of a suit property.
template <class Def>
static inline Def& SuitAs(UnkPropertyEx<>& property) {
    return reinterpret_cast<Def&>(property);
}
