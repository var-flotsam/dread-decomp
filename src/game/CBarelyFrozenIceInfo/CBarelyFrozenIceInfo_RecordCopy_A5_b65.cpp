// CBarelyFrozenIceInfo's unit: copy constructors of the record of two string ids and a scale (the
// record of fn_71007f1e6c in CBarelyFrozenIceInfo_Ctor_b44.cpp); two identical copies. Names are
// placeholders.
#include "base/global/CStrId.h"
#include "types.h"

#include <new>

using base::global::CStrId;

extern void* g_unk_7101a73e10[];  // the record's vtable

struct UnkFrozenIceRecord_A5_b65 {
    void* vtable;
    CStrId field_0x8;
    CStrId field_0x10;
    f32 field_0x18;

    UnkFrozenIceRecord_A5_b65(const UnkFrozenIceRecord_A5_b65& other)
        : vtable(reinterpret_cast<char*>(g_unk_7101a73e10) + 0x10), field_0x8(other.field_0x8),
          field_0x10(other.field_0x10), field_0x18(other.field_0x18) {}
};

void fn_71007f1edc(UnkFrozenIceRecord_A5_b65* self, const UnkFrozenIceRecord_A5_b65* other) {
    new (self) UnkFrozenIceRecord_A5_b65(*other);
}

void fn_71007f1f40(UnkFrozenIceRecord_A5_b65* self, const UnkFrozenIceRecord_A5_b65* other) {
    new (self) UnkFrozenIceRecord_A5_b65(*other);
}
