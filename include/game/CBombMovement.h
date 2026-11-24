#pragma once

// CBombMovement: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1610,
// 0x7100aa9170-0x7100aac8c8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct CGoliathXBurstProjectionBombMovement;
struct UnkFanBombList_ovb0122;
struct UnkPbMovementBase_ovb0015;
UnkFanBombList_ovb0122* fn_7100aa9ecc();
void fn_7100aa9f3c(UnkPbMovementBase_ovb0015*);  // Derived classes call it as their base's destructor.
extern void* g_unk_7101a94c90[];

// The bomb hit handler (fn_7100aaaaf4, src/game/CBombMovement/CBombMovement_Hits_ovb0078.cpp): hands each
// hit to the member function for its kind, an actor's or the tiles'.
struct UnkBombHitObject_ovb0078;
struct UnkBombHitInfo_ovb0078;
struct UnkBombHitHandler_ovb0078 {};
using UnkBombOnActor_ovb0078 = void (UnkBombHitHandler_ovb0078::*)(void* actor, void* arg, const u64* key,
                                                                   UnkBombHitInfo_ovb0078* info);
using UnkBombOnTiles_ovb0078 = void (UnkBombHitHandler_ovb0078::*)(UnkBombHitObject_ovb0078* tiles, void* arg,
                                                                   const u64* key, UnkBombHitInfo_ovb0078* info,
                                                                   f32 value);
void CBombMovement_DispatchHit(UnkBombHitHandler_ovb0078* self, u32* kinds, void* arg, UnkBombHitInfo_ovb0078* info,
                   f32 value, UnkBombOnActor_ovb0078 on_actor, UnkBombOnTiles_ovb0078 on_tiles);
void fn_7100aaa024(CGoliathXBurstProjectionBombMovement* self);
