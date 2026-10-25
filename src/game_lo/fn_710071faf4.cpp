// Copy constructor of an unidentified record: string ids and file-path ids add a reference,
// a reference-counted object pointer is shared, everything else is copied.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CStrId.h"
#include "types.h"

// String data whose count is a plain member: the original keeps the pool's empty entry in a
// register across the increments.
struct UnkIdData_710071faf4 {
    u8 unk_0x0[0x8];
    s32 ref_count;
};

// Pool is the pool's global pointer: g_unk_7101d47108 for string ids, g_unk_7101d47200 for file-path ids.
template <base::global::UnkStringPool** Pool>
struct UnkId_710071faf4 {
    UnkIdData_710071faf4* data;
    UnkId_710071faf4(const UnkId_710071faf4& other) : data(other.data) {
        if (&(*Pool)->empty != reinterpret_cast<base::global::UnkStringData*>(data))
            ++data->ref_count;
    }
};
using UnkStrId_710071faf4 = UnkId_710071faf4<&g_unk_7101d47108>;
using UnkPathId_710071faf4 = UnkId_710071faf4<&g_unk_7101d47200>;

// The resource of base/UnkCountedResource.h, seen with a plain count: the increment may alias
// anything (the original reloads the pointer it is about to replace after it), the decrement is
// a plain access (through the header's union member the function no longer matches), so the
// pointer is cast for the free.
struct UnkShared_710071faf4 {
    u8 unk_0x0[0x54];
    s32 ref_count;
};
typedef s32 __attribute__((may_alias)) UnkAliasS32_710071faf4;

struct UnkRef_710071faf4 {
    UnkShared_710071faf4* ptr;
    UnkRef_710071faf4(const UnkRef_710071faf4& other) : ptr(nullptr) {
        if (UnkShared_710071faf4* p = other.ptr) {
            ++*reinterpret_cast<UnkAliasS32_710071faf4*>(&p->ref_count);
            if (ptr && --ptr->ref_count == 0)
                fn_710001737c(reinterpret_cast<UnkCountedResource*>(ptr));
            ptr = p;
        }
    }
};

struct UnkBlockA_710071faf4 {
    u32 field_0x0[5];
};
struct UnkBlockB_710071faf4 {
    u8 field_0x0[0x3b];
};
struct UnkBlockC_710071faf4 {
    u64 field_0x0[2];
};

struct UnkStruct_710071faf4 {
    UnkStrId_710071faf4 field_0x0;
    UnkBlockA_710071faf4 field_0x8;
    UnkStrId_710071faf4 field_0x20;
    UnkPathId_710071faf4 field_0x28;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    UnkBlockB_710071faf4 field_0x3c;
    UnkBlockC_710071faf4 field_0x78;
    UnkBlockC_710071faf4 field_0x88;
    UnkStrId_710071faf4 field_0x98;
    UnkPathId_710071faf4 field_0xa0;
    UnkRef_710071faf4 field_0xa8;
    u32 field_0xb0;
    u32 field_0xb4;
    u32 field_0xb8;
    u32 field_0xbc;
    bool field_0xc0;
    u32 field_0xc4;
    u32 field_0xc8;

    UnkStruct_710071faf4(const UnkStruct_710071faf4& o)
        : field_0x0(o.field_0x0), field_0x8(o.field_0x8), field_0x20(o.field_0x20),
          field_0x28(o.field_0x28), field_0x30(o.field_0x30), field_0x34(o.field_0x34),
          field_0x38(o.field_0x38), field_0x3c(o.field_0x3c), field_0x78(o.field_0x78),
          field_0x88(o.field_0x88),
          field_0x98(o.field_0x98), field_0xa0(o.field_0xa0), field_0xa8(o.field_0xa8),
          field_0xb0(o.field_0xb0), field_0xb4(o.field_0xb4), field_0xb8(o.field_0xb8),
          field_0xbc(o.field_0xbc), field_0xc0(o.field_0xc0), field_0xc4(o.field_0xc4),
          field_0xc8(o.field_0xc8) {}
};
static_assert(sizeof(UnkStruct_710071faf4) == 0xd0, "layout");

// The constructor is inlined into the original; flatten keeps it from being emitted out of line.
__attribute__((flatten)) void fn_710071faf4(UnkStruct_710071faf4* self, const UnkStruct_710071faf4* other) {
    new (self) UnkStruct_710071faf4(*other);
}
