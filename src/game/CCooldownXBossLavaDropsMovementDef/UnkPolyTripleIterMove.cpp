// The move assignments of two reflected iterator values (base/UnkCollectionIter.h, u32 index)
// over polymorphic 0x18-byte elements whose assignment copies what follows the vtable:
// fn_71004b20f4 (unit 741 of tools/tu_map.py, CCooldownXBossLavaDropsMovementDef's; three words) and
// fn_71004bfd04 (unit 744, CCooldownXBossFireBallDef's; two words and a byte). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "types.h"

struct UnkPolyTripleBase {
    virtual ~UnkPolyTripleBase();
};

struct UnkPolyTriple : UnkPolyTripleBase {
    struct {
        u32 words[3];
    } field_0x8;
};
static_assert(sizeof(UnkPolyTriple) == 0x18, "UnkPolyTriple layout");

// fn_71004bfd04's element: two words and a byte after the vtable.
struct UnkPolyPairByte : UnkPolyTripleBase {
    u32 field_0x8[2];
    u8 field_0x10;
};
static_assert(sizeof(UnkPolyPairByte) == 0x18, "UnkPolyPairByte layout");

using UnkPolyPairByteIterValue = base::UnkIterValue<UnkPolyPairByte, u32>;
using UnkPolyTripleIterValue = base::UnkIterValue<UnkPolyTriple, u32>;

void* fn_71004b20f4(void* dst, const void* src) {
    UnkPolyTripleIterValue* d = static_cast<UnkPolyTripleIterValue*>(dst);
    return &(*d = static_cast<UnkPolyTripleIterValue&&>(*static_cast<UnkPolyTripleIterValue*>(const_cast<void*>(src))));
}
void* fn_71004bfd04(void* dst, const void* src) {
    UnkPolyPairByteIterValue* d = static_cast<UnkPolyPairByteIterValue*>(dst);
    return &(*d = static_cast<UnkPolyPairByteIterValue&&>(*static_cast<UnkPolyPairByteIterValue*>(const_cast<void*>(src))));
}
