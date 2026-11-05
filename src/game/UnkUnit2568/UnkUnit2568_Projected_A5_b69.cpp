// Unit 2568 (no reflection hint): projects two 3D points onto their x/y plane and forwards them,
// with two flags, to fn_7100ec154c. Names are placeholders.
#include "base/math/CVector.h"

using base::math::CVector2D;
using base::math::CVector3D;

bool fn_7100ec154c(void* self, const CVector2D* a, const CVector2D* b, u32 c, u32 d);

bool fn_7100ec14e4(void* self, const CVector3D* a, const CVector3D* b, u32 c, u32 d) {
    CVector2D pa;
    fn_710007b430(&pa, a);
    CVector2D pb;
    fn_710007b430(&pb, b);
    return fn_7100ec154c(self, &pa, &pb, c, d);
}
