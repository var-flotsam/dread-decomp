#pragma once

// UnkTU1176: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1176,
// 0x710076cf54-0x710077c5ec). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkEntry_cu;
struct UnkNamedEntry_7100772cb0;
struct UnkNamedList_7100772cb0;
struct UnkCentralUnit;
struct UnkProperty;
struct UnkCentralUnitList;
struct UnkCentralUnitProto;
struct UnkCentralUnit_b46a2;
struct UnkProtoCentralUnitBase;
struct UnkScenarioObject_ovb0032;
UnkCentralUnitList* fn_7100772c98();
void* fn_7100774014(UnkScenarioObject_ovb0032* actor);
void fn_710076e964(UnkProtoCentralUnitBase* self);
void fn_71007722f4(UnkCentralUnit_b46a2* self, f32 value);
void fn_710077561c(UnkCentralUnitProto* unit);
extern UnkProperty g_unk_71727141f8;
extern UnkProperty g_unk_7172714220;
void fn_7100772610(base::reflection::UnkReflObject* component, const base::global::CStrId* id, f32 a, f32 b);
void fn_7100772698(base::reflection::UnkReflObject* component, const base::global::CStrId* id, f32 value);
UnkNamedEntry_7100772cb0* fn_7100772cb0(UnkNamedList_7100772cb0* self, const base::global::CStrId* id);
void fn_7100775610(UnkCentralUnit* unit, bool on, const base::global::CStrId* id);
void fn_710077a700(base::UnkVector<UnkEntry_cu>* self, const UnkEntry_cu* value);
