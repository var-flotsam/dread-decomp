// Copies of the generated fn_71007abf80 (src/clones/clones_02.cpp) with 64-bit indices: move
// constructors of iterator values whose elements are moved out of line (tail calls).
// 0xb0-byte elements: fn_71000dbc34 (fn_7100097d18), fn_71000dbd7c (fn_71000e6fd8).
// 0xc8-byte elements: fn_7100260bd0 (fn_710025b278), fn_7100260df8 (fn_7100260718).
#include "base/UnkCollectionIter.h"
#include "types.h"

template <void (*Move)(void*, void*), u64 Size>
struct UnkMovedElem {
    u64 words[Size / 8];

    __attribute__((always_inline)) UnkMovedElem(UnkMovedElem&& other) { Move(this, &other); }
};

void fn_7100097d18(void* self, void* other);
void fn_71000e6fd8(void* self, void* other);
void fn_710025b278(void* self, void* other);
void fn_7100260718(void* self, void* other);

__attribute__((flatten)) void fn_71000dbc34(void* dst, const void* src) {
    base::refl_IterValueMove<UnkMovedElem<fn_7100097d18, 0xb0>, u64>(dst, src);
}

__attribute__((flatten)) void fn_71000dbd7c(void* dst, const void* src) {
    base::refl_IterValueMove<UnkMovedElem<fn_71000e6fd8, 0xb0>, u64>(dst, src);
}

__attribute__((flatten)) void fn_7100260bd0(void* dst, const void* src) {
    base::refl_IterValueMove<UnkMovedElem<fn_710025b278, 0xc8>, u64>(dst, src);
}

__attribute__((flatten)) void fn_7100260df8(void* dst, const void* src) {
    base::refl_IterValueMove<UnkMovedElem<fn_7100260718, 0xc8>, u64>(dst, src);
}
