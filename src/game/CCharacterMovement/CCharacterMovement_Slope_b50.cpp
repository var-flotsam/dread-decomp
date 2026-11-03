// CCharacterMovement unit (tools/tu_map.py, the name from reflection): asks the object at +0x158 of the
// owner for a pair of 3D vectors (virtual slot 5, given its sub-object at +8), projects both onto the
// XY plane and returns the negated Y of the first. Names are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkVectorPair7c88 {
    CVector3D a;
    CVector3D b;
};

struct UnkMovementShape7c88 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkVectorPair7c88 vf_0x28(void* sub);
    char field_0x8[8];
};

f32 fn_7100977c88(void*, char* owner) {
    UnkMovementShape7c88* shape = *reinterpret_cast<UnkMovementShape7c88**>(owner + 0x158);
    CVector2D xy[2];
    UnkVectorPair7c88 pair = shape->vf_0x28(shape->field_0x8);
    fn_710007b430(&xy[0], &pair.a);
    fn_710007b430(&xy[1], &pair.b);
    return 0.0f - xy[0].y;
}
