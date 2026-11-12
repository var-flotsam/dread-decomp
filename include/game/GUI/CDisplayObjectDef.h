#pragma once

// CDisplayObjectDef: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1669,
// 0x7100b04a58-0x7100b0a450). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkDisplayReader_ovb0108;
struct UnkReflType;
struct UnkTUHeaderFnsClass;

void fn_7100b05704();
void fn_7100b05ce8();
extern char g_unk_717275e1f8;
extern char g_unk_717275e1f9;
extern char g_unk_717275e1fa;
extern char g_unk_717275e1fb;
extern char g_unk_717275e1fc;
extern char g_unk_717275e338;
extern char g_unk_717275e339;
extern char g_unk_717275e478;
extern char g_unk_717275e479;
extern char g_unk_717275e5f0;
UnkReflType* fn_7100b05394();  // base::reflection::refl_GetType<GUI::CDisplayObjectDef*>.
UnkReflType* fn_7100b05470();
UnkReflType* fn_7100b0554c();
UnkReflType* fn_7100b05628();
UnkReflType* fn_7100b05c0c();
UnkReflType* fn_7100b061f0();
UnkReflType* fn_7100b08378();  // base::reflection::refl_GetType<GUI::CDisplayObjectStateDef*>.
extern UnkReflType g_unk_717275e200;
extern UnkReflType g_unk_717275e340;
extern UnkReflType g_unk_717275e480;
extern UnkTUHeaderFnsClass g_unk_717275ed68 __attribute__((visibility("hidden")));
void fn_7100b062cc(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
s32 fn_7100b09720(UnkDisplayReader_ovb0108* self, base::reflection::UnkReflObject* obj);
void fn_7100b05a88(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b0606c(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
