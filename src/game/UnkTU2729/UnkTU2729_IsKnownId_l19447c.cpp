// Unit 2729: whether a string id is one of eleven fixed names of the reflection id table.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "types.h"

using base::global::CStrId;

static inline const CStrId& TableId(u32 index) {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[index];
}

bool fn_710119447c(const CStrId& id) {
    return id == TableId(0xdc) || id == TableId(0xdd) || id == TableId(0xde) || id == TableId(0x22c) ||
           id == TableId(0x22d) || id == TableId(0x4ad) || id == TableId(0x4ae) || id == TableId(0x340) ||
           id == TableId(0x341) || id == TableId(0x3a0) || id == TableId(0x2fb);
}
