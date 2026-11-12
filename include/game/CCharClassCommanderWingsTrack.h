#pragma once

// CCharClassCommanderWingsTrack: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1947,
// 0x7100c10ce0-0x7100c11734). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflConverter.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeader;

void fn_7100c10ce0();
void fn_7100c1140c() __attribute__((visibility("hidden")));
void fn_7100c11418() __attribute__((visibility("hidden")));
extern char g_unk_717277e0a8;
extern char g_unk_717277e0f0;
extern UnkReflType g_unk_717277df90;
extern UnkTUHeader g_unk_717277e0f8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717277e118 __attribute__((visibility("hidden")));
extern UnkConverter g_unk_717277e0b0;
extern UnkConverter g_unk_717277e0d0;
void fn_7100c10e2c(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
