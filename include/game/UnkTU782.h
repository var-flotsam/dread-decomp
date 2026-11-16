#pragma once

// Unit 782 of tools/tu_map.py. Declarations only; names are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "types.h"

// The blackboard property name of a marker, built in place from an owner and the marker's key: 16 bytes,
// the name's string first, then the owner.
void fn_71004f1748(base::reflection::UnkReflRegistryMap::Value* value, base::reflection::UnkReflRegistryItem* item,
                   const base::global::CStrId& name);
