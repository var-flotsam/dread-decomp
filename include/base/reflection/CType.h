#pragma once

// CType: functions and globals of this translation unit that other files use (tools/tu_map.py unit 81,
// 0x710009c7b4-0x71000a16c4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/TypeObject.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "base/UnkCloneHelpers.h"
#include "types.h"

struct UnkVersionEntry;
struct UnkHashed;
u32 CType_ComputeBaseHash(UnkHashed* self);
void fn_710009cc04(char*);
// The void type.
template <> base::reflection::CType base::reflection::TypeObject<void>::object asm("g_unk_7101d5af48");
// The attribute stored under `key`, as a reflected value (empty when absent).
base::reflection::UnkReflValue CType_GetAttributeValue(UnkVersionEntry* self, const base::reflection::UnkReflAttrKey& key);
// The reflected value's copy-assignment.
bool CType_AssignOrConvertValue(const base::reflection::CType* type, base::reflection::UnkTypedRef* src, base::reflection::UnkTypedRef* dst);
bool CType_ObjectIsKindOf(const base::reflection::UnkReflObject* obj, const base::reflection::CType* type);
// The value a typed reference holds, as a reflected value.
bool CType_AreValuesEqual(const char* type, base::reflection::UnkTypedRef* dst, const base::reflection::UnkTypedRef* src);
