#pragma once

// UnkTU315: functions and globals of this translation unit that other files use (tools/tu_map.py unit 315,
// 0x71002182c0-0x710021aca4). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkFadeColor;
struct UnkMatrix44;
struct UnkRenderManager;
struct UnkSceneObj_71010e2278;

extern void* g_unk_7101a202e0[];
void fn_7100219c6c(UnkRenderManager* self, bool enable, u64 layer, u32 arg3, bool arg4);
void fn_7100219dd0(UnkRenderManager* self, u64 model, const UnkMatrix44* m, u32 a, u32 b, u64* instance, u32 c);
void fn_7100219e10(UnkRenderManager* self, u64 object);
void fn_71002191a4(UnkRenderManager* self, u32 value);
bool fn_7100219588(UnkRenderManager* self, const void* pos, u32 a, u32 b);
void fn_7100219e64(UnkRenderManager* self, u64 object, const UnkMatrix44* m);
void fn_7100219804(UnkRenderManager* self, u32 layer);  // Drops the layer.
bool fn_7100219970(UnkRenderManager* self);
void fn_7100219bb4(UnkRenderManager* mgr, UnkSceneObj_71010e2278* obj, u32 layer);
void fn_7100219e58(UnkRenderManager*, u64, bool);
void fn_7100219fe4(UnkRenderManager* self, u32 layer, const UnkFadeColor& color);
void fn_7100219974(UnkRenderManager* self, u32 value);
bool fn_7100219c4c(UnkRenderManager* self);

// Forwards to the pass list (fn_71001fd540).
void fn_7100219c5c(UnkRenderManager* self, bool hide, void* item, u32 mode, bool flag);

// Forwards to the pass list (fn_71001fd640).
void fn_7100219ca4(UnkRenderManager* self, void* item, bool enable, u32 mode);

void fn_7100219da4(UnkRenderManager* target, bool a, bool b, u32 mode);
void fn_710021a0b4(UnkRenderManager* self, const u64* hash, const base::math::CVector4D* value);
void fn_7100219558(UnkRenderManager* self, const base::global::CStrId* light, bool flag);
void fn_710021a0bc(UnkRenderManager* self, const u64* hash, base::math::CVector4D* value);
