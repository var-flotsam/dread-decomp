// A record's copy constructor and copy-assignment: fn_7100442780's layout with seven entries (UnkAnyPropsS7.h).
#include "UnkAnyPropsS7.h"

using namespace unk_any_props;

struct UnkRecordMid_710043439c : UnkRecordBase {
    void vf_0x0() override;
    u64 field_0x28[4];
};

struct UnkRecord_710043439c : UnkRecordMid_710043439c {
    void vf_0x0() override;
    UnkPropRef field_0x48;
    UnkPropRef field_0x68;
    UnkPropList field_0x88;
    u64 field_0x138[3];
    UnkEntry entry_0;
    UnkEntry entry_1;
    UnkEntry entry_2;
    UnkEntry entry_3;
    UnkEntry entry_4;
    UnkEntry entry_5;
    UnkEntry entry_6;
    UnkTail field_0x460;
    UnkRecord_710043439c(const UnkRecord_710043439c& o);
    UnkRecord_710043439c& operator=(const UnkRecord_710043439c& o);
};
static_assert(__builtin_offsetof(UnkRecord_710043439c, entry_0) == 0x150, "");
static_assert(sizeof(UnkRecord_710043439c) == 0x4f0, "");

UnkRecord_710043439c::UnkRecord_710043439c(const UnkRecord_710043439c& o) = default;
UnkRecord_710043439c& UnkRecord_710043439c::operator=(const UnkRecord_710043439c& o) = default;

extern "C" void fn_710043439c() __attribute__((alias("_ZN20UnkRecord_710043439caSERKS_")));
extern "C" void fn_7100433c5c() __attribute__((alias("_ZN20UnkRecord_710043439cC2ERKS_")));
