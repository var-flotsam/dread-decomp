// Copy assignment of a record whose two parts live either inside it or, when the mode at +0x50 is 2,
// behind pointers stored at +0x0 and +0x8: copies the first word, assigns the second part (through
// fn_71002cbddc, the string id at +0x28 and a 24-byte block at +0x30).
#include "base/global/CStrId.h"
#include "types.h"

typedef u32 __attribute__((may_alias)) a_u32;
typedef u64 __attribute__((may_alias)) a_u64;

struct UnkBlock {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkPart {
    u8 pad_0x0[0x28];
    base::global::CStrId field_0x28;
    UnkBlock field_0x30;
};

struct UnkRecord {
    u8 pad_0x0[0x50];
    u32 field_0x50;
};

void fn_71002cbddc(UnkPart* dst, UnkPart* src);

static inline a_u32* FirstOf(UnkRecord* record) {
    return record->field_0x50 == 2 ? *reinterpret_cast<a_u32**>(record) : reinterpret_cast<a_u32*>(record);
}

static inline UnkPart* PartOf(UnkRecord* record) {
    UnkPart* part = reinterpret_cast<UnkPart*>(reinterpret_cast<char*>(record) + 8);
    return record->field_0x50 == 2 ? *reinterpret_cast<UnkPart**>(part) : part;
}

UnkRecord* fn_71002ccbe0(UnkRecord* self, UnkRecord* other) {
    if (other != self) {
        *FirstOf(self) = *FirstOf(other);
        UnkPart* src = PartOf(other);
        UnkPart* dst = PartOf(self);
        fn_71002cbddc(dst, src);
        dst->field_0x28 = src->field_0x28;
        dst->field_0x30 = src->field_0x30;
    }
    return self;
}
