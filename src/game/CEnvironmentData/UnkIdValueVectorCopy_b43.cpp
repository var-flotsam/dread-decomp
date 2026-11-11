// The copy constructor and copy assignment of arrays (base::UnkVector) of polymorphic {vtable, string
// id, value} elements (base::UnkIdValueElem, include/base/UnkCloneHelpers.h), one copy per
// element class: reset and copy the step, flags and allocator (the assignment destroys the elements
// instead), grow to the other's capacity (the elements copied over and destroyed through their
// vtables), then push a copy of each of the other's elements with the class's out-of-line push-back.
// The assignment is base::refl_IdValueAssign. Copies in CEnvironmentData::SFogTransition's unit
// (vtables 0x7101cab220, 0x7101cab338) and in unit 2601 of tools/tu_map.py (0x7101cab400). Names and
// layouts are placeholders.
#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "game/UnkTU2599.h"
#include "game/UnkTU2601.h"
#include "types.h"

#include <new>

template <char* Vtable>
struct UnkIdValueVecTag_b43a3;

// The value is 24 bytes in 0x7101cab338's class, 16 in the others'.
template <char* Vtable>
struct UnkIdValueVecSize_b43a3 {
    static constexpr u64 value = 16;
};
template <>
struct UnkIdValueVecSize_b43a3<g_unk_7101cab338> {
    static constexpr u64 value = 24;
};
template <char* Vtable>
using UnkIdValueVecElem_b43a3 = base::UnkIdValueElem<UnkIdValueVecTag_b43a3<Vtable>,
                                                     UnkCloneElem<UnkIdValueVecTag_b43a3<Vtable>, UnkIdValueVecSize_b43a3<Vtable>::value>>;
template <char* Vtable>
using UnkIdValueVec_b43a3 = base::UnkVector<UnkIdValueVecElem_b43a3<Vtable>>;

// The push-backs, out of line.
void fn_7100f5db00(UnkIdValueVec_b43a3<g_unk_7101cab220>* v, const UnkIdValueVecElem_b43a3<g_unk_7101cab220>& x);
void fn_7100f6e784(UnkIdValueVec_b43a3<g_unk_7101cab338>* v, const UnkIdValueVecElem_b43a3<g_unk_7101cab338>& x);
void fn_7100f849d0(UnkIdValueVec_b43a3<g_unk_7101cab400>* v, const UnkIdValueVecElem_b43a3<g_unk_7101cab400>& x);

template <char* Vtable>
struct base::UnkIdValueTraits<UnkIdValueVecTag_b43a3<Vtable>> {
    static char* vtable() { return Vtable; }
    static void push_back(UnkIdValueVec_b43a3<Vtable>* v, const UnkIdValueVecElem_b43a3<Vtable>& x);
};

template <>
inline void base::UnkIdValueTraits<UnkIdValueVecTag_b43a3<g_unk_7101cab220>>::push_back(
    UnkIdValueVec_b43a3<g_unk_7101cab220>* v, const UnkIdValueVecElem_b43a3<g_unk_7101cab220>& x) {
    fn_7100f5db00(v, x);
}
template <>
inline void base::UnkIdValueTraits<UnkIdValueVecTag_b43a3<g_unk_7101cab338>>::push_back(
    UnkIdValueVec_b43a3<g_unk_7101cab338>* v, const UnkIdValueVecElem_b43a3<g_unk_7101cab338>& x) {
    fn_7100f6e784(v, x);
}
template <>
inline void base::UnkIdValueTraits<UnkIdValueVecTag_b43a3<g_unk_7101cab400>>::push_back(
    UnkIdValueVec_b43a3<g_unk_7101cab400>* v, const UnkIdValueVecElem_b43a3<g_unk_7101cab400>& x) {
    fn_7100f849d0(v, x);
}

template <char* Vtable>
static inline __attribute__((always_inline)) void CopyConstruct_b43a3(UnkIdValueVec_b43a3<Vtable>* self,
                                                                      const UnkIdValueVec_b43a3<Vtable>* other) {
    using Traits = base::UnkIdValueTraits<UnkIdValueVecTag_b43a3<Vtable>>;
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    u32 cap = other->capacity;
    if (self->capacity < cap)
        base::refl_IdValueMoveTo(self, cap);
    for (const UnkIdValueVecElem_b43a3<Vtable> *p = other->data, *end = p + other->count; p != end; ++p)
        Traits::push_back(self, *p);
}

#define UNK_ID_VALUE_VEC_COPY_B43A3(addr, vtable)                                                        \
    void fn_##addr(UnkIdValueVec_b43a3<vtable>* self, const UnkIdValueVec_b43a3<vtable>* other) {        \
        CopyConstruct_b43a3<vtable>(self, other);                                                        \
    }
#define UNK_ID_VALUE_VEC_ASSIGN_B43A3(addr, vtable)                                                      \
    UnkIdValueVec_b43a3<vtable>* fn_##addr(UnkIdValueVec_b43a3<vtable>* self,                           \
                                           const UnkIdValueVec_b43a3<vtable>* other) {                  \
        return base::refl_IdValueAssign(self, other);                                                    \
    }

UNK_ID_VALUE_VEC_COPY_B43A3(7100f5d998, g_unk_7101cab220)
UNK_ID_VALUE_VEC_ASSIGN_B43A3(7100f5dcc4, g_unk_7101cab220)
UNK_ID_VALUE_VEC_COPY_B43A3(7100f6e60c, g_unk_7101cab338)
UNK_ID_VALUE_VEC_COPY_B43A3(7100f84868, g_unk_7101cab400)
UNK_ID_VALUE_VEC_ASSIGN_B43A3(7100f84b94, g_unk_7101cab400)
