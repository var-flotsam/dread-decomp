#pragma once

// UnkTU1640: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1640,
// 0x7100adf0bc-0x7100adfa00). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflConverter.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeader;

void fn_7100adf0bc();
void fn_7100adf658() __attribute__((visibility("hidden")));
void fn_7100adf664() __attribute__((visibility("hidden")));
void fn_7100adf670() __attribute__((visibility("hidden")));
void fn_7100adf67c() __attribute__((visibility("hidden")));
extern char g_unk_7172758950;
extern char g_unk_7172758a88;
extern UnkReflType g_unk_7172758838;
extern UnkReflType g_unk_7172758998;
extern UnkTUHeader g_unk_7172758ad0 __attribute__((visibility("hidden")));
void fn_7100adf508(UnkReflType* self, const base::global::CStrId& name, void* extra);
extern UnkConverter g_unk_7172758958;
extern UnkConverter g_unk_7172758978;
extern UnkConverter g_unk_7172758a90;
extern UnkConverter g_unk_7172758ab0;
void fn_7100adf388(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
