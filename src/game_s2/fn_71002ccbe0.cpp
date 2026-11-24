// Copy assignment of a record whose two parts live either inside it or, when the mode at +0x50 is 2,
// behind pointers stored at +0x0 and +0x8: copies the first word, assigns the second part (through
// fn_71002cbddc, the string id at +0x28 and a 24-byte block at +0x30).
#include "base/global/CStrId.h"
#include "types.h"

typedef u32 a_u32;
typedef u64 a_u64;

struct UnkBlock_2ccbe0 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkPart_2ccbe0 {
    u8 pad_0x0[0x28];
    base::global::CStrId field_0x28;
    UnkBlock_2ccbe0 field_0x30;
};

struct UnkRecord_2ccbe0 {
    u8 pad_0x0[0x8];
    UnkPart_2ccbe0 field_0x8;  // inline unless the mode is 2 (then a pointer to it)
    u32 field_0x50;
};

UnkPart_2ccbe0* fn_71002cbddc(UnkPart_2ccbe0* dst, UnkPart_2ccbe0* src);

static inline a_u32* FirstOf(UnkRecord_2ccbe0* record) {
    return record->field_0x50 == 2 ? *reinterpret_cast<a_u32**>(record) : reinterpret_cast<a_u32*>(record);
}

static inline UnkPart_2ccbe0* PartOf(UnkRecord_2ccbe0* record) {
    UnkPart_2ccbe0* part = &record->field_0x8;
    return record->field_0x50 == 2 ? *reinterpret_cast<UnkPart_2ccbe0**>(part) : part;
}

UnkRecord_2ccbe0* fn_71002ccbe0(UnkRecord_2ccbe0* self, UnkRecord_2ccbe0* other) {
    if (other != self) {
        *FirstOf(self) = *FirstOf(other);
        UnkPart_2ccbe0* src = PartOf(other);
        UnkPart_2ccbe0* dst = PartOf(self);
        fn_71002cbddc(dst, src);
        dst->field_0x28 = src->field_0x28;
        dst->field_0x30 = src->field_0x30;
    }
    return self;
}
