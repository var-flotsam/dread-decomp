#pragma once

// UnkTU127: functions and globals of this translation unit that other files use (tools/tu_map.py unit 127,
// 0x710014333c-0x7100143b3c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkStruct_71001436f4;
struct UnkModel_ovb0032;
struct UnkModel;
struct UnkTaggedSubArg;
struct UnkTaggedSubObject127;
void fn_71001436a0(UnkModel* model, u32 flag);
void fn_710014364c(UnkModel* model, u32 flag);
void fn_710014333c(UnkTaggedSubObject127* self, UnkTaggedSubArg* arg);
extern char g_unk_7101a1c170[];
bool fn_71001436f4(UnkStruct_71001436f4* self, u32 a, const base::math::CVector4D* b, u32 c);
void fn_7100143578(UnkModel_ovb0032* self, const UnkVec3f* translation);
