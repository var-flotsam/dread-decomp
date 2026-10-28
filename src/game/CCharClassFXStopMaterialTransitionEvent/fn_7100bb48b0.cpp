// Copy assignments of records of a word, two string ids and a last field (8, 4 or 1 bytes),
// past their first eight bytes. Eight copies.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

template <class T>
struct UnkStrIdPairValue {
    u8 unk_0x0[0x8];
    u32 field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    T field_0x20;

    __attribute__((always_inline)) UnkStrIdPairValue& operator=(const UnkStrIdPairValue& other) {
        if (this != &other) {
            field_0x8 = other.field_0x8;
            field_0x10 = other.field_0x10;
            field_0x18 = other.field_0x18;
            field_0x20 = other.field_0x20;
        }
        return *this;
    }
};

#define ASSIGN(addr, T)                                                                        \
    UnkStrIdPairValue<T>* fn_##addr(UnkStrIdPairValue<T>* self, const UnkStrIdPairValue<T>* other) { \
        return &(*self = *other);                                                              \
    }

ASSIGN(7100bb48b0, u64)
ASSIGN(7100bb49ac, u64)
ASSIGN(7100cadb08, u32)
ASSIGN(7100cadc04, u32)
ASSIGN(7100be0500, bool)
ASSIGN(7100be05fc, bool)
ASSIGN(7100cd9b8c, bool)
ASSIGN(7100cd9c88, bool)
