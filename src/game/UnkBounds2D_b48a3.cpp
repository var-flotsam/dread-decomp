// Unit 2565 (tools/tu_map.py, no name hint): the 2D bounds (x and y of each corner, fn_710007b430) of
// the 3D bounds the object at +0x8 reports through its virtual at slot 5, given its own member at
// +0x8. Names and layout are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkBounds3D_b48a3 {
    CVector3D min;
    CVector3D max;
};

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkBounds2D_b48a3 {
    CVector2D min;
    CVector2D max;
    UnkBounds2D_b48a3(const UnkBounds3D_b48a3& b) {
        fn_710007b430(&min, &b.min);
        fn_710007b430(&max, &b.max);
    }
    UnkBounds2D_b48a3(const UnkBounds2D_b48a3& o) : min(o.min), max(o.max) {}
};

class UnkBoundsSource_b48a3 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkBounds3D_b48a3 vf_0x28(void* member);

    u8 field_0x8[1];
};

struct UnkBoundsOwner_b48a3 {
    u8 pad_0x0[0x8];
    UnkBoundsSource_b48a3* source;
};

UnkBounds2D_b48a3 fn_7100e782e0(UnkBoundsOwner_b48a3* self) {
    UnkBoundsSource_b48a3* source = self->source;
    return UnkBounds2D_b48a3(source->vf_0x28(source->field_0x8));
}
