#pragma once

// CHyperDashAttackAction: functions and globals of this translation unit that other files use (tools/tu_map.py unit 662,
// 0x7100449060-0x710044b72c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

namespace base {
namespace reflection {
struct UnkReflObject;
}  // namespace reflection
}  // namespace base
struct UnkHyperDashNode_ovb0095;
struct UnkHyperDashAttackAction_l4496a8;
extern void* g_unk_7101a36838[];  // The action's vtable.
extern u8 g_unk_71726c8f08[];
base::reflection::UnkReflObject* CHyperDashAttackAction_GetNamedOrCurrentAttack(UnkHyperDashNode_ovb0095* self);  // The dash's target.
// Starts the dash at `target`; whether it started.
bool fn_7100449bdc(UnkHyperDashAttackAction_l4496a8* self, void* target, bool flag);
