#pragma once

// CExplosionGfx: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2552,
// 0x7100dded9c-0x7100de277c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflConverter.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeader;

void fn_7100ddef54();
void fn_7100de1b0c() __attribute__((visibility("hidden")));
void fn_7100de1b18() __attribute__((visibility("hidden")));
extern char g_unk_71727be380;
extern char g_unk_71727be381;
extern char g_unk_71727be4a0;
extern char g_unk_71727be4e8;
UnkReflType* fn_7100dded9c();
UnkReflType* fn_7100ddee78();
extern UnkTUHeader g_unk_71727be710 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727be730 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727be388;
extern UnkConverter g_unk_71727be4a8;
extern UnkConverter g_unk_71727be4c8;
void fn_7100ddf1c0(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
