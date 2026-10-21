// Constructor of a table of 15 entries: each holds two string ids copied from a global default,
// four floats set to 1, two cleared words, a count of 1, a float of -1, a -1 sentinel and a flag.
#include "base/global/CStrId.h"
#include "types.h"

#include <new>

using base::global::CStrId;

extern CStrId g_unk_7101d470e0;  // a global string id

struct UnkEntry_71007bf550 {
    CStrId field_0x0 = g_unk_7101d470e0;
    CStrId field_0x8 = g_unk_7101d470e0;
    f32 field_0x10[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    u64 field_0x20 = 0;
    u64 field_0x28 = 0;
    u32 field_0x30 = 1;
    f32 field_0x34 = -1.0f;
    s64 field_0x38 = -1;
    bool field_0x40 = false;
};

struct UnkTable_71007bf550 {
    u32 field_0x0 = 0;
    UnkEntry_71007bf550 entries[15];

    __attribute__((always_inline)) UnkTable_71007bf550() {}
};

void fn_71007bf550(UnkTable_71007bf550* self) {
    new (self) UnkTable_71007bf550;
}
