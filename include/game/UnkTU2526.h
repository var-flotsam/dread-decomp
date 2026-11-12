#pragma once

// UnkTU2526: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2526,
// 0x7100d911d8-0x7100d92158). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeader;

void fn_7100d911d8();
void fn_7100d916e4();
extern char g_unk_71727b8ef0;
extern char g_unk_71727b9010;
extern UnkReflType g_unk_71727b8dd8;
extern UnkReflType g_unk_71727b8ef8;
extern UnkTUHeader g_unk_71727b9018 __attribute__((visibility("hidden")));
void fn_7100d91564(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
void fn_7100d91d10(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
