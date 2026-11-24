#pragma once

// UnkTU92: functions and globals of this translation unit that other files use (tools/tu_map.py unit 92,
// 0x71000edfc0-0x71000ef534). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"
#include "base/reflection/TypeObject.h"

struct UnkReadArg_b0026;
struct UnkEnumInfo;
struct UnkWriteRoot_b0026;
struct UnkWriteStack_b0026;
void InitReflValueListWithRoot(UnkWriteStack_b0026* self, UnkWriteRoot_b0026* root, u32 count);
namespace base::serialization { enum class EDeserializeCollectionElementMode : u32; }
template <> UnkEnumInfo base::reflection::TypeObject<base::serialization::EDeserializeCollectionElementMode, UnkEnumInfo>::object asm("g_unk_7101d67c10");
bool ReadReflListingTopEntryByKind(void* reader, u32 flags, UnkReadArg_b0026* arg, const base::global::CStrId* name);
