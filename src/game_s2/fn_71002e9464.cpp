// Copy assignment (returns this) of a record: the string id at 0x8 is assigned and the plain
// fields at 0x10..0x33 are copied (an assignment to itself changes nothing).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord_71002e9464 {
    u8 pad_0x0[8];
    CStrId field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    u32 field_0x30;

    UnkRecord_71002e9464& operator=(const UnkRecord_71002e9464& other) {
        if (this == &other)
            return *this;
        field_0x8 = other.field_0x8;
        field_0x10 = other.field_0x10;
        u64 t18 = other.field_0x18;
        u64 t20 = other.field_0x20;
        u64 t28 = other.field_0x28;
        u32 t30 = other.field_0x30;
        field_0x30 = t30;
        field_0x28 = t28;
        field_0x20 = t20;
        field_0x18 = t18;
        return *this;
    }
};

UnkRecord_71002e9464* fn_71002e9464(UnkRecord_71002e9464* self, const UnkRecord_71002e9464* other) {
    return &(*self = *other);
}
