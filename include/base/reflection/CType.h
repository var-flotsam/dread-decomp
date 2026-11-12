#pragma once

// CType: functions and globals of this translation unit that other files use (tools/tu_map.py unit 81,
// 0x710009c7b4-0x71000a16c4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

struct UnkVersionEntry;
struct UnkHashed;
u32 fn_710009d110(UnkHashed* self);
void fn_710009cc04(char*);
extern base::reflection::CType g_unk_7101d5af48;  // The void type.
// The attribute stored under `key`, as a reflected value (empty when absent).
base::reflection::UnkReflValue fn_710009ddd4(UnkVersionEntry* self, const base::reflection::UnkReflAttrKey& key);
