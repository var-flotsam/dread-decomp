#pragma once

// The items' blackboard properties, built by the item unit's static initialiser
// (src/game_s7/fn_7100ae39e0.cpp) from the names in the comments ("ITEM_..."); their layout is
// UnkPropertyEx<> (game/UnkTUProperty.h). Sorted by address; the amounts and tank counts come
// first, then the weapons, suits and abilities.
//
// The item code reads the same objects as item definitions: the property's member at +0x10 is the
// qualified id of game/UnkGameBlackboard.h, and the property itself is the object the id names
// (it starts with a vtable, as UnkIdSource does). ItemQualifiedId and ItemSource give those two
// views; a reader builds the id on first use with fn_7100ae9a10(&id, ItemSource(item), prefix).

#include "game/UnkGameBlackboard.h"
#include "game/UnkTUProperty.h"

extern UnkPropertyEx<> g_unk_7172759fd8;  // ITEM_METROID_COUNT
extern UnkPropertyEx<> g_unk_717275a018;  // ITEM_METROID_TOTAL_COUNT
extern UnkPropertyEx<> g_unk_717275a058;  // ITEM_MAX_LIFE
extern UnkPropertyEx<> g_unk_717275a098;  // ITEM_LIFE_SHARDS
extern UnkPropertyEx<> g_unk_717275a118;  // ITEM_CURRENT_LIFE
extern UnkPropertyEx<> g_unk_717275a158;  // ITEM_MAX_SPECIAL_ENERGY
extern UnkPropertyEx<> g_unk_717275a198;  // ITEM_CURRENT_SPECIAL_ENERGY
extern UnkPropertyEx<> g_unk_717275a1d8;  // ITEM_WEAPON_MISSILE_CURRENT
extern UnkPropertyEx<> g_unk_717275a218;  // ITEM_WEAPON_MISSILE_MAX
extern UnkPropertyEx<> g_unk_717275a258;  // ITEM_WEAPON_POWER_BOMB_CURRENT
extern UnkPropertyEx<> g_unk_717275a298;  // ITEM_WEAPON_POWER_BOMB_MAX
extern UnkPropertyEx<> g_unk_717275a2d8;  // ITEM_ENERGY_TANK_SHARDS
extern UnkPropertyEx<> g_unk_717275a318;  // ITEM_ENERGY_TANKS
extern UnkPropertyEx<> g_unk_717275a358;  // ITEM_MISSILE_TANKS
extern UnkPropertyEx<> g_unk_717275a398;  // ITEM_MISSILE_PLUS_TANKS
extern UnkPropertyEx<> g_unk_717275a3d8;  // ITEM_POWER_BOMB_TANKS
extern UnkPropertyEx<> g_unk_717275a418;  // ITEM_HYPER_SUIT
extern UnkPropertyEx<> g_unk_717275a458;  // ITEM_GRAVITY_SUIT
extern UnkPropertyEx<> g_unk_717275a498;  // ITEM_VARIA_SUIT
extern UnkPropertyEx<> g_unk_717275a4d8;  // ITEM_WEAPON_BOMB
extern UnkPropertyEx<> g_unk_717275a518;  // ITEM_WEAPON_CHARGE_BEAM
extern UnkPropertyEx<> g_unk_717275a558;  // ITEM_WEAPON_DIFFUSION_BEAM
extern UnkPropertyEx<> g_unk_717275a598;  // ITEM_WEAPON_GRAPPLE_BEAM
extern UnkPropertyEx<> g_unk_717275a5d8;  // ITEM_WEAPON_WIDE_BEAM
extern UnkPropertyEx<> g_unk_717275a618;  // ITEM_WEAPON_ICE_BEAM
extern UnkPropertyEx<> g_unk_717275a658;  // ITEM_WEAPON_WAVE_BEAM
extern UnkPropertyEx<> g_unk_717275a698;  // ITEM_WEAPON_SPAZER_BEAM
extern UnkPropertyEx<> g_unk_717275a6d8;  // ITEM_WEAPON_PLASMA_BEAM
extern UnkPropertyEx<> g_unk_717275a718;  // ITEM_WEAPON_HYPER_BEAM
extern UnkPropertyEx<> g_unk_717275a758;  // ITEM_WEAPON_SUPER_MISSILE
extern UnkPropertyEx<> g_unk_717275a798;  // ITEM_WEAPON_ICE_MISSILE
extern UnkPropertyEx<> g_unk_717275a7d8;  // ITEM_WEAPON_LINE_BOMB
extern UnkPropertyEx<> g_unk_717275a818;  // ITEM_WEAPON_POWER_BOMB
extern UnkPropertyEx<> g_unk_717275a858;  // ITEM_SPECIAL_ENERGY
extern UnkPropertyEx<> g_unk_717275a898;  // ITEM_DOUBLE_JUMP
extern UnkPropertyEx<> g_unk_717275a8d8;  // ITEM_MORPH_BALL
extern UnkPropertyEx<> g_unk_717275a918;  // ITEM_SCREW_ATTACK
extern UnkPropertyEx<> g_unk_717275a958;  // ITEM_SPACE_JUMP
extern UnkPropertyEx<> g_unk_717275a998;  // ITEM_SPRING_BALL
extern UnkPropertyEx<> g_unk_717275a9d8;  // ITEM_MAGNET_GLOVE
extern UnkPropertyEx<> g_unk_717275aa18;  // ITEM_FLOOR_SLIDE
extern UnkPropertyEx<> g_unk_717275aa58;  // ITEM_OPTIC_CAMOUFLAGE
extern UnkPropertyEx<> g_unk_717275aa98;  // ITEM_SPEED_BOOSTER
extern UnkPropertyEx<> g_unk_717275aad8;  // ITEM_GHOST_AURA
extern UnkPropertyEx<> g_unk_717275ab18;  // ITEM_METROIDNIZATION
extern UnkPropertyEx<> g_unk_717275ab58;  // ITEM_SONAR
extern UnkPropertyEx<> g_unk_717275ab98;  // ITEM_MULTILOCKON
extern UnkPropertyEx<> g_unk_717275ac18;  // ITEM_CENTRAL_UNIT_ENERGY

// The qualified id an item property holds at +0x10 (empty until it is first built).
static inline UnkQualifiedId& ItemQualifiedId(UnkPropertyEx<>& item) {
    return reinterpret_cast<UnkQualifiedId&>(item.field_0x10);
}

// The object the item's qualified id names: the property itself.
static inline UnkIdSource* ItemSource(UnkPropertyEx<>& item) {
    return reinterpret_cast<UnkIdSource*>(&item);
}
