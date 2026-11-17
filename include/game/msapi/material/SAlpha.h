#pragma once

// SAlpha: functions and globals of this translation unit that other files use (tools/tu_map.py unit 229,
// 0x71001a12f4-0x71001a9f3c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/UnkNoArg.h"
#include "base/global/CRntFile.h"
#include "types.h"

struct UnkMaterialReset_ovb0098;
struct UnkRenderItem_b0247;
struct UnkMaterial;
struct UnkMaterialValuesVector;
void fn_71001a6ec8(UnkMaterial* self, u32 flags);
void fn_71001a9bd0(UnkMaterialValuesVector* v, u32 n);
void fn_71001a836c(UnkMaterial* self, base::global::CRntVector<u64>* list);
extern void* g_unk_7101a1e388[];
void fn_71001a69a0(UnkNoArg, base::global::CRntFile* file, void* dst);
void fn_71001a7bfc(UnkRenderItem_b0247* item, u32 mode);
void fn_71001a6418(UnkMaterialReset_ovb0098* self);
