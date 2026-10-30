// Copy constructor of the object destroyed by fn_71002b6094: copies the plain fields and the
// string id, deep-copies the arrays, sets the first flag and leaves the refcounted pointer null.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkObject_0x100.h"
#include "types.h"

using base::global::CStrId;

// A growable array of 16-bit values whose copy constructor is inlined here.
struct UnkStruct_71002b58c0_U16Vec {
    u16* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkStruct_71002b58c0_U16Vec(const UnkStruct_71002b58c0_U16Vec& o)
        : data(nullptr), count(0), capacity(0), step(o.step), field_0x14(o.field_0x14), flags(o.flags),
          allocator(o.allocator) {
        if (u32 n = o.capacity) {
            u16* p = static_cast<u16*>(allocator->vf_0x40((u64)n * sizeof(u16), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(u16));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
        __builtin_memcpy(data, o.data, (u64)o.count * sizeof(u16));
        count = o.count;
    }
};

// The other arrays, copied out of line.
struct UnkStruct_71002b58c0_Vec {
    u8 raw[0x28];
    UnkStruct_71002b58c0_Vec(const UnkStruct_71002b58c0_Vec& o) asm("fn_710027e258");  // fn_710027e258
};

struct UnkStruct_71002b58c0 {
    bool field_0x0;
    u8 field_0x1;
    u16 field_0x2;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u8 field_0x1c;
    u16 field_0x1e;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    CStrId field_0x38;
    u8 field_0x40;
    u8 field_0x41;
    u32 field_0x44;
    u32 field_0x48;
    u8 field_0x4c;
    u8 field_0x4d;
    u32 field_0x50;
    UnkStruct_71002b58c0_U16Vec field_0x58;
    UnkStruct_71002b58c0_Vec field_0x80;
    UnkStruct_71002b58c0_Vec field_0xa8;
    UnkStruct_71002b58c0_Vec field_0xd0;
    void* field_0xf8;

    __attribute__((always_inline)) UnkStruct_71002b58c0(const UnkStruct_71002b58c0& o)
        : field_0x0(true), field_0x1(o.field_0x1), field_0x2(o.field_0x2), field_0x4(o.field_0x4),
          field_0x8(o.field_0x8), field_0xc(o.field_0xc), field_0x10(o.field_0x10), field_0x14(o.field_0x14),
          field_0x18(o.field_0x18), field_0x1c(o.field_0x1c), field_0x1e(o.field_0x1e),
          field_0x20(o.field_0x20), field_0x24(o.field_0x24), field_0x28(o.field_0x28),
          field_0x2c(o.field_0x2c), field_0x30(o.field_0x30), field_0x34(o.field_0x34),
          field_0x38(o.field_0x38), field_0x40(o.field_0x40), field_0x41(o.field_0x41),
          field_0x44(o.field_0x44), field_0x48(o.field_0x48), field_0x4c(o.field_0x4c),
          field_0x4d(o.field_0x4d), field_0x50(o.field_0x50), field_0x58(o.field_0x58),
          field_0x80(o.field_0x80), field_0xa8(o.field_0xa8), field_0xd0(o.field_0xd0),
          field_0xf8(nullptr) {}
};

void fn_71002b58c0(UnkObject_0x100* self, const UnkObject_0x100* other) {
    new (self) UnkStruct_71002b58c0(*reinterpret_cast<const UnkStruct_71002b58c0*>(other));
}
