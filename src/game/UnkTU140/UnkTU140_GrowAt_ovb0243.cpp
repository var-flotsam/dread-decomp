// Unit 140 of tools/tu_map.py (see game/UnkSceneObject.h): the element at an index of an array of
// 16-byte records, growing the array first when the index is past its end (UnkVector::GrowAt). The
// new records are zeroed; the old ones move over word by word.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"
#include "types.h"

typedef u32 __attribute__((may_alias)) a_u32_9230;

struct UnkRecord16_9230 {
    u32 words[4];

    UnkRecord16_9230() : words{0, 0, 0, 0} {}
    UnkRecord16_9230(UnkRecord16_9230&& o) {
        ((a_u32_9230*)this)[0] = ((const a_u32_9230*)&o)[0];
        ((a_u32_9230*)this)[1] = ((const a_u32_9230*)&o)[1];
        ((a_u32_9230*)this)[2] = ((const a_u32_9230*)&o)[2];
        ((a_u32_9230*)this)[3] = ((const a_u32_9230*)&o)[3];
    }
};

UnkRecord16_9230* fn_7100149230(base::UnkVector<UnkRecord16_9230>* self, u32 index) {
    return &self->GrowAt(index);
}
