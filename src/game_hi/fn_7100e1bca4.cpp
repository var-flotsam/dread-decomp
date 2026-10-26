// Constructs an object holding three chained hash maps (and a zeroed counter after the first),
// each starting with 11 empty buckets.
#include "base/UnkChainedMap.h"
#include "types.h"

// A chained map (base/UnkChainedMap.h) that starts with 11 empty buckets.
struct UnkChainedMap11 : UnkChainedMap {
    __attribute__((always_inline)) UnkChainedMap11() { ResizeBuckets(11); }
};

struct UnkMapSet {
    UnkChainedMap11 map0;
    u32 field_0x48;
    UnkChainedMap11 map1;
    UnkChainedMap11 map2;
    __attribute__((always_inline)) UnkMapSet() : field_0x48(0) {}
};

void fn_7100e1bca4(UnkMapSet* self) {
    new (self) UnkMapSet();
}
