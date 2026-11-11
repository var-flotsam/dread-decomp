#pragma once

// CAudioComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1148,
// 0x7100718714-0x710071ff64). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntitySoundComponent;
struct UnkSoundHandle;
void fn_7100719064(UnkEntitySoundComponent* self, bool a, bool b, bool c, bool d);
void fn_710071a524(UnkEntitySoundComponent* self, void* a, f32 x, f32 y);
void fn_7100719894(void* player, UnkSoundHandle handle, f32 a, f32 b);
UnkSoundHandle fn_710071a338(UnkEntitySoundComponent* self, void* a);
void fn_710071a398(void* target, u32* mask, f32 a, f32 b);
extern s32 g_unk_717270f61c;  // The live sound count.
