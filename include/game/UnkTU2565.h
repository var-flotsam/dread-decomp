#pragma once

// UnkTU2565: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2565,
// 0x7100e768b0-0x7100e7cdfc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeader;
struct UnkCharMap;
void fn_7100e768b0(UnkCharMap* self, const char* chars, u32 count, u32 a, void* b, u32 c);
void fn_7100e76a5c(void* self);
void fn_7100e76c50();
void fn_7100e77234();
void fn_7100e77a0c();

// Wraps `arg` in a small visitor and visits the items of `kind` with it; returns the item count
// (src/game/UnkUnit2565/UnkUnit2565_Visit.cpp).
u32 fn_7100e791a4(void* self, void* arg, u32 kind, bool flag, void* extra);

extern UnkCharMap g_unk_71727c8f58;
extern char g_unk_71727c8f80;
extern char g_unk_71727c8f81;
extern char g_unk_71727c90c0;
extern char g_unk_71727c90c1;
extern char g_unk_71727c9200;
extern char g_unk_71727c9201;
extern char g_unk_71727c9378;
extern char g_unk_71727c94b8;
UnkReflType* fn_7100e76a98();
UnkReflType* fn_7100e76b74();
UnkReflType* fn_7100e77158();
UnkReflType* fn_7100e7773c();
extern UnkReflType g_unk_71727c8f88;
extern UnkReflType g_unk_71727c90c8;
extern UnkReflType g_unk_71727c9208;
extern UnkReflType g_unk_71727c9380;
extern UnkTUHeader g_unk_71727c9948 __attribute__((visibility("hidden")));
void fn_7100e77818(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
void fn_7100e76fd4(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100e775b8(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100e77d68(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern UnkPropertyEx<> g_unk_71727c94c0;
