#pragma once

// UnkTU2555: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2555,
// 0x7100e06ce0-0x7100e07b60). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflConverter.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeader;

void fn_7100e06ce0();
void fn_7100e0700c();
void fn_7100e073d4() __attribute__((visibility("hidden")));
void fn_7100e073e0() __attribute__((visibility("hidden")));
extern char g_unk_71727bf810;
extern char g_unk_71727bf990;
extern UnkReflType g_unk_71727bf6f8;
extern UnkReflType g_unk_71727bf858;
extern UnkTUHeader g_unk_71727bf998 __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727bf818;
extern UnkConverter g_unk_71727bf838;
void fn_7100e06e8c(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
void fn_7100e07250(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
