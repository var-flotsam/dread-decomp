// Copy-assignment of a record holding three 4-float values (two identical copies): copies field
// by field, the 4-float values through their out-of-line assignment.
#include "types.h"

struct UnkVec4 {
    f32 x, y, z, w;
    UnkVec4& operator=(const UnkVec4& other);  // fn_710007b518
};

struct UnkRecord_0x40 {
    u32 field_0x0;
    UnkVec4 field_0x4;
    UnkVec4 field_0x14;
    UnkVec4 field_0x24;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;

    UnkRecord_0x40& Assign(const UnkRecord_0x40& other) {
        // Member calls: the target's address is computed before the source's.
        if (this != &other) {
            field_0x0 = other.field_0x0;
            field_0x4.operator=(other.field_0x4);
            field_0x14.operator=(other.field_0x14);
            field_0x24.operator=(other.field_0x24);
            field_0x34 = other.field_0x34;
            field_0x38 = other.field_0x38;
            field_0x3c = other.field_0x3c;
        }
        return *this;
    }
};

UnkRecord_0x40& fn_7100f65400(UnkRecord_0x40* self, const UnkRecord_0x40* other) {
    return self->Assign(*other);
}

UnkRecord_0x40& fn_7100f6546c(UnkRecord_0x40* self, const UnkRecord_0x40* other) {
    return self->Assign(*other);
}
