// Unit 2565 (tools/tu_map.py): the reflected copy constructor (slot 0x30) of a 0x5f78-byte type: a
// string id, four words, then a secondary base (vtable g_unk_7101aa9d38 at +0x18; its destructor is
// fn_7100e7c4fc) whose plain data is copied as one block. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

extern void* g_unk_7101aa9d38[];

struct UnkUnit2565Base_A6_b74 {
    char* vtable;
    u8 data[0x5f54];

    UnkUnit2565Base_A6_b74(const UnkUnit2565Base_A6_b74& o) : vtable(reinterpret_cast<char*>(g_unk_7101aa9d38) + 0x10) {
        __builtin_memcpy(data, o.data, sizeof(data));
    }
};

struct UnkUnit2565Value_A6_b74 {
    base::global::CStrId id;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    UnkUnit2565Base_A6_b74 base;

    UnkUnit2565Value_A6_b74(const UnkUnit2565Value_A6_b74& o)
        : id(o.id), field_0x8(o.field_0x8), field_0xc(o.field_0xc), field_0x10(o.field_0x10), field_0x14(o.field_0x14),
          base(o.base) {}
};
static_assert(sizeof(UnkUnit2565Value_A6_b74) == 0x5f78, "UnkUnit2565Value_A6_b74 layout");

void fn_7100e7c434(void* dst, const void* src) {
    new (dst) UnkUnit2565Value_A6_b74(*static_cast<const UnkUnit2565Value_A6_b74*>(src));
}
