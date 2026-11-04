// Unit 108: collection slot 0x110 of a reflected array of 0x48-byte elements, each starting with
// an array (base::UnkVector) freed on destruction: Clear destroys every element, keeping the
// storage (fn_710011ec9c). Names are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkElem48_b60 {
    base::UnkVector<u8> items;
    u8 pad_0x28[0x20];

    ~UnkElem48_b60() { items.Free(); }
};
static_assert(sizeof(UnkElem48_b60) == 0x48, "UnkElem48_b60 layout");

void fn_710011ec9c(base::UnkVector<UnkElem48_b60>* vec) {
    vec->Clear();
}
