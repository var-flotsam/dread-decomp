// Copy constructor (returns this) of a four-level object (each level sets its own vtable and copies
// its members): the string ids at 0x8, 0x10 and 0x40 are copied (counting a use on each non-empty
// one) and the words at 0x18..0x37 and 0x38 are copied plainly.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkPair {
    u64 field_0x0;
    u64 field_0x8;
};

// A string id held as a bare pointer: the level constructors build it in the body (after the
// vtable store), so it needs no constructor of its own.
struct UnkId {
    UnkStringData* data;
};

// What CStrId's copy constructor does.
__attribute__((always_inline)) static inline void CopyId(UnkId& dst, const UnkId& src) {
    dst.data = src.data;
    if (&g_unk_7101d47108->empty != dst.data)
        ++dst.data->ref_count;
}

extern void* g_unk_7101a21718[];
extern void* g_unk_7101a21af8[];
extern void* g_unk_7101a21ef0[];
extern void* g_unk_7101a21fc8[];

struct UnkLevelA {
    void** vtable;
    UnkId field_0x8;

    UnkLevelA(const UnkLevelA& other) {
        vtable = g_unk_7101a21718 + 2;
        CopyId(field_0x8, other.field_0x8);
    }
};

struct UnkLevelB : UnkLevelA {
    UnkId field_0x10;
    UnkPair field_0x18;

    UnkLevelB(const UnkLevelB& other) : UnkLevelA(other) {
        vtable = g_unk_7101a21af8 + 2;
        CopyId(field_0x10, other.field_0x10);
        field_0x18 = other.field_0x18;
    }
};

struct UnkLevelC : UnkLevelB {
    UnkPair field_0x28;
    u32 field_0x38;
    u8 pad_0x3c[4];

    UnkLevelC(const UnkLevelC& other) : UnkLevelB(other) {
        vtable = g_unk_7101a21fc8 + 2;
        field_0x28 = other.field_0x28;
        field_0x38 = other.field_0x38;
    }
};

struct UnkLevelD : UnkLevelC {
    UnkId field_0x40;

    UnkLevelD(const UnkLevelD& other) : UnkLevelC(other) {
        vtable = g_unk_7101a21ef0 + 2;
        CopyId(field_0x40, other.field_0x40);
    }
};

// NON_MATCHING(minor): the copied words at 0x28..0x38 are held in other registers (x10/x11 and w9
// instead of x9/x10 and w11)
UnkLevelD* fn_71002e5db0(UnkLevelD* self, const UnkLevelD* other) {
    new (self) UnkLevelD(*other);
    return self;
}
