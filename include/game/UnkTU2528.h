#pragma once

// UnkTU2528: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2528,
// 0x7100d975d8-0x7100d9cf1c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkTUInit.h"
#include "base/reflection/UnkReflConverter.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeader;

void fn_7100d975d8();
void fn_7100d97964();
void fn_7100d97db0();
void fn_7100d9b7a4() __attribute__((visibility("hidden")));
void fn_7100d9b7b0() __attribute__((visibility("hidden")));
void fn_7100d9b7bc() __attribute__((visibility("hidden")));
void fn_7100d9b7c8() __attribute__((visibility("hidden")));
void fn_7100d9b7d4() __attribute__((visibility("hidden")));
void fn_7100d9b7e0() __attribute__((visibility("hidden")));
void fn_7100d9b7ec() __attribute__((visibility("hidden")));
void fn_7100d9b7f8() __attribute__((visibility("hidden")));
extern char g_unk_71727ba3f8;
extern char g_unk_71727ba598;
extern char g_unk_71727ba758;
extern char g_unk_71727ba759;
extern char g_unk_71727ba75a;
UnkReflType* fn_7100d9af70();
UnkReflType* fn_7100d9b04c();
extern UnkReflType g_unk_71727ba620;
extern UnkTUHeader g_unk_71727ba980 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727ba2e0;
extern UnkReflType g_unk_71727ba480;
extern UnkConverter g_unk_71727ba400;
extern UnkConverter g_unk_71727ba420;
extern UnkConverter g_unk_71727ba440;
extern UnkConverter g_unk_71727ba460;
extern UnkConverter g_unk_71727ba5a0;
extern UnkConverter g_unk_71727ba5c0;
extern UnkConverter g_unk_71727ba5e0;
extern UnkConverter g_unk_71727ba600;
void fn_7100d977e4(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
void fn_7100d97c30(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
void fn_7100d99934(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
