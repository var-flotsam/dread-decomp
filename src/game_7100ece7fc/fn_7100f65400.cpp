// Copy-assignment of a record holding three 4-float values (two identical copies): copies field
// by field, the 4-float values through their out-of-line assignment.
#include "types.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"

using base::math::CVector4D;

struct UnkRecord_0x40_f65400 {
    u32 field_0x0;
    CVector4D field_0x4;
    CVector4D field_0x14;
    CVector4D field_0x24;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;

    UnkRecord_0x40_f65400& Assign(const UnkRecord_0x40_f65400& other) {
        if (this != &other) {
            field_0x0 = other.field_0x0;
            base::math::Assign(field_0x4, other.field_0x4);
            base::math::Assign(field_0x14, other.field_0x14);
            base::math::Assign(field_0x24, other.field_0x24);
            field_0x34 = other.field_0x34;
            field_0x38 = other.field_0x38;
            field_0x3c = other.field_0x3c;
        }
        return *this;
    }
};

UnkRecord_0x40_f65400& fn_7100f65400(UnkRecord_0x40_f65400* self, const UnkRecord_0x40_f65400* other) {
    return self->Assign(*other);
}

UnkRecord_0x40_f65400& fn_7100f6546c(UnkRecord_0x40_f65400* self, const UnkRecord_0x40_f65400* other) {
    return self->Assign(*other);
}
