#pragma once

// CBaseObject: functions of this translation unit that other files call (tools/tu_map.py unit 29,
// 0x7100023460-0x7100024fd4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkCameraCtrl;
struct UnkNode;
void fn_7100023854(UnkNode* node);
void fn_7100023838(void* self);
void fn_710002384c(void* self);  // The base destructor of the unit's polymorphic elements.
bool fn_7100023d00(const UnkCameraCtrl* object, const base::reflection::CClass* cls);
s32 fn_7100024140(const base::reflection::CType* type);
