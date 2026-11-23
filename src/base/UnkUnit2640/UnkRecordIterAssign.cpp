// From unit 2640 of tools/tu_map.py.
// Shaped as fn_710011e718 (src/base/UnkUnit108/UnkPartsIterAssign.cpp): the copy assignment of a
// reflected iterator value (base/UnkCollectionIter.h) with a u32 index and a 0x50-byte record: a
// word, a part assigned by fn_7100ffbad8, then plain fields (an implicit copy assignment). Names
// and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkPartFfca00.h"
#include "base/UnkRecordFfca00.h"
#include "game/UnkTU2640.h"
#include "types.h"

struct UnkPartFfca00;

void* fn_7100ffca00(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkRecordFfca00, u32>;
    return &(*static_cast<V*>(dst) = *static_cast<const V*>(src));
}
