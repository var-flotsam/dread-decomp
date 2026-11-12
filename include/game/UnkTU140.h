#pragma once

// UnkTU140: functions and globals of this translation unit that other files use (tools/tu_map.py unit 140,
// 0x710014803c-0x7100149bec). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkRenderPool;
struct UnkColorArg_710050f26c;
struct UnkSceneObject;

extern char g_unk_7101a1cbe8[];
bool fn_710014826c(UnkSceneObject*, u32, u32, UnkColorArg_710050f26c, bool, f32);
void* fn_7100149930(UnkRenderPool* pool, void* a, UnkSceneObject** out, base::global::CStrId* name);
void fn_7100149460(UnkSceneObject* object, const UnkVec3f* position);
