// Constructor tail: runs the base constructor, clears the state at 0x58..0x75 and, when the owner
// has an object at +0x298, sends it a query record whose direction is the third column of the
// owner's matrix (fn_710002614c). Finishes with fn_7100426b54 on a fixed key.
#include "base/UnkMatrix44.h"
#include "game_s2/UnkChozoCommanderAttack.h"
#include "game_s2/UnkParams.h"
#include "types.h"

extern void* g_unk_7101a24b58[];

struct UnkQuery {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 field_0x14;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;

    UnkQuery() {
        field_0x1c = 0;
        field_0x34 = 0;
        field_0x20 = 0;
        field_0x28 = 0;
        field_0x30 = 0;
        vtable = g_unk_7101a24b58 + 2;
        __builtin_memset(&field_0x8, 0, 13);
    }
};

struct UnkTarget {
    u8 pad_0x0[0x298];
    void* field_0x298;
};

struct UnkOwner {
    u8 pad_0x0[0x38];
    u8 field_0x38[1];
    u8 pad_0x39[0x25f];
    void* field_0x298;
};

struct UnkSelf {
    u8 pad_0x0[8];
    UnkOwner* field_0x8;
    u8 pad_0x10[0x48];
    u8 field_0x58;
    u8 pad_0x59[3];
    u8 field_0x5c[0x20];
};

void fn_710042bb3c(UnkSelf* self);

// The bit pattern of a float (by value: the by-reference form the other callers use changes this
// non-matching function's code).
static inline u32 AsU32(f32 value) {
    u32 bits;
    __builtin_memcpy(&bits, &value, 4);
    return bits;
}

// NON_MATCHING(minor): the zeroing of the query direction (overwritten right after) is optimised
// out, so the stack setup is two stores shorter
void fn_7100438b48(UnkSelf* self) {
    fn_710042bb3c(self);
    UnkOwner* owner = self->field_0x8;
    self->field_0x58 = 0;
    __builtin_memset(self->field_0x5c, 0, 13);
    __builtin_memset(self->field_0x5c + 0x10, 0, 10);
    if (owner->field_0x298) {
        UnkQuery query;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
        u32 x = AsU32(m.m[2]);  // the third column (the facing), as bit patterns
        u32 y = AsU32(m.m[6]);
        u32 z = AsU32(m.m[10]);
        query.field_0x8 = x;
        query.field_0xc = y;
        query.field_0x10 = z;
        query.field_0x14 = 1;
        fn_71006f4b0c(self->field_0x8->field_0x298, &query, 1);
    }
    fn_7100426b54(reinterpret_cast<UnkChozoCommanderAttack*>(self), 0x8f4e8a524ebacc1bull, false);
}
