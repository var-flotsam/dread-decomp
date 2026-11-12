#pragma once

// UnkTU1726: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1726,
// 0x7100b879a0-0x7100b88a68). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeader;

void fn_7100b87b5c();
extern char g_unk_7172768ac8;
extern char g_unk_7172768ac9;
extern UnkReflType g_unk_7172768990;
extern UnkTUHeader g_unk_7172768ad0 __attribute__((visibility("hidden")));
void fn_7100b87e10(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
