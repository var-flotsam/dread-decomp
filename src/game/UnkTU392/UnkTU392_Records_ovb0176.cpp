// Unit 392 (no reflected class): a vector of 0x40-byte records that each need tearing down. Names and
// layouts are placeholders.
#include "game/UnkTU377.h"
#include "types.h"

struct UnkRecord_ovb0176 {
    u8 data[0x40];
};

struct UnkRefVectorHolder_b47;

struct UnkRecordVector_ovb0176 {
    u8 pad_0x0[0x18];
    UnkRecord_ovb0176* records;  // 0x18
    u32 count;                   // 0x20
};
static_assert(__builtin_offsetof(UnkRecordVector_ovb0176, count) == 0x20, "layout");

void fn_71002c5b9c(UnkRecordVector_ovb0176* self) {
    for (u64 i = 0; i < self->count; ++i)
        fn_71002b67e8(reinterpret_cast<UnkRefVectorHolder_b47*>(&self->records[i]));
    self->count = 0;
}
