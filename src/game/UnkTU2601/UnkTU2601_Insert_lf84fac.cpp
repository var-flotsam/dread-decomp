// Unit 2601: inserts a copy of a {vtable, string id, 16-byte value} element (0x7101cab400's class) at an
// index of an array of them, moving the later elements up one place; inserting at the end is an inlined
// push-back. Either way the array grows first when it is full. Names and layouts are placeholders.
#include <new>

#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
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

void fn_7100f849d0(UnkIdValueVec_b43a3<g_unk_7101cab400>* v, const UnkIdValueVecElem_b43a3<g_unk_7101cab400>& x);

template <>
struct base::UnkIdValueTraits<UnkIdValueVecTag_b43a3<g_unk_7101cab400>> {
    static char* vtable() { return g_unk_7101cab400; }
    static void push_back(UnkIdValueVec_b43a3<g_unk_7101cab400>* v, const UnkIdValueVecElem_b43a3<g_unk_7101cab400>& x) {
        fn_7100f849d0(v, x);
    }
};

void fn_7100f84fac(UnkIdValueVec_b43a3<g_unk_7101cab400>* v, const UnkIdValueVecElem_b43a3<g_unk_7101cab400>* value,
                   u32 index) {
    using T = UnkIdValueVecElem_b43a3<g_unk_7101cab400>;
    u32 cap = v->capacity;
    u32 n = v->count;
    if (n == index) {
        // Appending: the push-back, inlined.
        u32 new_capacity = cap;
        if (cap <= n && v->step != 0)
            new_capacity = (index / v->step + 1) * v->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            base::refl_IdValueMoveTo(v, new_capacity);
            index = v->count;
        }
        new (&v->data[index]) T(*value);
        ++v->count;
        return;
    }
    if (cap <= n && v->step != 0) {
        u32 grown = (n / v->step + 1) * v->step;
        if (cap < grown) {
            base::refl_IdValueMoveTo(v, grown);
            n = v->count;
        }
    }
    // The elements from the last down to the index move up one place (the index runs through -1 when it is 0).
    for (u32 i = n - 1; i >= index; --i) {
        T* from = &v->data[i];
        new (from + 1) T(*from);
        v->data[i].Destroy();
        if (i == 0)
            break;
    }
    new (&v->data[index]) T(*value);
    ++v->count;
}
