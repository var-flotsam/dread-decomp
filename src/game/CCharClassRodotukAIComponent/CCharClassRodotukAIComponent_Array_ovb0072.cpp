// CCharClassRodotukAIComponent's unit (tools/tu_map.py): more helpers of its reflected array of
// 0x30-byte trivially copyable entries (base/UnkVector.h, base/UnkCollectionIter.h): the iterator
// value's move assignment and grow-to-index. Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkRodotukEntry_ovb0072 {
    u32 words[12];
};
static_assert(sizeof(UnkRodotukEntry_ovb0072) == 0x30, "UnkRodotukEntry_ovb0072 layout");

using UnkRodotukIterValue_ovb0072 = base::UnkIterValue<UnkRodotukEntry_ovb0072, u32>;
using UnkRodotukVector_ovb0072 = base::UnkVector<UnkRodotukEntry_ovb0072>;

void* fn_7100670b98(void* dst, const void* src) {
    UnkRodotukIterValue_ovb0072* d = static_cast<UnkRodotukIterValue_ovb0072*>(dst);
    return &(*d = static_cast<UnkRodotukIterValue_ovb0072&&>(
                 *static_cast<UnkRodotukIterValue_ovb0072*>(const_cast<void*>(src))));
}

UnkRodotukEntry_ovb0072* fn_7100671638(UnkRodotukVector_ovb0072* self, u32 index) { return &self->GrowAt(index); }
