#pragma once

// CLightManager: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2556,
// 0x7100e07b60-0x7100e0f028). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeaderFnsClass;
struct UnkListedObj;
struct UnkPtrList;
void fn_7100e08768();
void fn_7100e08d4c();
UnkListedObj** fn_7100e0a5ec(UnkPtrList* list, u32 index);
extern char g_unk_71727bf9b0;
extern char g_unk_71727bf9b1;
extern char g_unk_71727bf9b2;
extern char g_unk_71727bfaf0;
extern char g_unk_71727bfaf1;
extern char g_unk_71727bfc30;
extern char g_unk_71727bfc31;
extern char g_unk_71727bfda8;
extern char g_unk_71727bfda9;
extern char g_unk_71727bfdaa;
extern char g_unk_71727bfdab;
extern char g_unk_71727bfdac;
UnkReflType* fn_7100e085b0();
UnkReflType* fn_7100e0868c();
UnkReflType* fn_7100e08c70();
UnkReflType* fn_7100e09254();
UnkReflType* fn_7100e09500();
UnkReflType* fn_7100e095dc();
UnkReflType* fn_7100e096b8();
extern UnkReflType g_unk_71727bf9b8;
extern UnkReflType g_unk_71727bfaf8;
extern UnkReflType g_unk_71727bfc38;
extern UnkTUHeaderFnsClass g_unk_71727c0520 __attribute__((visibility("hidden")));
void fn_7100e09330(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
