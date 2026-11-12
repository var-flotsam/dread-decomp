#pragma once

// UnkTU1670: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1670,
// 0x7100b0a450-0x7100b0da50). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeader;

void fn_7100b0a450();
void fn_7100b0ab60();
void fn_7100b0b144();
extern char g_unk_717275f088;
extern char g_unk_717275f089;
extern char g_unk_717275f08a;
extern char g_unk_717275f08b;
extern char g_unk_717275f1c8;
extern char g_unk_717275f1c9;
extern char g_unk_717275f308;
extern char g_unk_717275f309;
extern char g_unk_717275f480;
UnkReflType* fn_7100b0a9a8();
UnkReflType* fn_7100b0aa84();
UnkReflType* fn_7100b0b068();
UnkReflType* fn_7100b0b64c();
extern UnkReflType g_unk_717275f090;
extern UnkReflType g_unk_717275f1d0;
extern UnkReflType g_unk_717275f310;
extern UnkTUHeader g_unk_717275f8c8 __attribute__((visibility("hidden")));
void fn_7100b0b728(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
void fn_7100b0a6cc(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b0aee4(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b0b4c8(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
