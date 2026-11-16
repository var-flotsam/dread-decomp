// Unit 2601: the element at an index of an array of {vtable, string id, 16-byte value} elements
// (0x7101cab400's class), growing the array up to it with value-initialised elements when growing is
// allowed: CRntVector::GrowAt for these elements. Names and layouts are placeholders.
#include <new>

#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2601.h"
#include "types.h"

// The array's elements, named as src/game/CEnvironmentData/UnkIdValueVectorCopy_b43.cpp.
template <char* Vtable>
struct UnkIdValueVecTag_b43a3;

template <char* Vtable>
struct UnkIdValueVecSize_b43a3 {
    static constexpr u64 value = 16;
};
template <char* Vtable>
using UnkIdValueVecElem_b43a3 = base::UnkIdValueElem<UnkIdValueVecTag_b43a3<Vtable>,
                                                     UnkCloneElem<UnkIdValueVecTag_b43a3<Vtable>, UnkIdValueVecSize_b43a3<Vtable>::value>>;
template <char* Vtable>
using UnkIdValueVec_b43a3 = base::global::CRntVector<UnkIdValueVecElem_b43a3<Vtable>>;

template <>
struct base::UnkIdValueTraits<UnkIdValueVecTag_b43a3<g_unk_7101cab400>> {
    static char* vtable() { return g_unk_7101cab400; }
};

// A new element as its constructors build it: the base class's vtable and an empty string id, then
// the class's own vtable and the value's last word (the two constructors inlined into one).
struct UnkIdValueNew_lf85380 {
    char* vtable;
    base::global::CStrId id;
    u64 field_0x10;
    u32 field_0x18;
    u32 field_0x1c;

    UnkIdValueNew_lf85380();
};

inline UnkIdValueNew_lf85380::UnkIdValueNew_lf85380()
    : vtable(g_unk_7101cab1f8 + 0x10), id(base::global::UnkStrIdInline(), nullptr, false) {
    vtable = g_unk_7101cab400 + 0x10;
    field_0x1c = 0;
}

// Value-initialised: zeroed, then constructed.
struct UnkIdValueSlot_lf85380 {
    UnkIdValueNew_lf85380 elem;
};
static_assert(sizeof(UnkIdValueSlot_lf85380) == sizeof(UnkIdValueVecElem_b43a3<g_unk_7101cab400>), "element size");

UnkIdValueVecElem_b43a3<g_unk_7101cab400>* fn_7100f85380(UnkIdValueVec_b43a3<g_unk_7101cab400>* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 elements.
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                base::refl_IdValueMoveTo(self, grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            base::refl_IdValueMoveTo(self, n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkIdValueSlot_lf85380();
        self->count = n;
    }
    return &self->data[index];
}
