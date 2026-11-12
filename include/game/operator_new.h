#pragma once

// operator_new: functions of this translation unit that other files call (tools/tu_map.py unit 60,
// 0x7100082c38-0x7100084240). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSoundVoice2_b56;
struct UnkVoicePool_b56;
u64 fn_7100083854(UnkVoicePool_b56* self, UnkSoundVoice2_b56* voice);
void fn_71000839c0(bool current_thread);
void fn_7100083a10();
void* fn_7100083804(UnkVoicePool_b56* self);
