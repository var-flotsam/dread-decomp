#pragma once

// UnkTU2541: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2541,
// 0x7100daf1cc-0x7100db23bc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflConverter.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeader;

void fn_7100daf1cc();
void fn_7100db2038() __attribute__((visibility("hidden")));
void fn_7100db2044() __attribute__((visibility("hidden")));
extern char g_unk_71727bc988;
extern UnkReflType g_unk_71727bc870;
extern UnkTUHeader g_unk_71727bc9d0 __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727bc990;
extern UnkConverter g_unk_71727bc9b0;
