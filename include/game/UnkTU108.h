#pragma once

// UnkTU108: functions of this translation unit that other files call (tools/tu_map.py unit 108,
// 0x7100110c54-0x71001239b0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

struct UnkShakernautArea_b65a4;
struct UnkArgObj;
struct UnkEntry;
struct UnkInsertTarget;
struct UnkPart0;
struct UnkPartsRecord;
struct UnkPolygon109;
struct UnkPolygon2D;
struct UnkPolygon_b61;
struct UnkTarget108;
struct UnkTileOutline;
struct UnkVec3;
void* fn_710011dac0(UnkPartsRecord* self, const UnkPartsRecord* other);
bool fn_7100116de0(UnkEntry* entry, void* arg, u32 a, u32 b, f32 scale);
void* fn_7100118b94(void* sink, UnkVec3* point);
void fn_710011d8b4(UnkPart0* self, const UnkPart0* other);
bool fn_7100117aa8(UnkPolygon109* polygon, void* query, f32 distance);
void fn_7100118c78(UnkPolygon_b61* self);
void fn_7100119b88(UnkPolygon2D* self);
void fn_7100119c1c(UnkTarget108* target);
void fn_7100119f98(UnkTileOutline* outline, bool);
void fn_7100122b2c(UnkInsertTarget* self, const UnkArgObj* point, u32 index);
bool fn_71001176e8(UnkShakernautArea_b65a4* area, const base::math::CVector2D* pos, u32* out, void* filter, u32 flags, f32 radius);
void fn_710011ddc0(base::reflection::UnkReflFunction* fn);
