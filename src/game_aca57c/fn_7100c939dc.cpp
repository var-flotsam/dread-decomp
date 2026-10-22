// Two constructors of a record: a copy of the global id g_unk_7101d470e0, a word (zero or the
// argument), the "all" mask of two function-local static masks, then zeroed fields.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_7101d470e0;

struct UnkMaskA {
    u32 value;
    UnkMaskA() : value(0) {}
};

struct UnkMaskB {
    u32 value;
    UnkMaskB(u8 v) : value(v) {}
};

inline const UnkMaskA& UnkMaskNone_7101d57cc8() {
    static UnkMaskA s_none;
    return s_none;
}

inline const UnkMaskB& UnkMaskAll_7101d57cb8() {
    static UnkMaskB s_all((u8)~*reinterpret_cast<const u8*>(&UnkMaskNone_7101d57cc8().value));
    return s_all;
}

struct UnkMaskedId {
    CStrId id;
    u32 field_0x8;
    u32 mask;
    u64 field_0x10;
    u32 field_0x18;
    bool field_0x1c;

    __attribute__((always_inline)) explicit UnkMaskedId(u32 v)
        : id(g_unk_7101d470e0), field_0x8(v), mask(UnkMaskAll_7101d57cb8().value), field_0x10(0),
          field_0x18(0), field_0x1c(false) {}
};

void fn_7100c939dc(UnkMaskedId* self) { new (self) UnkMaskedId(0); }
void fn_7100c93aec(UnkMaskedId* self, u32 v) { new (self) UnkMaskedId(v); }
