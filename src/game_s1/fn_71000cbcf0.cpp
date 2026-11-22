// Copies of fn_7100e05764 (src/game/SCameraRailNode/fn_7100c0b7a4.cpp) for another element type, naming their own callees and
// data (the comments are adapted from the original's): fn_71000cbcf0, fn_710011dac0. Copy-assignments of growable
// arrays (base::global::CRntVector's layout) of plain elements copied one by one: the array is emptied, reserves the other's
// capacity and appends each element, growing like PushBack (including its hang).
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkCopiedVector.h"
#include "game/UnkTU108.h"
#include "game/UnkTU83.h"
#include "types.h"

// Three words copied piece by piece.
struct UnkThreeWords {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;

    UnkThreeWords(const UnkThreeWords& other) {
        field_0x0 = other.field_0x0;
        field_0x4 = other.field_0x4;
        field_0x8 = other.field_0x8;
    }
};

UnkCopiedVector<UnkThreeWords>* fn_71000cbcf0(UnkCopiedVector<UnkThreeWords>* self,
                                              const UnkCopiedVector<UnkThreeWords>* other) {
    return &(*self = *other);
}
UnkCopiedVector<UnkThreeWords>* fn_710011dac0(UnkCopiedVector<UnkThreeWords>* self,
                                              const UnkCopiedVector<UnkThreeWords>* other) {
    return &(*self = *other);
}
