#pragma once

// CCharClassBaseTriggerComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1562,
// 0x7100a461c0-0x7100a4983c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkMagmaArea_b50;
struct CFreezeRoomComponent;
struct UnkFan_A4_b74;
struct UnkSteamJetArg;
struct UnkSteamJetComponent;
void fn_7100a46a68(char*);
bool fn_7100a46e00(UnkSteamJetComponent*, UnkSteamJetArg*);
void fn_7100a474f8(UnkFan_A4_b74*, f32);
void fn_7100a46e78(CFreezeRoomComponent* self);
bool CCharClassBaseTriggerComponent_IsPointInColliderBounds(UnkMagmaArea_b50*, const base::math::CVector2D*);
void fn_7100a46b64(char*);
extern void* g_unk_7101a8ebc8[];
