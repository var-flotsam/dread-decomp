// Move constructor of a keyed holder (UnkKeyedHolder.h) whose record is two 0x40-byte members
// moved out of line (fn_71002458a4).
#include "UnkKeyedHolder.h"

struct UnkMember_71002458a4 {
    u64 unk_0x0[8];
    UnkMember_71002458a4(UnkMember_71002458a4&& other);
};

struct UnkRecord_7100779ad0 {
    UnkMember_71002458a4 field_0x0;
    UnkMember_71002458a4 field_0x40;
};

using UnkHolder_7100779ad0 = UnkKeyedHolder<u32, UnkRecord_7100779ad0>;

void fn_7100779ad0(UnkHolder_7100779ad0* self, UnkHolder_7100779ad0* other) {
    new (self) UnkHolder_7100779ad0(std::move(*other));
}
