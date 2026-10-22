// A record's copy constructor and copy-assignment: twelve entries (UnkAnyPropsS7.h).
#include "UnkAnyPropsS7.h"

using namespace unk_any_props;

struct UnkRecordMid_7100442780 : UnkRecordBase {
    void vf_0x0() override;
    u64 field_0x28[4];
};

struct UnkRecord_7100442780 : UnkRecordMid_7100442780 {
    void vf_0x0() override;
    UnkPropRef field_0x48;
    UnkPropRef field_0x68;
    UnkPropList field_0x88;
    u64 field_0x138[3];
    UnkEntry entry_0;  // 0x150
    UnkEntry entry_1;  // 0x1c0
    UnkEntry entry_2;  // 0x230
    UnkEntry entry_3;  // 0x2a0
    UnkEntry entry_4;  // 0x310
    UnkEntry entry_5;  // 0x380
    UnkEntry entry_6;  // 0x3f0
    UnkEntry entry_7;  // 0x460
    UnkEntry entry_8;  // 0x4d0
    UnkEntry entry_9;  // 0x540
    UnkEntry entry_10;  // 0x5b0
    UnkEntry entry_11;  // 0x620
    UnkTail field_0x690;
    UnkRecord_7100442780(const UnkRecord_7100442780& o);
    UnkRecord_7100442780& operator=(const UnkRecord_7100442780& o);
};
static_assert(__builtin_offsetof(UnkRecord_7100442780, entry_0) == 0x150, "");
static_assert(sizeof(UnkRecord_7100442780) == 0x720, "");

UnkRecord_7100442780::UnkRecord_7100442780(const UnkRecord_7100442780& o) = default;
UnkRecord_7100442780& UnkRecord_7100442780::operator=(const UnkRecord_7100442780& o) = default;

extern "C" void fn_7100441cfc() __attribute__((alias("_ZN20UnkRecord_7100442780C2ERKS_")));
extern "C" void fn_7100442780() __attribute__((alias("_ZN20UnkRecord_7100442780aSERKS_")));
