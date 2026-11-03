// Unit 55 (0x710007b430-0x710007bb30, no class hint): the dot product of two 4-component float
// vectors. Names are placeholders.
#include "types.h"

struct UnkVec4f {
    f32 x, y, z, w;
};

f32 fn_710007b590(const UnkVec4f& a, const UnkVec4f& b) {
    return b.x * a.x + b.y * a.y + b.z * a.z + b.w * a.w;
}
