#pragma once

// The items' blackboard properties, built by the item unit's static initialiser
// (src/game_s7/fn_7100ae39e0.cpp) from the names in the comments ("ITEM_..."); their layout is
// UnkPropertyEx<> (game/UnkTUProperty.h).
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
extern UnkPropertyEx<> g_unk_7172759fd8;  // ITEM_METROID_COUNT
extern UnkPropertyEx<> g_unk_717275a018;  // ITEM_METROID_TOTAL_COUNT
extern UnkPropertyEx<> g_unk_717275a2d8;  // ITEM_ENERGY_TANK_SHARDS
extern UnkPropertyEx<> g_unk_717275a318;  // ITEM_ENERGY_TANKS
extern UnkPropertyEx<> g_unk_717275a358;  // ITEM_MISSILE_TANKS
extern UnkPropertyEx<> g_unk_717275a398;  // ITEM_MISSILE_PLUS_TANKS
extern UnkPropertyEx<> g_unk_717275a3d8;  // ITEM_POWER_BOMB_TANKS
extern UnkPropertyEx<> g_unk_717275ac18;  // ITEM_CENTRAL_UNIT_ENERGY
