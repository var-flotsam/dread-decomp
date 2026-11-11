#pragma once

// UnkTU0: functions and globals of this translation unit that other files use (tools/tu_map.py unit 0,
// 0x7100000030-0x710000147c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkDropTarget;
struct UnkUserDataOwner;

bool fn_7100000790(const char* name, const char* fmt, ...);  // Calls a script function.
void fn_71000003d4(void*, const char*, bool);  // The string-id constructor (base/global/CStrId.h).
extern UnkUserDataOwner* g_unk_7101d47308;
extern UnkDropTarget* g_unk_7101d47318;
