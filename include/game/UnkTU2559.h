#pragma once

// UnkTU2559: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2559,
// 0x7100e16e60-0x7100e1bc3c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflConverter.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeader;
struct UnkLoadCheckPointSingleton_ovb0118;
void fn_7100e16e60();
void fn_7100e1793c();
void fn_7100e17cc8(UnkLoadCheckPointSingleton_ovb0118* self);
void fn_7100e19954(UnkLoadCheckPointSingleton_ovb0118* self, bool flag);
void fn_7100e1b5c8() __attribute__((visibility("hidden")));
void fn_7100e1b5d4() __attribute__((visibility("hidden")));
void fn_7100e1b5e0() __attribute__((visibility("hidden")));
void fn_7100e1b5ec() __attribute__((visibility("hidden")));
void fn_7100e1b5f8() __attribute__((visibility("hidden")));
void fn_7100e1b604() __attribute__((visibility("hidden")));
extern char g_unk_71727c0cd8;
extern char g_unk_71727c0e10;
extern char g_unk_71727c0f70;
extern UnkReflType g_unk_71727c0bc0;
extern UnkReflType g_unk_71727c0d20;
extern UnkReflType g_unk_71727c0e58;
extern UnkTUHeader g_unk_71727c0fb8 __attribute__((visibility("hidden")));
void fn_7100e177ec(UnkReflType* self, const base::global::CStrId& name, void* extra);
extern UnkConverter g_unk_71727c0ce0;
extern UnkConverter g_unk_71727c0d00;
extern UnkConverter g_unk_71727c0e18;
extern UnkConverter g_unk_71727c0e38;
extern UnkConverter g_unk_71727c0f78;
extern UnkConverter g_unk_71727c0f98;
void fn_7100e1766c(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
void fn_7100e17b48(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
