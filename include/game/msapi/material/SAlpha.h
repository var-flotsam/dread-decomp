#pragma once

// SAlpha: functions of this translation unit that other files call (tools/tu_map.py unit 229,
// 0x71001a12f4-0x71001a9f3c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkMaterial;
struct UnkMaterialValuesVector;
void fn_71001a6ec8(UnkMaterial* self, u32 flags);
void fn_71001a9bd0(UnkMaterialValuesVector* v, u32 n);
void fn_71001a836c(UnkMaterial* self, base::UnkVector<u64>* list);
