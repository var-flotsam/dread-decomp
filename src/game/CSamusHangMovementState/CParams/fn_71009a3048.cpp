// Copy-assignment of a polymorphic record of plain fields (the vtable is kept). fn_71009a3140 is
// an identical copy.
#include "types.h"

struct UnkSub_0x14 {
    u32 field_0x0[5];

    UnkSub_0x14& operator=(const UnkSub_0x14& other);  // fn_7100125b44
};

// Polymorphic, so a derived class reuses its tail padding (+0x2c).
struct UnkRecordBase {
    virtual void vf0();
    void* field_0x8;
    void* field_0x10;
    void* field_0x18;
    void* field_0x20;
    u32 field_0x28;
};

struct UnkRecord_0xa0 : UnkRecordBase {
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;  // not copied
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    void* field_0x70;
    u32 field_0x78;
    u32 field_0x7c;
    u32 field_0x80;
    UnkSub_0x14 field_0x84;
    void* field_0x98;

    __attribute__((always_inline)) UnkRecord_0xa0& operator=(const UnkRecord_0xa0& other) {
        if (this != &other) {
            UnkRecordBase::operator=(other);
            field_0x2c = other.field_0x2c;
            field_0x30 = other.field_0x30;
            field_0x38 = other.field_0x38;
            field_0x3c = other.field_0x3c;
            field_0x40 = other.field_0x40;
            field_0x44 = other.field_0x44;
            field_0x48 = other.field_0x48;
            field_0x4c = other.field_0x4c;
            field_0x50 = other.field_0x50;
            field_0x54 = other.field_0x54;
            field_0x58 = other.field_0x58;
            field_0x5c = other.field_0x5c;
            field_0x60 = other.field_0x60;
            field_0x64 = other.field_0x64;
            field_0x68 = other.field_0x68;
            field_0x70 = other.field_0x70;
            field_0x78 = other.field_0x78;
            field_0x7c = other.field_0x7c;
            field_0x80 = other.field_0x80;
            // An explicit call: under C++17 `a = b` evaluates `b` first, which the original
            // doesn't.
            field_0x84.operator=(other.field_0x84);
            field_0x98 = other.field_0x98;
        }
        return *this;
    }
};

UnkRecord_0xa0* fn_71009a3048(UnkRecord_0xa0* self, const UnkRecord_0xa0* other) {
    return &(*self = *other);
}

UnkRecord_0xa0* fn_71009a3140(UnkRecord_0xa0* self, const UnkRecord_0xa0* other) {
    return &(*self = *other);
}
