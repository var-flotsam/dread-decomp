// A record's copy-assignment: twelve entries (UnkAnyPropsS7.h).
#include "UnkAnyPropsS7.h"

using namespace unk_any_props;

struct UnkRecord_7100442780 {
    virtual void vf_0x0();
    UnkBlock40 field_0x8;
    UnkPropRef field_0x48;
    UnkPropRef field_0x68;
    UnkPropList field_0x88;
    u64 field_0x138[3];
    UnkEntry entries[12];
    UnkTail field_0x690;
    UnkRecord_7100442780& operator=(const UnkRecord_7100442780& o);
};
static_assert(__builtin_offsetof(UnkRecord_7100442780, entries) == 0x150, "");
static_assert(sizeof(UnkRecord_7100442780) == 0x720, "");

UnkRecord_7100442780& UnkRecord_7100442780::operator=(const UnkRecord_7100442780& o) = default;

extern "C" void fn_7100442780() __attribute__((alias("_ZN20UnkRecord_7100442780aSERKS_")));
