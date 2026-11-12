#pragma once

// UnkTU253: functions of this translation unit that other files call (tools/tu_map.py unit 253,
// 0x71001c071c-0x71001c33cc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "base/reflection/CClass.h"
#include "base/color/CColor4F.h"
#include "types.h"

struct UnkSlots;
struct UnkModelData;
struct UnkResetOwner;
struct UnkSource_710020c780;
void fn_71001c0968(UnkModelData* data, bool);
bool fn_71001c3040(void* object, u64 hash, u32 a, const f32* values, u32 count, f32 f);
void fn_71001c3270(UnkResetOwner* self);
bool fn_71001c32bc(UnkSource_710020c780* source, const u64* key, u32 flag, f32* out);
bool fn_71001c287c(void* target, u32 a, u32 b, base::math::CVector4D* colour, u32 c);
void fn_71001c09c4(UnkSource_710020c780* source, base::reflection::UnkReflObject* obj);
bool fn_71001c2bac(UnkSlots* self, u64 key, u32 kind, base::color::CColor4F* out, u32 mode);
void fn_71001c2ec0(void* target, u32 a, u32 b, base::math::CVector4D* colour, u32 c, f32 time);
