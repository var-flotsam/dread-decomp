#pragma once

// CCharClassSetCameraNearEvent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2278,
// 0x7100cd11d0-0x7100cd2190). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflConverter.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeaderAndClass;

void fn_7100cd17a8();
void fn_7100cd1c90() __attribute__((visibility("hidden")));
void fn_7100cd1c9c() __attribute__((visibility("hidden")));
extern char g_unk_717279caf8;
extern char g_unk_717279cc18;
extern UnkReflType g_unk_717279cb00;
extern UnkTUHeaderAndClass g_unk_717279cc60 __attribute__((visibility("hidden")));
extern UnkConverter g_unk_717279cc20;
extern UnkConverter g_unk_717279cc40;
void fn_7100cd1954(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
