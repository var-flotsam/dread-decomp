// Unit 1728 (tools/tu_map.py, no name hint): the constructor of a record of a string id (starting
// empty), a base::UnkVector (allocator flags 0x58) and two members built out of line (fn_7100068eb4 at
// +0x30, fn_710006b6a4 at +0xf8). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

#include <new>

using base::global::CStrId;

void fn_7100068eb4(void* self);
void fn_710006b6a4(void* self);

struct UnkEmptyStrId_b43a3 {
    union {
        CStrId id;
    };
    UnkEmptyStrId_b43a3() {
        id.data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        id.data = &g_unk_7101d47108->empty;
    }
    ~UnkEmptyStrId_b43a3() { id.~CStrId(); }
};

struct UnkMember30_b43a3 {
    u8 data[0xc8];
    UnkMember30_b43a3() { fn_7100068eb4(this); }
};

struct UnkMemberF8_b43a3 {
    u8 data[0x8];
    UnkMemberF8_b43a3() { fn_710006b6a4(this); }
};

struct UnkUnit1728Record_b43a3 {
    UnkEmptyStrId_b43a3 name;
    base::UnkVector<void*> list{0x58};  // 0x8
    UnkMember30_b43a3 field_0x30;
    UnkMemberF8_b43a3 field_0xf8;
};
static_assert(__builtin_offsetof(UnkUnit1728Record_b43a3, field_0xf8) == 0xf8, "UnkUnit1728Record_b43a3 layout");

__attribute__((flatten)) void fn_7100b8b964(UnkUnit1728Record_b43a3* self) {
    new (self) UnkUnit1728Record_b43a3;
}
