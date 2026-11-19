#pragma once

// UnkTU1601: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1601,
// 0x7100a8fbd8-0x7100a969f0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/math/CVector4D.h"
#include "base/reflection/TypeObject.h"
#include "types.h"

struct UnkCallbackSetElem;
struct UnkCallbackSetVector;
struct UnkThermalDevice_A5_b73;
void fn_7100a93bc4(void* self);
void fn_7100a925a0(UnkThermalDevice_A5_b73* self);
extern void* g_unk_7101a92ed0[];
void fn_7100a956b8(UnkCallbackSetVector* v, u32 n);
void fn_7100a95868(UnkCallbackSetVector* v, const UnkCallbackSetElem* value);
void fn_7100a959a0(UnkCallbackSetVector* v, u32 size);
namespace CThermalDeviceComponent { enum class EPipeGroup : u32; }
template <> base::reflection::CType base::reflection::TypeObject<CThermalDeviceComponent::EPipeGroup>::object asm("g_unk_7172752160");
extern base::math::CVector4D g_unk_7101a92ec0;
extern base::math::CVector4D g_unk_7172751c18;
extern const u64 g_unk_710164b5f0 __attribute__((visibility("hidden")));
extern const u64 g_unk_710164b5f8 __attribute__((visibility("hidden")));
