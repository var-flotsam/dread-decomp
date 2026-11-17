#pragma once

// CAbilitySpeedBooster: functions and globals of this translation unit that other files use (tools/tu_map.py unit 435,
// 0x710033a318-0x710033fa28). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkV4_b47;
struct UnkAbility;
struct UnkSmAbility2_A2_b63;
struct UnkUnit435Self;
void fn_710033cd50(UnkUnit435Self* self, u32 arg);
u64 fn_710033b6a4();
bool fn_710033c174(UnkSmAbility2_A2_b63*);
extern char g_unk_7101a239c8[];  // The value holders' vtable.
bool fn_710033cf20(UnkAbility* ability);
extern const u8 g_unk_7101649054[] __attribute__((visibility("hidden")));
extern const UnkV4_b47 g_unk_7101649044 __attribute__((visibility("hidden")));  // the model colour reset
