#pragma once

// CTriggerComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1578,
// 0x7100a59a00-0x7100a653b8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkTUProperty.h"
#include "base/global/CRntFile.h"
#include "types.h"

struct UnkTriggerUpdate_ovb0067;
struct UnkTriggerActor_ovb0067;
struct UnkTriggerComponent_ovb0067;
struct UnkVec3f;
struct UnkColliderTrigger_b0058;
struct UnkProperty;
struct UnkHandleList_CGameManager;
struct UnkTriggerEntry_b50a5;
struct UnkTriggerOwned_b56;
struct UnkTrigger_b50a5;
struct UnkTriggerComponent_la601cc;
struct UnkTriggerTarget_la601cc;
void fn_7100a632e8(UnkTriggerOwned_b56* self);
UnkHandleList_CGameManager* fn_7100a5e1dc();
// Moves the component's key from `previous`'s listener list to `target`'s.
void CTriggerComponent_ReplaceEntityInside(UnkTriggerComponent_la601cc* self, UnkTriggerTarget_la601cc* target,
                   UnkTriggerTarget_la601cc* previous);

// The original's unit defines it, so the compiler knows it doesn't keep `entry`'s address; noescape says so here,
// which lets a caller's last call be a tail call.
void fn_7100a5fdcc(UnkTrigger_b50a5* self, UnkTriggerEntry_b50a5** __attribute__((noescape)) entry);
extern UnkProperty g_unk_717274d6c8;
extern UnkTunable g_unk_717274ddd8;
extern UnkProperty g_unk_717274d6f0;
extern void* g_unk_7101a8f5c8[];
void fn_7100a5e8a4(UnkTriggerUpdate_ovb0067* self);
void fn_7100a5ee80(UnkTriggerComponent_ovb0067* self, s32 kind, UnkTriggerActor_ovb0067* actor, const UnkVec3f* dir);
void CTriggerComponent_LoadFromFile(UnkColliderTrigger_b0058* self, base::global::CRntFile* file);
