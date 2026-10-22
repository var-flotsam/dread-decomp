// A record's copy-assignment: fn_7100442780's layout with seven entries (UnkAnyPropsS7.h).
#include "UnkAnyPropsS7.h"

using namespace unk_any_props;

struct UnkRecord_710043439c {
    virtual void vf_0x0();
    UnkBlock40 field_0x8;
    UnkPropRef field_0x48;
    UnkPropRef field_0x68;
    UnkPropList field_0x88;
    u64 field_0x138[3];
    UnkEntry entries[7];
    UnkTail field_0x460;
    UnkRecord_710043439c& operator=(const UnkRecord_710043439c& o);
};
static_assert(__builtin_offsetof(UnkRecord_710043439c, entries) == 0x150, "");
static_assert(sizeof(UnkRecord_710043439c) == 0x4f0, "");

UnkRecord_710043439c& UnkRecord_710043439c::operator=(const UnkRecord_710043439c& o) = default;

extern "C" void fn_710043439c() __attribute__((alias("_ZN20UnkRecord_710043439caSERKS_")));
