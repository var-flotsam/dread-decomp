#pragma once

// UnkTU2176: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2176,
// 0x7100c65ffc-0x7100c66ab0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflConverter.h"
#include "base/global/CStrId.h"
#include "types.h"
#include "base/reflection/TypeObject.h"

struct UnkReflType;
struct UnkTUHeader;

void fn_7100c65ffc();
void fn_7100c66808() __attribute__((visibility("hidden")));
void fn_7100c66814() __attribute__((visibility("hidden")));
void fn_7100c66820() __attribute__((visibility("hidden")));
void fn_7100c6682c() __attribute__((visibility("hidden")));
extern char g_unk_7172791da8;
namespace game::logic::collision { enum class EColMat : u32; }
template <> UnkReflType base::reflection::TypeObject<game::logic::collision::EColMat, UnkReflType>::object asm("g_unk_7172791c90");
extern UnkTUHeader g_unk_7172791e30 __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172791db0;
extern UnkConverter g_unk_7172791dd0;
extern UnkConverter g_unk_7172791df0;
extern UnkConverter g_unk_7172791e10;
void fn_7100c66688(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
