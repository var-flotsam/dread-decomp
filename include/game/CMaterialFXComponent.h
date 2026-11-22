#pragma once

// CMaterialFXComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1418,
// 0x7100925fbc-0x7100927670). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "types.h"

struct UnkMagnetColorChange_b0086;
struct UnkMagnetFx_b0086;
void fn_7100926aa0(UnkMagnetFx_b0086* fx, UnkMagnetColorChange_b0086* change);
extern char g_unk_7101a833b0[];
void fn_71009272c4(UnkMagnetColorChange_b0086* self, u32 index, bool flag, const base::math::UnkVector4D* from, const base::math::UnkVector4D* to, f32 time);
