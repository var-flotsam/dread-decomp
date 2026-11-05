// game::logic::collision::CCollider's unit (2503): reflection helpers with the same code as other units'.
// Names are placeholders.
#include "../../UnkMovedArray_b52.h"
#include "base/UnkVector.h"
#include "types.h"
// Collection slot 0x110 of a reflected array of arrays (0x28 bytes each, freed on destruction): Clear, the
// same code as fn_710011ec9c (src/base/UnkUnit108/UnkUnit108_VecClear_b60.cpp).
struct UnkElem28_A6_b63 {
    base::UnkVector<u8> items;

    ~UnkElem28_A6_b63() { items.Free(); }
};
static_assert(sizeof(UnkElem28_A6_b63) == 0x28, "UnkElem28_A6_b63 layout");

void fn_7100d4372c(base::UnkVector<UnkElem28_A6_b63>* vec) {
    vec->Clear();
}

// The move helper (CType+0x38) of the iterator value over a reflected array of arrays (as fn_71005ee198).
void fn_7100d42f98(void* dst, const void* src) {
    base::refl_IterValueMove<UnkMovedArray_b52>(dst, src);
}
