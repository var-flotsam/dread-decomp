#pragma once

// CAbilityGhostAura: functions of this translation unit that other files call (tools/tu_map.py unit 430,
// 0x7100306910-0x71003162a8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAbility;
struct UnkGhostAura_A2_b70;
struct UnkSelf;
u64 fn_7100308c74();
u32 fn_7100309568(UnkAbility* ability);
void fn_7100309fd8(UnkGhostAura_A2_b70* self, u32 a, u32 b);
bool fn_710030b8b4(UnkSelf* self, void* a2, void* zero);
