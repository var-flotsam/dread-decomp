// CCutSceneDef unit (tools/tu_map.py): copy constructor of a record of two CStrIds and a u32. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

#include <new>

struct UnkStrIdPairRec_A6 {
    base::global::CStrId first;
    base::global::CStrId second;
    u32 field_0x10;
};

// The reflected copy constructor (slot 0x30 in src/reflection/CCutSceneDef.cpp).
void fn_7100c8f73c(void* dst, const void* src) {
    new (dst) UnkStrIdPairRec_A6(*static_cast<const UnkStrIdPairRec_A6*>(src));
}
