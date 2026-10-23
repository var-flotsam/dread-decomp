// Copy constructors of a 0x40-byte record: plain fields up to 0x2c, a string id at 0x30 (retained
// by base::global::CStrId's copy) and a word at 0x38 (fn_71001b3a3c, fn_71001b3a94). They have no
// direct callers.

#include <new>

#include "base/global/CStrId.h"
#include "types.h"

struct UnkIdRecord_71001b3a3c {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    base::global::CStrId id;
    u64 field_0x38;
};

void fn_71001b3a3c(UnkIdRecord_71001b3a3c* self, const UnkIdRecord_71001b3a3c* other) {
    new (self) UnkIdRecord_71001b3a3c(*other);
}

void fn_71001b3a94(UnkIdRecord_71001b3a3c* self, const UnkIdRecord_71001b3a3c* other) {
    new (self) UnkIdRecord_71001b3a3c(*other);
}
