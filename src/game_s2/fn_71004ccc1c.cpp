// Copy assignment of a record of plain fields (an assignment to itself changes nothing).
#include "types.h"

struct UnkPair {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkQuad {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkRecord_71004ccc1c {
    u8 pad_0x0[8];
    UnkQuad field_0x8;
    u64 field_0x28;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u64 field_0x58;
    u8 field_0x60;
    u8 pad_0x61[7];
    UnkPair field_0x68;
    u64 field_0x78;
    u64 field_0x80;
    u64 field_0x88;
    u32 field_0x90;

    UnkRecord_71004ccc1c& operator=(const UnkRecord_71004ccc1c& other) {
        if (this == &other)
            return *this;
        field_0x8 = other.field_0x8;
        field_0x28 = other.field_0x28;
        field_0x30 = other.field_0x30;
        field_0x34 = other.field_0x34;
        field_0x38 = other.field_0x38;
        field_0x3c = other.field_0x3c;
        field_0x40 = other.field_0x40;
        field_0x44 = other.field_0x44;
        field_0x48 = other.field_0x48;
        field_0x4c = other.field_0x4c;
        field_0x54 = other.field_0x54;
        field_0x50 = other.field_0x50;
        u64 t58 = other.field_0x58;
        field_0x60 = other.field_0x60;
        field_0x58 = t58;
        u64 t78 = other.field_0x78;
        u64 t80 = other.field_0x80;
        u64 t88 = other.field_0x88;
        u32 t90 = other.field_0x90;
        field_0x90 = t90;
        field_0x88 = t88;
        field_0x80 = t80;
        field_0x78 = t78;
        field_0x68 = other.field_0x68;
        return *this;
    }
};

UnkRecord_71004ccc1c* fn_71004ccc1c(UnkRecord_71004ccc1c* self, const UnkRecord_71004ccc1c* other) {
    return &(*self = *other);
}
