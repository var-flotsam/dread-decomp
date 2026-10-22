// A record's copy constructor and copy-assignment: twelve entries (UnkAnyPropsS7.h).
#include "UnkAnyPropsS7.h"

using namespace unk_any_props;

// Copied field by field. The first field is two u32s copied as one 8-byte load: an untyped copy,
// which keeps the store of this class's vptr in the copy constructor.
struct UnkRecordMid_710048b740 : UnkRecordBase {
    void vf_0x0() override;
    UnkPair32 field_0x28;
    UnkVal<u32> field_0x30;
    UnkVal<u32> field_0x34;
    UnkVal<u32> field_0x38;

    UnkRecordMid_710048b740(const UnkRecordMid_710048b740& o)
        : UnkRecordBase(o), field_0x28(o.field_0x28), field_0x30(o.field_0x30), field_0x34(o.field_0x34),
          field_0x38(o.field_0x38) {}
    UnkRecordMid_710048b740& operator=(const UnkRecordMid_710048b740& o) = default;
};

struct UnkRecord_710048b740 : UnkRecordMid_710048b740 {
    void vf_0x0() override;
    UnkPropRef field_0x40;
    UnkPropRef field_0x60;
    UnkPropList field_0x80;
    u64 field_0x130[3];
    UnkEntry entry_0;  // 0x148
    UnkEntry entry_1;  // 0x1b8
    UnkEntry entry_2;  // 0x228
    UnkEntry entry_3;  // 0x298
    UnkEntry entry_4;  // 0x308
    UnkEntry entry_5;  // 0x378
    UnkEntry entry_6;  // 0x3e8
    UnkEntry entry_7;  // 0x458
    UnkEntry entry_8;  // 0x4c8
    UnkEntry entry_9;  // 0x538
    UnkEntry entry_10;  // 0x5a8
    UnkEntry entry_11;  // 0x618
    UnkTail field_0x688;
    UnkRecord_710048b740(const UnkRecord_710048b740& o);
    UnkRecord_710048b740& operator=(const UnkRecord_710048b740& o);
};
static_assert(__builtin_offsetof(UnkRecord_710048b740, entry_0) == 0x148, "");
static_assert(sizeof(UnkRecord_710048b740) == 0x718, "");

UnkRecord_710048b740::UnkRecord_710048b740(const UnkRecord_710048b740& o) = default;
UnkRecord_710048b740& UnkRecord_710048b740::operator=(const UnkRecord_710048b740& o) = default;

extern "C" void fn_710048acac() __attribute__((alias("_ZN20UnkRecord_710048b740C2ERKS_")));
extern "C" void fn_710048b740() __attribute__((alias("_ZN20UnkRecord_710048b740aSERKS_")));
