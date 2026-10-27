#pragma once

// The minimap's icon kinds: string ids that the static initialiser of the minimap definitions'
// unit (src/game_s7/fn_7100e72680.cpp; tools/tu_map.py unit 2564, CMinimapDef) builds from their
// names with fn_7100e68cac, which also adds each to a list of all of them; fn_7100e68dd0 releases
// them at exit. Only the ones another file reads are declared here: the kinds the minimap manager
// shows again by number (fn_7100e9d1f8, src/game/CMinimapManager.cpp).

#include "base/global/CStrId.h"

// The breakable tiles' icons, by what breaks them (fn_7100e9d1f8's kinds 1 to 7 and 9).
extern base::global::CStrId g_unk_71727c80b0;  // "TilePowerbeam"
extern base::global::CStrId g_unk_71727c80b8;  // "TileBomb"
extern base::global::CStrId g_unk_71727c80c0;  // "TileMissile"
extern base::global::CStrId g_unk_71727c80c8;  // "TileSupermissile"
extern base::global::CStrId g_unk_71727c80d0;  // "TilePowerbomb"
extern base::global::CStrId g_unk_71727c80d8;  // "TileScrewattack"
extern base::global::CStrId g_unk_71727c80e0;  // "TileWeight"
extern base::global::CStrId g_unk_71727c80e8;  // "TileSpeedboost"
