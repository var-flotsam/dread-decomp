// A record's copy constructor and copy-assignment: thirteen entries, then values copied one by one and a string id (UnkAnyPropsS7.h).
#include "UnkAnyPropsS7.h"
#include "base/global/CStrId.h"

using namespace unk_any_props;

struct UnkRecord_7100510548 : UnkRecordBase {
    void vf_0x0() override;
    UnkPropList field_0x28;
    u64 field_0xd8;
    UnkPropRef field_0xe0;
    // The u64 and the block after the entries are plain fields: the last entry's block is then
    // copied in another order.
    UnkEntry entry_0;
    UnkEntry entry_1;
    UnkEntry entry_2;
    UnkEntry entry_3;
    UnkEntry entry_4;
    UnkEntry entry_5;
    UnkEntry entry_6;
    UnkEntry entry_7;
    UnkEntry entry_8;
    UnkEntry entry_9;
    UnkEntry entry_10;
    UnkEntry entry_11;
    UnkEntry entry_12;
    u64 field_0x6b0;
    UnkBlock20 field_0x6b8;
    UnkVal<u32> field_0x6d8;
    UnkVal<u32> field_0x6dc;
    UnkVal<u32> field_0x6e0;
    UnkVal<u32> field_0x6e4;
    UnkVal<u32> field_0x6e8;
    UnkVal<u32> field_0x6ec;
    UnkBlock10 field_0x6f0;
    UnkVal<u32> field_0x700;
    UnkVal<u32> field_0x704;
    UnkVal<u32> field_0x708;
    UnkVal<u32> field_0x70c;
    UnkVal<u32> field_0x710;
    UnkVal<u32> field_0x714;
    UnkVal<u32> field_0x718;
    UnkVal<u32> field_0x71c;
    UnkVal<u32> field_0x720;
    UnkVal<u32> field_0x724;
    UnkVal<u32> field_0x728;
    UnkVal<u32> field_0x72c;
    UnkVal<u32> field_0x730;
    UnkVal<u32> field_0x734;
    UnkVal<u32> field_0x738;
    UnkVal<u32> field_0x73c;
    UnkVal<u32> field_0x740;
    UnkVal<u32> field_0x744;
    UnkVal<u32> field_0x748;
    UnkVal<u32> field_0x74c;
    UnkVal<u32> field_0x750;
    UnkVal<u32> field_0x754;
    UnkVal<u32> field_0x758;
    UnkVal<u32> field_0x75c;
    UnkVal<u32> field_0x760;
    UnkVal<u32> field_0x764;
    UnkVal<u32> field_0x768;
    UnkVal<u32> field_0x76c;
    UnkVal<u32> field_0x770;
    UnkVal<u32> field_0x774;
    UnkVal<u32> field_0x778;
    UnkVal<u32> field_0x77c;
    UnkVal<u32> field_0x780;
    UnkVal<u32> field_0x784;
    UnkVal<u64> field_0x788;
    UnkBlock10 field_0x790;
    base::global::CStrId field_0x7a0;
    u64 field_0x7a8;
    u64 field_0x7b0;
    u32 field_0x7b8;
    UnkRecord_7100510548(const UnkRecord_7100510548& o);
    UnkRecord_7100510548& operator=(const UnkRecord_7100510548& o);
};
static_assert(__builtin_offsetof(UnkRecord_7100510548, entry_0) == 0x100, "");
static_assert(__builtin_offsetof(UnkRecord_7100510548, field_0x7a0) == 0x7a0, "");

UnkRecord_7100510548::UnkRecord_7100510548(const UnkRecord_7100510548& o) = default;
UnkRecord_7100510548& UnkRecord_7100510548::operator=(const UnkRecord_7100510548& o) = default;

extern "C" void fn_7100510548() __attribute__((alias("_ZN20UnkRecord_7100510548aSERKS_")));
extern "C" void fn_710050f824() __attribute__((alias("_ZN20UnkRecord_7100510548C2ERKS_")));
