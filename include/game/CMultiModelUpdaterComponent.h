#pragma once

// CMultiModelUpdaterComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1426,
// 0x710093776c-0x7100939af0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkMultiModel_l93852c;
struct UnkStrIdPathResource;
struct UnkModelValue_b0119;
struct UnkModelEntry_A5_b67;
struct UnkMultiModel_A5_b67;
bool fn_7100938fcc(UnkMultiModel_A5_b67* self, UnkModelEntry_A5_b67* entry);
bool CMultiModelUpdaterComponent_SelectModelByName(void* updater, const u64* id);
void CMultiModelUpdaterComponent_DestroyModelMapValue(UnkStrIdPathResource* self);
void CMultiModelUpdaterComponent_CopyConstructModelMapValue(UnkModelValue_b0119* self, const UnkModelValue_b0119* other);
bool CMultiModelUpdaterComponent_AddModel(UnkMultiModel_l93852c* self, const base::global::CStrId* name, const base::global::CFilePathStrId* path);
