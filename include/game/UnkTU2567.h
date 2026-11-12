#pragma once

// UnkTU2567: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2567,
// 0x7100eb695c-0x7100ebf278). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflConverter.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkReflType;
struct UnkTUData;
struct UnkTUHeader;
struct UnkArray_71729ce018;

bool fn_7100eb7470(void* transition, f32 dt);
void fn_7100eb695c();
void fn_7100eb6ccc();
void fn_7100eb7590(void* self);
void fn_7100eb7680(void* part);
void fn_7100ebecec() __attribute__((visibility("hidden")));
void fn_7100ebecf8() __attribute__((visibility("hidden")));
extern char g_unk_71729cdeb0;
extern char g_unk_71729cdfd0;
extern UnkArray_71729ce018 g_unk_71729ce018;
extern UnkReflType g_unk_71729cdd78;
extern UnkReflType g_unk_71729cdeb8;
extern UnkTUHeader g_unk_71729ce040 __attribute__((visibility("hidden")));
extern UnkTUData g_unk_71729ce058 __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71729cdfd8;
extern UnkConverter g_unk_71729cdff8;
void fn_7100eb6b48(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100eb71d8(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
