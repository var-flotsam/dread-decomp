#pragma once

// CGameBlackboard: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1647,
// 0x7100ae963c-0x7100aefd04). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflConverter.h"
#include "types.h"

struct UnkReflType;
struct UnkTUHeader;

void fn_7100aef674(void* self);
void fn_7100ae963c();
void fn_7100aea424();
void fn_7100aeaa08();
void fn_7100aebf80() __attribute__((visibility("hidden")));
void* fn_7100aec000(void*, void*) __attribute__((visibility("hidden")));
extern char g_unk_717275b6b8;
extern char g_unk_717275b700;
extern char g_unk_717275b701;
extern char g_unk_717275b702;
extern char g_unk_717275b840;
extern char g_unk_717275b841;
extern char g_unk_717275b980;
extern char g_unk_717275b981;
extern char g_unk_717275baf8;
UnkReflType* fn_7100aea26c();
UnkReflType* fn_7100aea348();
UnkReflType* fn_7100aea92c();
UnkReflType* fn_7100aeaf10();
extern UnkReflType g_unk_717275b580;
extern UnkReflType g_unk_717275b708;
extern UnkReflType g_unk_717275b848;
extern UnkReflType g_unk_717275b988;
extern UnkTUHeader g_unk_717275bf40 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717275bf60 __attribute__((visibility("hidden")));
void fn_7100aeafec(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
extern UnkConverter g_unk_717275b6c0;
extern UnkConverter g_unk_717275b6e0;
void fn_7100ae96e8(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100aea7a8(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100aead8c(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
