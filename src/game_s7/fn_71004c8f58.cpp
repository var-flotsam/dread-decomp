// A record's copy-assignment: four entries (UnkAnyPropsS7.h).
#include "UnkAnyPropsS7.h"

using namespace unk_any_props;

// Copied second field first.
struct UnkPair_71004c8f58 {
    u32 field_0x0;
    u32 field_0x4;

    UnkPair_71004c8f58& operator=(const UnkPair_71004c8f58& o) {
        field_0x4 = o.field_0x4;
        field_0x0 = o.field_0x0;
        return *this;
    }
};

struct UnkRecord_71004c8f58 {
    virtual void vf_0x0();
    UnkBlock20 field_0x8;
    UnkVal<u64> field_0x28;
    UnkVal<u32> field_0x30;
    UnkVal<u32> field_0x34;
    UnkVal<u32> field_0x38;
    UnkVal<u32> field_0x3c;
    UnkVal<u32> field_0x40;
    UnkVal<u32> field_0x44;
    UnkVal<u32> field_0x48;
    UnkVal<u32> field_0x4c;
    UnkPair_71004c8f58 field_0x50;
    u64 field_0x58;
    u8 field_0x60;
    UnkPropList field_0x68;
    u64 field_0x118;
    UnkEntry entries[4];
    UnkBlock10 field_0x2e0;
    UnkVal<u32> field_0x2f0;
    UnkVal<u32> field_0x2f4;
    UnkVal<u32> field_0x2f8;
    u32 field_0x2fc[11];
    UnkPropRef field_0x328;
    u64 field_0x348;
    UnkRecord_71004c8f58& operator=(const UnkRecord_71004c8f58& o);
};
static_assert(__builtin_offsetof(UnkRecord_71004c8f58, entries) == 0x120, "");
static_assert(sizeof(UnkRecord_71004c8f58) == 0x350, "");

UnkRecord_71004c8f58& UnkRecord_71004c8f58::operator=(const UnkRecord_71004c8f58& o) = default;

extern "C" void fn_71004c8f58() __attribute__((alias("_ZN20UnkRecord_71004c8f58aSERKS_")));
