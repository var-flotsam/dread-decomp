#pragma once

// UnkTU23: functions and globals of this translation unit that other files use (tools/tu_map.py unit 23,
// 0x7100014d94-0x7100016890). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSoundMgr;
struct UnkMatrix44;

void fn_7100014e88(void* member);
void fn_7100014dfc(void* self, const void* other);
void fn_7100014d94(void* self, u32 arg);
void fn_7100014e5c(void* self, const void* other);
UnkMatrix44* fn_7100015508(void* matrices, u64 id);
extern UnkSoundMgr* g_unk_7101d4d820;  // The sound manager: base::snd::CSoundSystem (0x400940 bytes).
