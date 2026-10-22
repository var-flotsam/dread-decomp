// A record's copy-assignment: twelve entries after a header copied field by field (UnkAnyPropsS7.h).
#include "UnkAnyPropsS7.h"

using namespace unk_any_props;

// Copied field by field, the block as one run.
struct UnkHead_710048b740 {
    UnkBlock20 field_0x0;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;

    UnkHead_710048b740& operator=(const UnkHead_710048b740& o) {
        field_0x0 = o.field_0x0;
        field_0x20 = o.field_0x20;
        field_0x28 = o.field_0x28;
        field_0x2c = o.field_0x2c;
        field_0x30 = o.field_0x30;
        return *this;
    }
};

struct UnkRecord_710048b740 {
    virtual void vf_0x0();
    UnkHead_710048b740 field_0x8;
    UnkPropRef field_0x40;
    UnkPropRef field_0x60;
    UnkPropList field_0x80;
    u64 field_0x130[3];
    UnkEntry entries[12];
    UnkTail field_0x688;
    UnkRecord_710048b740& operator=(const UnkRecord_710048b740& o);
};
static_assert(__builtin_offsetof(UnkRecord_710048b740, entries) == 0x148, "");
static_assert(sizeof(UnkRecord_710048b740) == 0x718, "");

UnkRecord_710048b740& UnkRecord_710048b740::operator=(const UnkRecord_710048b740& o) = default;

extern "C" void fn_710048b740() __attribute__((alias("_ZN20UnkRecord_710048b740aSERKS_")));
