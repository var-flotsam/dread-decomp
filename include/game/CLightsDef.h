#pragma once

// CLightsDef: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2529,
// 0x7100d9cf1c-0x7100da201c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeader;
struct UnkClearedArray;
struct UnkLightsArray_b42a3;
void fn_7100da0870(UnkClearedArray* self, u32 count);  // Resizes.
void fn_7100d9dce8();
void fn_7100d9e2cc();
void fn_7100da1508(UnkLightsArray_b42a3* self, u32 capacity);
extern char g_unk_71727ba998;
extern char g_unk_71727ba999;
extern char g_unk_71727ba99a;
extern char g_unk_71727ba99b;
extern char g_unk_71727ba99c;
extern char g_unk_71727baad8;
extern char g_unk_71727baad9;
extern char g_unk_71727bac18;
extern char g_unk_71727bac19;
extern char g_unk_71727bad90;
extern char g_unk_71727bad91;
extern char g_unk_71727bad92;
UnkReflType* fn_7100d9d978();
UnkReflType* fn_7100d9da54();
UnkReflType* fn_7100d9db30();
UnkReflType* fn_7100d9dc0c();
UnkReflType* fn_7100d9e1f0();
UnkReflType* fn_7100d9e7d4();
UnkReflType* fn_7100d9eaa4();
UnkReflType* fn_7100d9eb80();
extern UnkReflType g_unk_71727ba9a0;
extern UnkReflType g_unk_71727baae0;
extern UnkReflType g_unk_71727bac20;
extern UnkTUHeader g_unk_71727bb618 __attribute__((visibility("hidden")));
void fn_7100d9e8b0(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
void fn_7100d9e06c(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100d9e650(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
